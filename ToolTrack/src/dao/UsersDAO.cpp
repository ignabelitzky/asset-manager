#include <QSqlQuery>
#include <QSqlError>
#include "UsersDAO.h"
#include "src/core/DatabaseManager.h"

QVector<User> UsersDAO::getAll() const
{
    QVector<User> result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, first_name, last_name, barcode
        FROM users
    )");
    if (!query.exec())
    {
        qDebug() << "[USERSDAO] Error in getAll:" << query.lastError().text();
        return result;
    }
    while (query.next())
    {
        QSqlRecord record = query.record();
        result.append(gatherUserData(record));
    }
    return result;
}

std::optional<User> UsersDAO::getUserById(int userId) const
{
    User user;

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, first_name, last_name, barcode
        FROM users
        WHERE id = :id
    )");
    query.bindValue(":id", userId);

    if (!query.exec())
    {
        qDebug() << "[USERSDAO] Error in getUserById:" << query.lastError().text();
        return std::nullopt;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        user = gatherUserData(record);
    }
    return user;
}

std::optional<User> UsersDAO::getUserByBarcode(const QString& userBarcode) const
{
    User user;

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, first_name, last_name, barcode
        FROM users
        WHERE barcode = :barcode
    )");
    query.bindValue(":barcode", userBarcode);

    if (!query.exec())
    {
        qDebug() << "[USERSDAO] Error in getUserByBarcode:" << query.lastError().text();
        return std::nullopt;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        user = gatherUserData(record);
    }
    return user;
}

bool UsersDAO::existsBarcode(const QString& barcode, int excludeId) const
{
    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT COUNT(*) FROM users
        WHERE barcode = :barcode AND id != :excludeId
    )");
    query.bindValue(":barcode", barcode);
    query.bindValue(":excludeId", excludeId);

    if (!query.exec())
        return false;

    query.next();
    return query.value(0).toInt() > 0;
}

bool UsersDAO::insert(const User& user) const
{
    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        INSERT INTO users (first_name, last_name, barcode)
        VALUES (:first_name, :last_name, :barcode)
    )");
    query.bindValue(":first_name", user.firstName());
    query.bindValue(":last_name", user.lastName());
    query.bindValue(":barcode", user.barcode());

    if (!query.exec())
    {
        qDebug() << "[USERSDAO] Error in insert:" << query.lastError().text();
        return false;
    }
    return true;
}

bool UsersDAO::update(const User& user) const
{
    if (user.id() <= 0)
    {
        qDebug() << "[USERSDAO] update called with invalid id:" << user.id();
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        UPDATE users
        SET first_name = :first_name,
            last_name = :last_name,
            barcode = :barcode
        WHERE id = :id
    )");
    query.bindValue(":first_name", user.firstName());
    query.bindValue(":last_name", user.lastName());
    query.bindValue(":barcode", user.barcode());
    query.bindValue(":id", user.id());

    if (!query.exec())
    {
        qDebug() << "[USERSDAO] Error in update:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool UsersDAO::remove(const int userId) const
{
    if (userId < 0)
    {
        qDebug() << "[USERSDAO] remove called with invalid id:" << userId;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        DELETE FROM users
        WHERE id = :id
    )");
    query.bindValue(":id", userId);

    if (!query.exec())
    {
        qDebug() << "[USERSDAO] Error in remove:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

User UsersDAO::gatherUserData(const QSqlRecord& record) const
{
    User user;
    user.setId(record.value("id").toInt());
    user.setFirstName(record.value("first_name").toString());
    user.setLastName(record.value("last_name").toString());
    user.setBarcode(record.value("barcode").toString());
    return user;
}
