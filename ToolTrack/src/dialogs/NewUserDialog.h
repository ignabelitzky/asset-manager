#pragma once

#include <QDialog>
#include "src/dao/UserDAO.h"

namespace Ui {
class NewUserDialog;
}

class NewUserDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewUserDialog(UserDAO& userDAO, QWidget *parent = nullptr);
    ~NewUserDialog();

private slots:
    void onSave();

private:
    Ui::NewUserDialog *ui;
    UserDAO& m_userDAO;
};
