#include "NewItemDialog.h"
#include "ui_NewItemDialog.h"

#include <QMessageBox>

NewItemDialog::NewItemDialog(ItemsDAO& itemsDAO,
                             ItemTypesDAO& itemTypesDAO,
                             ItemStatesDAO& itemStatesDAO,
                             OwnerTypesDAO& ownerTypesDAO,
                             PersonsDAO& personsDAO,
                             InstitutionsDAO& institutionsDAO,
                             LocationsDAO& locationsDAO,
                             QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::NewItemDialog)
    , m_itemsDAO(itemsDAO)
    , m_itemTypesDAO(itemTypesDAO)
    , m_itemStatesDAO(itemStatesDAO)
    , m_ownerTypesDAO(ownerTypesDAO)
    , m_personsDAO(personsDAO)
    , m_institutionsDAO(institutionsDAO)
    , m_locationsDAO(locationsDAO)
{
    ui->setupUi(this);

    loadCombos();

    connect(ui->ownerTypeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NewItemDialog::onOwnerTypeChanged);

    connect(ui->saveButton, &QPushButton::clicked,
            this, &NewItemDialog::onSaveClicked);

    connect(ui->cancelButton, &QPushButton::clicked,
            this, &NewItemDialog::onCancelClicked);
}

NewItemDialog::~NewItemDialog()
{
    delete ui;
}

void NewItemDialog::setItemToEdit(const Item& item)
{
    m_isEditMode = true;
    m_originalItem = item;

    setWindowTitle("Editar Item");

    ui->nameLineEdit->setText(item.name());
    ui->barcodeLineEdit->setText(item.barcode());
    ui->stockSpinBox->setValue(item.stock());
    ui->brandLineEdit->setText(item.brand());
    ui->modelLineEdit->setText(item.model());
    ui->notesTextEdit->setPlainText(item.notes());

    // Type
    ui->typeComboBox->setCurrentIndex(ui->typeComboBox->findData(item.typeId()));

    // State
    ui->stateComboBox->setCurrentIndex(ui->stateComboBox->findData(item.stateId()));

    // Owner types
    ui->ownerTypeComboBox->setCurrentIndex(ui->ownerTypeComboBox->findData(item.ownerTypeId()));
    loadOwnersForType(item.ownerTypeId());

    ui->ownerComboBox->setCurrentIndex(ui->ownerComboBox->findData(item.ownerId()));

    // Location
    ui->locationComboBox->setCurrentIndex(ui->locationComboBox->findData(item.locationId()));
}

void NewItemDialog::loadCombos()
{
    ui->typeComboBox->clear();
    for (auto t : m_itemTypesDAO.getAll())
        ui->typeComboBox->addItem(t.name, t.id);

    ui->stateComboBox->clear();
    for (auto s : m_itemStatesDAO.getAll())
        ui->stateComboBox->addItem(s.name, s.id);

    ui->ownerTypeComboBox->clear();
    for (auto o : m_ownerTypesDAO.getAll())
        ui->ownerTypeComboBox->addItem(o.name, o.id);

    ui->locationComboBox->clear();
    for (auto l : m_locationsDAO.getAll())
        ui->locationComboBox->addItem(l.name, l.id);

    if (ui->ownerTypeComboBox->count() > 0)
    {
        ui->ownerTypeComboBox->setCurrentIndex(0);

        int ownerTypeId = ui->ownerTypeComboBox->currentData().toInt();

        loadOwnersForType(ownerTypeId);
    }
}


void NewItemDialog::loadOwnersForType(int ownerTypeId)
{
    ui->ownerComboBox->clear();

    if (ownerTypeId == 1)  // Persona
    {
        for (auto p : m_personsDAO.getAll())
            ui->ownerComboBox->addItem(QString("%1 %2").arg(p.firstName, p.lastName).trimmed(), p.id);
    }
    else if (ownerTypeId == 2) // Institución
    {
        for (auto inst : m_institutionsDAO.getAll())
            ui->ownerComboBox->addItem(inst.name, inst.id);
    }
}

void NewItemDialog::onOwnerTypeChanged(int index)
{
    if (index < 0)
        return;
    int ownerTypeId = ui->ownerTypeComboBox->itemData(index).toInt();
    loadOwnersForType(ownerTypeId);
}


bool NewItemDialog::validateFields()
{
    QString name = ui->nameLineEdit->text().trimmed();
    QString barcode = ui->barcodeLineEdit->text().trimmed();

    if (name.isEmpty())
    {
        QMessageBox::warning(this, "Error", "El nombre no puede estar vacío.");
        return false;
    }

    if (barcode.isEmpty())
    {
        QMessageBox::warning(this, "Error", "El código no puede estar vacío.");
        return false;
    }

    // Barcode UNICO
    {
        auto existing = m_itemsDAO.getByBarcode(barcode);

        if (existing.has_value())
        {
            // Si estamos editando, permitir el mismo barcode solo si es del mismo item.
            if (!m_isEditMode || existing->id() != m_originalItem.id())
            {
                QMessageBox::warning(this, "Error",
                                     "El código ya está en uso por otro item.");
                return false;
            }
        }
    }

    int stock = ui->stockSpinBox->value();
    if (stock < 0)
    {
        QMessageBox::warning(this, "Error", "El stock no puede ser negativo.");
        return false;
    }

    int ownerTypeId = ui->ownerTypeComboBox->currentData().toInt();
    int ownerId     = ui->ownerComboBox->currentData().toInt();

    // Owner obligatorio si ownerType tiene valor
    if (ownerTypeId > 0 && ownerId <= 0)
    {
        QMessageBox::warning(this, "Error",
                             "Debe seleccionar un responsable (Persona o Institución).");
        return false;
    }

    // Location obligatoria
    int locationId = ui->locationComboBox->currentData().toInt();

    if (locationId <= 0)
    {
        QMessageBox::warning(this, "Error",
                             "Debe seleccionar una ubicación.");
        return false;
    }

    return true;
}

void NewItemDialog::onSaveClicked()
{
    if (!validateFields())
        return;

    Item item;
    if (m_isEditMode)
        item.setId(m_originalItem.id());

    item.setName(ui->nameLineEdit->text().trimmed());
    item.setBarcode(ui->barcodeLineEdit->text().trimmed());
    item.setStock(ui->stockSpinBox->value());
    item.setBrand(ui->brandLineEdit->text().trimmed());
    item.setModel(ui->modelLineEdit->text().trimmed());
    item.setNotes(ui->notesTextEdit->toPlainText().trimmed());

    item.setTypeId(ui->typeComboBox->currentData().toInt());
    item.setStateId(ui->stateComboBox->currentData().toInt());
    item.setLocationId(ui->locationComboBox->currentData().toInt());
    item.setOwner(ui->ownerTypeComboBox->currentData().toInt(),
                  ui->ownerComboBox->currentData().toInt());

    m_originalItem = item;
    accept();
}

void NewItemDialog::onCancelClicked()
{
    reject();
}

Item NewItemDialog::getItem() const
{
    return m_originalItem;
}
