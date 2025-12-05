#pragma once

#include <QWidget>
#include <QItemSelection>
#include "src/dao/UsersDAO.h"
#include "src/models/UsersTableModel.h"
#include "src/models/UsersProxyModel.h"

namespace Ui {
class UsersPage;
}

class UsersPage : public QWidget
{
    Q_OBJECT

public:
    explicit UsersPage(UsersDAO& userDAO, QWidget *parent = nullptr);
    ~UsersPage();

private slots:
    void onSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
    void onNewUserClicked();
    void onEditUserClicked();
    void onDeleteUserClicked();
    void onRefreshClicked();

private:
    void setupUsersTable();
    void setupConnections();
    void reconnectSelectionModel();
    int getSelectedUserId() const;

private:
    Ui::UsersPage *ui;
    UsersDAO& m_usersDAO;
    UsersTableModel* m_model;
    UsersProxyModel* m_proxy;
};
