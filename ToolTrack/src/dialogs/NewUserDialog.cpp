#include <QMessageBox>
#include <QRegularExpression>
#include <QPushButton>

#include "NewUserDialog.h"
#include "ui_NewUserDialog.h"

NewUserDialog::NewUserDialog(UsersDAO& usersDAO, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewUserDialog)
    , m_usersDAO(usersDAO)
{
    ui->setupUi(this);
    connect(ui->saveButton, &QPushButton::clicked,
            this, &NewUserDialog::onSave);
    connect(ui->cancelButton, &QPushButton::clicked,
            this, &NewUserDialog::reject);

    setWindowTitle("Nuevo Usuario");
}

NewUserDialog::~NewUserDialog()
{
    delete ui;
}

void NewUserDialog::loadUser(int userId)
{
    m_userId = userId;

    std::optional<User> user = m_usersDAO.getUserById(m_userId);
    if (!user.has_value())
        return;

    ui->firstNameLineEdit->setText(user->firstName());
    ui->lastNameLineEdit->setText(user->lastName());
    ui->barcodeLineEdit->setText(user->barcode());

    setWindowTitle("Editar Usuario");
}

void NewUserDialog::onSave()
{
    QString errorMessage;
    if (!validateInput(errorMessage))
    {
        QMessageBox::warning(this, "Error de validación", errorMessage);
        return;
    }

    User user;
    user.setId(m_userId);
    user.setFirstName(ui->firstNameLineEdit->text());
    user.setLastName(ui->lastNameLineEdit->text());
    user.setBarcode(ui->barcodeLineEdit->text());

    bool success = false;
    if (m_userId < 0)
        success = m_usersDAO.insert(user);
    else
        success = m_usersDAO.update(user);

    if (!success)
    {
        QMessageBox::critical(this, "Error", "No se puede guardar el usuario.");
        return;
    }

    accept();
}

bool NewUserDialog::validateInput(QString& errorMessage) const
{
    const QString firstName = ui->firstNameLineEdit->text().trimmed();
    const QString lastName = ui->lastNameLineEdit->text().trimmed();
    const QString barcode = ui->barcodeLineEdit->text().trimmed();

    if (firstName.isEmpty())
    {
        errorMessage = "El nombre no puede estar vacío";
        return false;
    }

    if (firstName.length() > 50)
    {
        errorMessage = "El nombre no puede superar los 50 caracteres";
        return false;
    }

    if (!QRegularExpression("^[A-Za-zÁÉÍÓÚáéíóúÑñ ]+$").match(firstName).hasMatch())
    {
        errorMessage = "El nombre contiene caracteres no permitidos";
        return false;
    }

    if (lastName.isEmpty())
    {
        errorMessage = "El apellido no puede estar vacío";
        return false;
    }

    if (lastName.length() > 50)
    {
        errorMessage = "El apellido no puede superar los 50 caracteres";
        return false;
    }

    if (!QRegularExpression("^[A-Za-zÁÉÍÓÚáéíóúÑñ ]+$").match(lastName).hasMatch())
    {
        errorMessage = "El apellido contiene caracteres no permitidos";
        return false;
    }

    if (barcode.isEmpty()) {
        errorMessage = "El código no puede estar vacío.";
        return false;
    }
    if (!QRegularExpression("^[0-9]+$").match(barcode).hasMatch()) {
        errorMessage = "El código debe contener solo dígitos.";
        return false;
    }
    if (barcode.length() < 4 || barcode.length() > 32) {
        errorMessage = "El código debe tener entre 4 y 32 dígitos.";
        return false;
    }

    if (m_usersDAO.existsBarcode(barcode, m_userId))
    {
        errorMessage = "El código ya está registrado a otro usuario.";
        return false;
    }

    return true;
}
