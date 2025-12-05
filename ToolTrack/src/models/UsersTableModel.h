#pragma once

#include <QAbstractTableModel>
#include <QObject>
#include "src/dao/UsersDAO.h"
#include "src/models/User.h"

class UsersTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit UsersTableModel(UsersDAO& usersDAO,
                             QObject* parent = nullptr);
    ~UsersTableModel() = default;

    enum {
        First_Name,
        Last_Name,
        Barcode,
        COLUMN_COUNT
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // Custom actions
    void refresh();
    const User& getUser(int row) const;

private:
    UsersDAO& m_usersDAO;
    QVector<User> m_users;
};
