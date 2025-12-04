#include <QSqlQuery>
#include <QSqlError>
#include "UserDAO.h"
#include "src/core/DatabaseManager.h"

std::optional<User> UserDAO::getUserByBarcode(const QString& userBarcode) const
{
    // TODO
    return std::nullopt;
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
    // TODO
    return false;
}

bool UserDAO::deleteUser(const int userId) const
{
    //TODO
    return false;
}

User UserDAO::gatherUserData(const QSqlRecord& record)
{
    //TODO
    return User();
}
