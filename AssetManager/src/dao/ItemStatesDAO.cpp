#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "ItemStatesDAO.h"
#include "src/core/DatabaseManager.h"

QVector<ItemState> ItemStatesDAO::getAll() const
{
    QVector<ItemState> result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name
        FROM item_states
    )");

    if (!query.exec())
    {
        qDebug() << "[ITEMSTATESDAO] Error in getAll:" << query.lastError().text();
        return result;
    }
    while (query.next())
    {
        QSqlRecord record = query.record();
        result.append({record.value("id").toInt(), record.value("name").toString()});
    }
    return result;
}

ItemState ItemStatesDAO::getById(int id) const
{
    ItemState result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name
        FROM item_states
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[ITEMSTATESDAO] Error in getById:" << query.lastError().text();
        return result;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        result = {record.value("id").toInt(), record.value("name").toString()};
    }
    return result;
}

QString ItemStatesDAO::getNameById(int id) const
{
    QString result{};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT name
        FROM item_states
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[ITEMSTATESDAO] Error in getNameById:" << query.lastError().text();
        return result;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        result = record.value("name").toString();
    }
    return result;
}

bool ItemStatesDAO::insert(const QString& name)
{
    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        INSERT INTO item_states (name) VALUES (:name)
    )");
    query.bindValue(":name", name);

    if (!query.exec())
    {
        qDebug() << "[ITEMSTATESDAO] Error in insert:" << query.lastError().text();
        return false;
    }
    return true;
}

bool ItemStatesDAO::update(int id, const QString& name)
{
    if (id <= 0)
    {
        qDebug() << "[ITEMSTATESDAO] update called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        UPDATE item_states
        SET name = :name
        WHERE id = :id
    )");
    query.bindValue(":name", name);
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[ITEMSTATESDAO] Error in update:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool ItemStatesDAO::remove(int id)
{
    if (id < 0)
    {
        qDebug() << "[ITEMSTATESDAO] remove called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        DELETE FROM item_states
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[ITEMSTATESDAO] Error in remove:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}
