#include <QRegularExpression>
#include <QMessageBox>
#include "NewTypeDialog.h"
#include "ui_NewTypeDialog.h"

NewTypeDialog::NewTypeDialog(ItemTypesDAO& itemTypesDAO,
                             QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewTypeDialog)
    , m_itemTypesDAO(itemTypesDAO)
{
    ui->setupUi(this);
    connect(ui->saveButton, &QPushButton::clicked, this, &NewTypeDialog::onSave);
    connect(ui->cancelButton, &QPushButton::clicked, this, &NewTypeDialog::reject);

    setWindowTitle("Nuevo Tipo");
}

NewTypeDialog::~NewTypeDialog()
{
    delete ui;
}

void NewTypeDialog::loadType(int typeId)
{
    m_typeId = typeId;

    ItemType type = m_itemTypesDAO.getById(m_typeId);
    if (type.name.isEmpty())
        return;

    ui->typeLineEdit->setText(type.name);
    setWindowTitle("Editar Tipo");
}

void NewTypeDialog::onSave()
{
    QString errorMessage;
    if (!validateInput(errorMessage))
    {
        QMessageBox::warning(this, "Error de validación", errorMessage);
        return;
    }

    ItemType type;
    type.name = ui->typeLineEdit->text();

    bool success = false;
    if (m_typeId <= 0)
        success = m_itemTypesDAO.insert(type.name);
    else
        success = m_itemTypesDAO.update(m_typeId, type.name);

    if (!success)
    {
        QMessageBox::critical(this, "Error", "No se puede guardar el tipo.");
        return;
    }

    accept();
}

bool NewTypeDialog::validateInput(QString& errorMessage) const
{
    const QString name = ui->typeLineEdit->text().trimmed();

    if (name.isEmpty())
    {
        errorMessage = "El tipo no puede estar vacío";
        return false;
    }

    if (name.length() > 50)
    {
        errorMessage = "El tipo no puede superar los 50 caracteres";
        return false;
    }

    if (!QRegularExpression("^[A-Z-a-z0-9ÁÉÍÓÚáéíóúÑñ ]+$").match(name).hasMatch())
    {
        errorMessage = "El tipo contiene caracteres no permitidos";
        return false;
    }

    return true;
}
