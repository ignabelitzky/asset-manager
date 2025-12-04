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

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &NewUserDialog::onSave);
}

NewUserDialog::~NewUserDialog()
{
    delete ui;
}

void NewUserDialog::onSave()
{
    User user;
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

    const bool success = m_userDAO.addUser(user);

    if (!success)
    {
        QMessageBox::critical(this, "Error", "No se pudo guardar al usuario en la base de datos.");
        return;
    }

    accept();
}
