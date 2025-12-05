#include <QSqlQuery>
#include <QSqlError>
#include "CheckoutDAO.h"
#include "src/core/DatabaseManager.h"

CheckoutDAO::CheckoutDAO(ItemsDAO& itemsDAO)
    : m_itemsDAO(itemsDAO)
{
}

bool CheckoutDAO::checkoutItem(int userId, int itemId)
{
    QSqlDatabase db = DatabaseManager::instance().db();
    if (!db.isOpen() && !db.open()) {
        qWarning() << "checkoutItem: no se pudo abrir la base:" << db.lastError().text();
        return false;
    }

    if (!db.transaction()) {
        qWarning() << "checkoutItem: no se pudo iniciar transacción";
        return false;
    }

    // Insert en tabla checkout
    {
        QSqlQuery q(db);
        q.prepare("INSERT INTO items_checkout (user_id, item_id, checkout_at) "
                  "VALUES (:u, :i, CURRENT_TIMESTAMP)");
        q.bindValue(":u", userId);
        q.bindValue(":i", itemId);

        if (!q.exec()) {
            qWarning() << "checkoutItem INSERT ERROR:" << q.lastError().text();
            db.rollback();
            return false;
        }
    }

    if (!db.commit()) {
        qWarning() << "checkoutItem COMMIT ERROR:" << db.lastError().text();
        db.rollback();
        return false;
    }

    return true;
}


bool CheckoutDAO::returnItem(int checkoutId)
{
    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        UPDATE items_checkout
        SET returned_at = CURRENT_TIMESTAMP
        WHERE id = :id AND returned_at IS NULL
    )");
    query.bindValue(":id", checkoutId);

    if (!query.exec())
    {
        qDebug() << "[CHECKOUTDAO] returnItem exec failed:" << query.lastError().text();
        return false;
    }

    if (query.numRowsAffected() == 0)
    {
        qDebug() << "[CHECKOUTDAO] returnItem: no active checkout with id=" << checkoutId;
        return false;
    }

    return true;
}

std::optional<Checkout> CheckoutDAO::getActiveCheckout(int itemId)
{
    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, user_id, item_id, checkout_at, returned_at
        FROM items_checkout
        WHERE item_id = :item_id AND returned_at IS NULL
        LIMIT 1
    )");
    query.bindValue(":item_id", itemId);

    if (!query.exec())
    {
        qDebug() << "[CHECKOUTDAO] getActiveCheckout exec failed:" << query.lastError().text();
        return std::nullopt;
    }

    if (!query.next())
        return std::nullopt;

    return buildCheckout(query.record());
}

QVector<Checkout> CheckoutDAO::listActive()
{
    QVector<Checkout> list;

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, user_id, item_id, checkout_at, returned_at
        FROM items_checkout
        WHERE returned_at IS NULL
        ORDER BY checkout_at ASC
    )");

    if (!query.exec())
    {
        qDebug() << "[CHECKOUTDAO] listActive exec failed:" << query.lastError().text();
        return list;
    }

    while (query.next())
    {
        std::optional<Checkout> activeCheckout = buildCheckout(query.record());
        if (activeCheckout.has_value())
            list.push_back(*activeCheckout);
    }

    return list;
}

bool CheckoutDAO::checkoutMultiple(int userId, const QMap<int, int>& items)
{
    if (items.isEmpty())
        return false;

    QSqlDatabase db = DatabaseManager::instance().db();
    if (!db.isValid())
    {
        qDebug() << "[CHECKOUTDAO] DB invalid";
        return false;
    }

    if (!db.transaction())
    {
        qDebug() << "[CHECKOUTDAO] Failed to start transaction:" <<db.lastError().text();
        return false;
    }

    // Re-check availability for each item
    for (auto it = items.constBegin(); it != items.constEnd(); ++it)
    {
        const int itemId = it.key();
        const int qty = it.value();
        if (qty <= 0)
        {
            qDebug() << "[CHECKOUTDAO] Invalid qty for item" << itemId;
            db.rollback();
            return false;
        }

        // Get total stock
        QSqlQuery qStock(db);
        qStock.prepare(R"(
            SELECT stock FROM items WHERE id = :id
        )");
        qStock.bindValue(":id", itemId);
        if (!qStock.exec() || !qStock.next())
        {
            qDebug() << "[CHECKOUTDAO] Failed to get stock for item" << itemId << ":" << qStock.lastError().text();
            db.rollback();
            return false;
        }
        const int totalStock = qStock.value(0).toInt();

        // Count currently checked out
        QSqlQuery qCount(db);
        qCount.prepare(R"(
            SELECT COUNT(*) FROM items_checkout
            WHERE item_id = :item_id AND returned_at IS NULL
        )");
        qCount.bindValue(":item_id", itemId);
        if (!qCount.exec() || !qCount.next())
        {
            qDebug() << "[CHECKOUTDAO] Failed to count checkouts for item" << itemId << ":" << qCount.lastError().text();
            db.rollback();
            return false;
        }
        const int checkedOut = qCount.value(0).toInt();
        const int available = totalStock - checkedOut;
        if (available < qty)
        {
            qDebug() << "[CHECKOUTDAO] Not enough stock for item" << itemId << "available=" << available << "requested=" << qty;
            db.rollback();
            return false;
        }
    }

    // All availability checks passed - insert required rows
    QSqlQuery qInsert(db);
    qInsert.prepare(R"(
        INSERT INTO items_checkout (user_id, item_id, checkout_at)
        VALUES (:user_id, :item_id, CURRENT_TIMESTAMP)
    )");

    for (auto it = items.constBegin(); it != items.constEnd(); ++it)
    {
        const int itemId = it.key();
        const int qty = it.value();

        for (int i = 0; i < qty; ++i)
        {
            qInsert.bindValue(":user_id", userId);
            qInsert.bindValue(":item_id", itemId);

            if (!qInsert.exec())
            {
                qDebug() << "[CHECKOUTDAO] Failed to insert checkout for item" << itemId << ":" << qInsert.lastError().text();
                db.rollback();
                return false;
            }
        }
    }

    if (!db.commit())
    {
        qDebug() << "[CHECKOUTDAO] Failed to commit transaction:" << db.lastError().text();
        db.rollback();
        return false;
    }

    return true;
}

std::optional<Checkout> CheckoutDAO::buildCheckout(const QSqlRecord& record) const
{
    Checkout result;
    result.id = record.value("id").toInt();
    result.userId = record.value("user_id").toInt();
    result.itemId = record.value("item_id").toInt();

    result.checkoutAt = QDateTime::fromString(record.value("checkout_at").toString(), Qt::ISODate);

    if (!record.value("returned_at").isNull())
        result.returnedAt = QDateTime::fromString(record.value("returned_at").toString(), Qt::ISODate);
    else
        result.returnedAt.reset();

    return result;
}
