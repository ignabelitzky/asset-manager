#pragma once

#include <QString>
#include <QSqlRecord>
#include <optional>
#include "src/models/User.h"

class UserDAO
{
public:
    UserDAO() = default;
    ~UserDAO() = default;

    QVector<User> getAll() const;
    std::optional<User> getUserById(int userId) const;
    std::optional<User> getUserByBarcode(const QString& userBarcode) const;

    bool addUser(const User& user) const;
    bool updateUser(const User& user) const;
    bool deleteUser(const int userId) const;

private:
    User gatherUserData(const QSqlRecord& record) const;
};
