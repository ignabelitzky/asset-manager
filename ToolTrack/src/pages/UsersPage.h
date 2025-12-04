#pragma once

#include <QWidget>
#include "src/dao/UserDAO.h"

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
    void onNewUserClicked();

private:
    void setupConnections();

private:
    Ui::UsersPage *ui;
    UserDAO& m_userDAO;
};
