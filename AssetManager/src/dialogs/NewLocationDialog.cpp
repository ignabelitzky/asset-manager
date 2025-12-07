#include <QMessageBox>
#include <QRegularExpression>
#include "NewLocationDialog.h"
#include "ui_NewLocationDialog.h"

NewLocationDialog::NewLocationDialog(LocationsDAO& locationsDAO,
                                     QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewLocationDialog)
    , m_locationsDAO(locationsDAO)
{
    ui->setupUi(this);
    connect(ui->saveButton, &QPushButton::clicked, this, &NewLocationDialog::onSave);
    connect(ui->cancelButton, &QPushButton::clicked, this, &NewLocationDialog::reject);
}

NewLocationDialog::~NewLocationDialog()
{
    delete ui;
}

void NewLocationDialog::loadLocation(int locationId)
{
    m_locationId = locationId;

    Location location = m_locationsDAO.getById(m_locationId);
    if (location.name.isEmpty())
        return;

    ui->locationLineEdit->setText(location.name);
    setWindowTitle("Editar Ubicación");
}

void NewLocationDialog::onSave()
{
    QString errorMessage;
    if (!validateInput(errorMessage))
    {
        QMessageBox::warning(this, "Error de validación", errorMessage);
        return;
    }

    Location location;
    location.name = ui->locationLineEdit->text();

    bool success = false;
    if (m_locationId <= 0)
        success = m_locationsDAO.insert(location.name);
    else
        success = m_locationsDAO.update(m_locationId, location.name);

    if (!success)
    {
        QMessageBox::critical(this, "Error", "No se puede guardar la ubicación.");
        return;
    }

    accept();
}

bool NewLocationDialog::validateInput(QString& errorMessage) const
{
    const QString name = ui->locationLineEdit->text().trimmed();

    if (name.isEmpty())
    {
        errorMessage = "La ubicación no puede estar vacía";
        return false;
    }

    if (name.length() > 50)
    {
        errorMessage = "La ubicación no puede superar los 50 caracteres";
        return false;
    }

    if (!QRegularExpression("^[A-Z-a-z0-9ÁÉÍÓÚáéíóúÑñ ]+$").match(name).hasMatch())
    {
        errorMessage = "La ubicación contiene caracteres no permitidos";
        return false;
    }

    return true;
}
