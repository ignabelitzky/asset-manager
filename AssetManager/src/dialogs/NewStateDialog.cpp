#include <QMessageBox>
#include <QRegularExpression>
#include "NewStateDialog.h"
#include "ui_NewStateDialog.h"

NewStateDialog::NewStateDialog(ItemStatesDAO& itemStatesDAO,
                               QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NewStateDialog)
    , m_itemStatesDAO(itemStatesDAO)
{
    ui->setupUi(this);
    connect(ui->saveButton, &QPushButton::clicked, this, &NewStateDialog::onSave);
    connect(ui->cancelButton, &QPushButton::clicked, this, &NewStateDialog::reject);

    setWindowTitle("Nuevo Estado");
}

NewStateDialog::~NewStateDialog()
{
    delete ui;
}

void NewStateDialog::loadState(int stateId)
{
    m_stateId = stateId;

    ItemState state = m_itemStatesDAO.getById(m_stateId);
    if (state.name.isEmpty())
        return;

    ui->stateLineEdit->setText(state.name);
    setWindowTitle("Editar Estado");
}

void NewStateDialog::onSave()
{
    QString errorMessage;
    if (!validateInput(errorMessage))
    {
        QMessageBox::warning(this, "Error de validación", errorMessage);
        return;
    }

    ItemState state;
    state.name = ui->stateLineEdit->text();

    bool success = false;
    if (m_stateId <= 0)
        success = m_itemStatesDAO.insert(state.name);
    else
        success = m_itemStatesDAO.update(m_stateId, state.name);

    if (!success)
    {
        QMessageBox::critical(this, "Error", "Nose puede guardar el estado.");
        return;
    }

    accept();
}

bool NewStateDialog::validateInput(QString& errorMessage) const
{
    const QString name = ui->stateLineEdit->text().trimmed();

    if (name.isEmpty())
    {
        errorMessage = "El estado no puede estar vacío";
        return false;
    }

    if (name.length() > 50)
    {
        errorMessage = "El estado no puede superar los 50 caracteres";
        return false;
    }

    if (!QRegularExpression("^[A-Z-a-z0-9ÁÉÍÓÚáéíóúÑñ ]+$").match(name).hasMatch())
    {
        errorMessage = "El estado contiene caracteres no permitidos";
        return false;
    }

    return true;
}
