#include <QMessageBox>
#include "ItemsPage.h"
#include "ui_ItemsPage.h"
#include "src/models/Item.h"
#include "src/dialogs/NewItemDialog.h"

ItemsPage::ItemsPage(ItemsDAO& itemsDAO,
                     ItemTypesDAO& itemTypesDAO,
                     ItemStatesDAO& itemStatesDAO,
                     OwnerTypesDAO& ownerTypesDAO,
                     PersonsDAO& personsDAO,
                     InstitutionsDAO& institutionsDAO,
                     LocationsDAO& locationsDAO,
                     QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ItemsPage)
    , m_itemsDAO(itemsDAO)
    , m_itemTypesDAO(itemTypesDAO)
    , m_itemStatesDAO(itemStatesDAO)
    , m_ownerTypesDAO(ownerTypesDAO)
    , m_personsDAO(personsDAO)
    , m_institutionsDAO(institutionsDAO)
    , m_locationsDAO(locationsDAO)
    , m_model(new ItemsTableModel(m_itemsDAO, m_itemTypesDAO, m_itemStatesDAO,
                                  m_ownerTypesDAO, m_personsDAO, m_institutionsDAO,
                                  m_locationsDAO, this))
    , m_proxy(new ItemsProxyModel(this))
{
    ui->setupUi(this);
    ui->editItemButton->setEnabled(false);
    ui->deleteItemButton->setEnabled(false);

    m_proxy->setSourceModel(m_model);

    setupItemsTable();
    setupConnections();
}

ItemsPage::~ItemsPage()
{
    delete ui;
}

void ItemsPage::onSelectionChanged(const QItemSelection& selected,
                                   const QItemSelection& deselected)
{
    Q_UNUSED(deselected);
    const bool hasSelection = !selected.indexes().isEmpty();
    ui->editItemButton->setEnabled(hasSelection);
    ui->deleteItemButton->setEnabled(hasSelection);
}

void ItemsPage::onNewItemClicked()
{
    NewItemDialog dlg(
        m_itemsDAO,
        m_itemTypesDAO,
        m_itemStatesDAO,
        m_ownerTypesDAO,
        m_personsDAO,
        m_institutionsDAO,
        m_locationsDAO,
        this
        );

    if (dlg.exec() == QDialog::Accepted)
    {
        Item newItem = dlg.getItem();

        if (!newItem.isValid())
        {
            QMessageBox::warning(this, "Error", "El item ingresado no es válido.");
            return;
        }

        if (!m_itemsDAO.insert(newItem))
        {
            QMessageBox::critical(this, "Error", "No se pudo guardar el nuevo item en la base de datos.");
            return;
        }

        m_model->refresh();
        reconnectSelectionModel();
        ui->tableView->clearSelection();
        onSelectionChanged({}, {});
    }
}


void ItemsPage::onEditItemClicked()
{
    int itemId = getSelectedItemId();
    if (itemId < 0)
    {
        QMessageBox::warning(this, "Error", "No se ha seleccionado ningún item.");
        return;
    }

    std::optional<Item> itemOpt = m_itemsDAO.getById(itemId);
    if (!itemOpt.has_value())
    {
        QMessageBox::warning(this, "Error", "No se pudo cargar el item seleccionado.");
        return;
    }

    const Item& item = *itemOpt;

    if (!item.isValid())
    {
        QMessageBox::warning(this, "Error", "El item seleccionado contiene datos inválidos.");
        return;
    }

    NewItemDialog dlg(
        m_itemsDAO,
        m_itemTypesDAO,
        m_itemStatesDAO,
        m_ownerTypesDAO,
        m_personsDAO,
        m_institutionsDAO,
        m_locationsDAO,
        this
        );

    dlg.setItemToEdit(item);

    // Show the dialog and wait for the user action
    if (dlg.exec() == QDialog::Accepted)
    {
        Item updatedItem = dlg.getItem();
        updatedItem.setId(itemId); // Asegurarnos de mantener el mismo ID

        // Save changes in the database
        if (!m_itemsDAO.update(updatedItem))
        {
            QMessageBox::critical(this, "Error", "No se pudo actualizar el item en la base de datos.");
            return;
        }

        m_model->refresh();
        reconnectSelectionModel();
        ui->tableView->clearSelection();
        onSelectionChanged({}, {});
    }
}


void ItemsPage::onDeleteItemClicked()
{
    int itemId = getSelectedItemId();
    if (itemId < 0)
    {
        QMessageBox::warning(this, "Error", "No se ha seleccionado ningún item para eliminar.");
        return;
    }

    if (QMessageBox::question(
            this,
            "Eliminar item",
            "¿Está seguro que desea eliminar este item?",
            QMessageBox::Yes | QMessageBox::No
            ) != QMessageBox::Yes)
    {
        return;
    }

    if (!m_itemsDAO.remove(itemId))
    {
        QMessageBox::critical(this, "Error", "No se pudo eliminar el item de la base de datos.");
        return;
    }

    m_model->refresh();
    reconnectSelectionModel();
    ui->tableView->clearSelection();
    onSelectionChanged({}, {});
}


void ItemsPage::onRefreshClicked()
{
    m_model->refresh();
    ui->tableView->clearSelection();
    onSelectionChanged({}, {});
}

void ItemsPage::setupItemsTable()
{
    ui->tableView->setModel(m_proxy);
    ui->tableView->setSortingEnabled(true);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->tableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    reconnectSelectionModel();
}

void ItemsPage::setupConnections()
{
    QItemSelectionModel* selectionModel = ui->tableView->selectionModel();
    connect(selectionModel, &QItemSelectionModel::selectionChanged,
            this, &ItemsPage::onSelectionChanged);
    connect(ui->newItemButton, &QPushButton::clicked, this, &ItemsPage::onNewItemClicked);
    connect(ui->editItemButton, &QPushButton::clicked, this, &ItemsPage::onEditItemClicked);
    connect(ui->deleteItemButton, &QPushButton::clicked, this, &ItemsPage::onDeleteItemClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &ItemsPage::onRefreshClicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, m_proxy, &ItemsProxyModel::setSearchText);
}

void ItemsPage::reconnectSelectionModel()
{
    QItemSelectionModel* sel = ui->tableView->selectionModel();
    connect(sel, &QItemSelectionModel::selectionChanged,
            this, &ItemsPage::onSelectionChanged);
}

int ItemsPage::getSelectedItemId() const
{
    QModelIndex proxyIndex = ui->tableView->currentIndex();
    if (!proxyIndex.isValid())
        return -1;

    QModelIndex sourceIndex = m_proxy->mapToSource(proxyIndex);
    return sourceIndex.data(Qt::UserRole).toInt();
}
