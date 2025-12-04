#include <QMessageBox>
#include <QPushButton>
#include "NewUserDialog.h"
#include "ui_NewUserDialog.h"

NewUserDialog::NewUserDialog(UserDAO& userDAO, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewUserDialog)
    , m_userDAO(userDAO)
{
    ui->setupUi(this);
    connect(ui->saveButton, &QPushButton::clicked,
            this, &NewUserDialog::onSave);
    connect(ui->cancelButton, &QPushButton::clicked,
            this, &NewUserDialog::reject);
}

NewUserDialog::~NewUserDialog()
{
    delete ui;
}

void NewUserDialog::loadUser(int userId)
{
    m_userId = userId;

    std::optional<User> user = m_userDAO.getUserById(m_userId);
    if (!user.has_value())
        return;

    ui->firstNameLineEdit->setText(user->firstName());
    ui->lastNameLineEdit->setText(user->lastName());
    ui->barcodeLineEdit->setText(user->barcode());

    setWindowTitle("Editar Usuario");
}

void NewUserDialog::onSave()
{
    User user;
    user.setId(m_userId);
    user.setFirstName(ui->firstNameLineEdit->text());
    user.setLastName(ui->lastNameLineEdit->text());
    user.setBarcode(ui->barcodeLineEdit->text());

    // Validate
    if (user.firstName().isEmpty())
    {
        QMessageBox::warning(this, "Error de validación", "El nombre no puede estar vacío.");
        return;
    }
    if (user.lastName().isEmpty())
    {
        QMessageBox::warning(this, "Error de validación", "El apellido no puede estar vacío.");
        return;
    }
    if (user.barcode().isEmpty())
    {
        QMessageBox::warning(this, "Error de validación", "El código identificatorio no puede estar vacío.");
        return;
    }

    bool success = false;
    if (m_userId < 0)
        success = m_userDAO.addUser(user);
    else
        success = m_userDAO.updateUser(user);

    if (!success)
    {
        QMessageBox::critical(this, "Error", "No se puede guardar.");
        return;
    }

    accept();
}
