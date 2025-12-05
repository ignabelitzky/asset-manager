#pragma once

#include <QDialog>
#include <optional>
#include "src/dao/UsersDAO.h"
#include "src/models/User.h"

namespace Ui {
class NewUserDialog;
}

class NewUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewUserDialog(UsersDAO& usersDAO, QWidget *parent = nullptr);
    ~NewUserDialog();

    void loadUser(int userId);

private slots:
    void onSave();

private:
    bool validateInput(QString& errorMessage) const;

private:
    Ui::NewUserDialog *ui;
    UsersDAO& m_usersDAO;
    int m_userId{-1};
};
