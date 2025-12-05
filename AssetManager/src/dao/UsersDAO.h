#pragma once

#include <QString>
#include <QSqlRecord>
#include <optional>
#include "src/models/User.h"

class UsersDAO
{
public:
    UsersDAO() = default;
    ~UsersDAO() = default;

    QVector<User> getAll() const;
    std::optional<User> getUserById(int userId) const;
    std::optional<User> getUserByBarcode(const QString& userBarcode) const;
    bool existsBarcode(const QString& barcode, int excludeId) const;

    bool insert(const User& user) const;
    bool update(const User& user) const;
    bool remove(const int userId) const;

private:
    User gatherUserData(const QSqlRecord& record) const;
};
