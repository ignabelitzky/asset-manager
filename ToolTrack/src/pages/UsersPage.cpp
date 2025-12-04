#include "UsersPage.h"
#include "ui_UsersPage.h"
#include "src/dialogs/NewUserDialog.h"

UsersPage::UsersPage(UserDAO& userDAO, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UsersPage)
    , m_userDAO(userDAO)
{
    ui->setupUi(this);

    setupConnections();
}

UsersPage::~UsersPage()
{
    delete ui;
}

void UsersPage::onNewUserClicked()
{
    NewUserDialog dialog(m_userDAO, this);
    if (dialog.exec() == QDialog::Accepted)
        qDebug() << "User added";
}

void UsersPage::setupConnections()
{
    connect(ui->newUserButton, &QPushButton::clicked, this, &UsersPage::onNewUserClicked);
}
