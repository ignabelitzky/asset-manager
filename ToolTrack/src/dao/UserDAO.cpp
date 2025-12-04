#include <QSqlQuery>
#include <QSqlError>
#include "UserDAO.h"
#include "src/core/DatabaseManager.h"

QVector<User> UserDAO::getAll() const
{
    QVector<User> result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, first_name, last_name, barcode
        FROM users
    )");
    if (!query.exec())
    {
        qDebug() << "[USERDAO] Error in getAll:" << query.lastError().text();
        return result;
    }
    while (query.next())
    {
        QSqlRecord record = query.record();
        result.append(gatherUserData(record));
    }
    return result;
}

std::optional<User> UserDAO::getUserById(int userId) const
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
        qDebug() << "[USERDAO] Error in getUserById:" << query.lastError().text();
        return std::nullopt;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        user = gatherUserData(record);
    }
    return user;
}

std::optional<User> UserDAO::getUserByBarcode(const QString& userBarcode) const
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
        qDebug() << "[USERDAO] Error in getUserByBarcode:" << query.lastError().text();
        return std::nullopt;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        user = gatherUserData(record);
    }
    return user;
}

bool UserDAO::addUser(const User& user) const
{
    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        INSERT INTO users (first_name, last_name, barcode)
        VALUES (:first_name, :last_name, :barcode)
    )");
    query.bindValue(":first_name", user.firstName());
    query.bindValue(":last_name", user.lastName());
    query.bindValue(":barcode", user.barcode());

    if(!query.exec())
    {
        qDebug() << "[USERDAO] Error in addUser:" << query.lastError().text();
        return false;
    }
    return true;
}

bool UserDAO::updateUser(const User& user) const
{
    if (user.id() <= 0)
    {
        qDebug() << "[USERDAO] updateUser called with invalid id:" << user.id();
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
        qDebug() << "[USERDAO] Error in updateUser:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool UserDAO::deleteUser(const int userId) const
{
    if (userId < 0)
    {
        qDebug() << "[USERDAO] deleteUser called with invalid id:" << userId;
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
        qDebug() << "[USERDAO] Error in deleteUser:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

User UserDAO::gatherUserData(const QSqlRecord& record) const
{
    User user;
    user.setId(record.value("id").toInt());
    user.setFirstName(record.value("first_name").toString());
    user.setLastName(record.value("last_name").toString());
    user.setBarcode(record.value("barcode").toString());
    return user;
}
