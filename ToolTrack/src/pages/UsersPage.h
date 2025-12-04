#pragma once

#include <QWidget>
#include <QItemSelection>
#include "src/dao/UserDAO.h"
#include "src/models/UserTableModel.h"
#include "src/models/UserProxyModel.h"

namespace Ui {
class UsersPage;
}

class UsersPage : public QWidget
{
    Q_OBJECT

public:
    explicit UsersPage(UserDAO& userDAO, QWidget *parent = nullptr);
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
    UserDAO& m_userDAO;
    UserTableModel* m_model;
    UserProxyModel* m_proxy;
};
