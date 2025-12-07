#include <QMessageBox>
#include "MetadataPage.h"
#include "ui_MetadataPage.h"
#include "src/dialogs/NewTypeDialog.h"
#include "src/dialogs/NewStateDialog.h"
#include "src/dialogs/NewLocationDialog.h"

MetadataPage::MetadataPage(ItemTypesDAO& itemTypesDAO,
                  ItemStatesDAO& itemStatesDAO,
                  LocationsDAO locationsDAO,
                  QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MetadataPage)
    , m_itemTypesDAO(itemTypesDAO)
    , m_itemStatesDAO(itemStatesDAO)
    , m_locationsDAO(locationsDAO)
{
    ui->setupUi(this);
    ui->editTypeButton->setEnabled(false);
    ui->deleteTypeButton->setEnabled(false);
    ui->editStateButton->setEnabled(false);
    ui->deleteStateButton->setEnabled(false);
    ui->editLocationButton->setEnabled(false);
    ui->deleteLocationButton->setEnabled(false);

    m_typesModel = new TypesTableModel(m_itemTypesDAO, this);
    m_typesProxy = new TypesProxyModel(this);
    m_statesModel = new StatesTableModel(m_itemStatesDAO, this);
    m_statesProxy = new StatesProxyModel(this);
    m_locationsModel = new LocationsTableModel(m_locationsDAO, this);
    m_locationsProxy = new LocationsProxyModel(this);

    m_typesProxy->setSourceModel(m_typesModel);
    m_statesProxy->setSourceModel(m_statesModel);
    m_locationsProxy->setSourceModel(m_locationsModel);

    setupTypesTable();
    setupStatesTable();
    setupLocationsTable();

    setupConnections();
}
MetadataPage::~MetadataPage()
{
    delete ui;
}

void MetadataPage::onTypesSelectionChanged(const QItemSelection& selected,
                                           const QItemSelection& deselected)
{
    Q_UNUSED(deselected);
    const bool hasSelection = !selected.indexes().isEmpty();
    ui->editTypeButton->setEnabled(hasSelection);
    ui->deleteTypeButton->setEnabled(hasSelection);
}

void MetadataPage::onNewTypeClicked()
{
    NewTypeDialog dialog(m_itemTypesDAO, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_typesModel->refresh();
        reconnectTypesSelectionModel();
        onTypesSelectionChanged({}, {});
    }
}

void MetadataPage::onEditTypeClicked()
{
    int typeId = getSelectedTypeId();
    if (typeId <= 0)
        return;

    NewTypeDialog dialog(m_itemTypesDAO, this);
    dialog.loadType(typeId);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_typesModel->refresh();
        reconnectTypesSelectionModel();
        onTypesSelectionChanged({}, {});
    }
}

void MetadataPage::onDeleteTypeClicked()
{
    int typeId = getSelectedTypeId();
    if (typeId <= 0)
        return;

    if (QMessageBox::question(this, "Eliminar tipo", "¿Está seguro que desea eliminar este tipo?") == QMessageBox::Yes)
    {
        m_itemTypesDAO.remove(typeId);
        m_typesModel->refresh();
        reconnectTypesSelectionModel();
        onTypesSelectionChanged({}, {});
    }
}

void MetadataPage::onRefreshTypesClicked()
{
    m_typesModel->refresh();
    ui->typesTableView->clearSelection();
    onTypesSelectionChanged({}, {});
}

void MetadataPage::onStatesSelectionChanged(const QItemSelection& selected,
                                            const QItemSelection& deselected)
{
    Q_UNUSED(deselected);
    const bool hasSelection = !selected.indexes().isEmpty();
    ui->editStateButton->setEnabled(hasSelection);
    ui->deleteStateButton->setEnabled(hasSelection);
}

void MetadataPage::onNewStateClicked()
{
    NewStateDialog dialog(m_itemStatesDAO, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_statesModel->refresh();
        reconnectStatesSelectionModel();
        onStatesSelectionChanged({}, {});
    }
}

void MetadataPage::onEditStateClicked()
{
    int stateId = getSelectedStateId();
    if (stateId <= 0)
        return;

    NewStateDialog dialog(m_itemStatesDAO, this);
    dialog.loadState(stateId);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_statesModel->refresh();
        reconnectStatesSelectionModel();
        onStatesSelectionChanged({}, {});
    }
}

void MetadataPage::onDeleteStateClicked()
{
    int stateId = getSelectedStateId();
    if (stateId <= 0)
        return;

    if (QMessageBox::question(this, "Eliminar estado", "¿Está seguroq ue desea eliminar este estado?") == QMessageBox::Yes)
    {
        m_itemStatesDAO.remove(stateId);
        m_statesModel->refresh();
        reconnectStatesSelectionModel();
        onStatesSelectionChanged({}, {});
    }
}

void MetadataPage::onRefreshStatesClicked()
{
    m_statesModel->refresh();
    ui->statesTableView->clearSelection();
    onStatesSelectionChanged({}, {});
}

void MetadataPage::onLocationsSelectionChanged(const QItemSelection& selected,
                                               const QItemSelection& deselected)
{
    Q_UNUSED(deselected);
    const bool hasSelection = !selected.indexes().isEmpty();
    ui->editLocationButton->setEnabled(hasSelection);
    ui->deleteLocationButton->setEnabled(hasSelection);
}

void MetadataPage::onNewLocationClicked()
{
    NewLocationDialog dialog(m_locationsDAO, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_locationsModel->refresh();
        reconnectLocationsSelectionModel();
        onLocationsSelectionChanged({}, {});
    }
}

void MetadataPage::onEditLocationClicked()
{
    int locationId = getSelectedLocationId();
    if (locationId <= 0)
        return;

    NewLocationDialog dialog(m_locationsDAO, this);
    dialog.loadLocation(locationId);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_locationsModel->refresh();
        reconnectLocationsSelectionModel();
        onLocationsSelectionChanged({}, {});
    }
}

void MetadataPage::onDeleteLocationClicked()
{
    int locationId = getSelectedLocationId();
    if (locationId <= 0)
        return;

    if (QMessageBox::question(this, "Eliminar ubicación", "¿Está seguro que desea eliminar esta ubicación?") == QMessageBox::Yes)
    {
        m_locationsDAO.remove(locationId);
        m_locationsModel->refresh();
        reconnectLocationsSelectionModel();
        onLocationsSelectionChanged({}, {});
    }
}

void MetadataPage::onRefreshLocationsClicked()
{
    m_locationsModel->refresh();
    ui->locationsTableView->clearSelection();
    onLocationsSelectionChanged({}, {});
}

void MetadataPage::setupTypesTable()
{
    ui->typesTableView->setModel(m_typesProxy);
    ui->typesTableView->setSortingEnabled(true);
    ui->typesTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->typesTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->typesTableView->verticalHeader()->setVisible(false);
    ui->typesTableView->horizontalHeader()->setStretchLastSection(true);
    ui->typesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->typesTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->typesTableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->typesTableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
}

void MetadataPage::setupStatesTable()
{
    ui->statesTableView->setModel(m_statesProxy);
    ui->statesTableView->setSortingEnabled(true);
    ui->statesTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->statesTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->statesTableView->verticalHeader()->setVisible(false);
    ui->statesTableView->horizontalHeader()->setStretchLastSection(true);
    ui->statesTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->statesTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->statesTableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->statesTableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
}

void MetadataPage::setupLocationsTable()
{
    ui->locationsTableView->setModel(m_locationsProxy);
    ui->locationsTableView->setSortingEnabled(true);
    ui->locationsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->locationsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->locationsTableView->verticalHeader()->setVisible(false);
    ui->locationsTableView->horizontalHeader()->setStretchLastSection(true);
    ui->locationsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->locationsTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->locationsTableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->locationsTableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
}

void MetadataPage::setupConnections()
{
    QItemSelectionModel* selectionTypesModel = ui->typesTableView->selectionModel();
    connect(selectionTypesModel, &QItemSelectionModel::selectionChanged,
            this, &MetadataPage::onTypesSelectionChanged);
    QItemSelectionModel* selectionStatesModel = ui->statesTableView->selectionModel();
    connect(selectionStatesModel, &QItemSelectionModel::selectionChanged,
            this, &MetadataPage::onStatesSelectionChanged);
    QItemSelectionModel* selectionLocationsModel = ui->locationsTableView->selectionModel();
    connect(selectionLocationsModel, &QItemSelectionModel::selectionChanged,
            this, &MetadataPage::onLocationsSelectionChanged);

    connect(ui->newTypeButton, &QPushButton::clicked, this, &MetadataPage::onNewTypeClicked);
    connect(ui->editTypeButton, &QPushButton::clicked, this, &MetadataPage::onEditTypeClicked);
    connect(ui->deleteTypeButton, &QPushButton::clicked, this, &MetadataPage::onDeleteTypeClicked);
    connect(ui->refreshTypesButton, &QPushButton::clicked, this, &MetadataPage::onRefreshTypesClicked);
    connect(ui->searchTypeLineEdit, &QLineEdit::textChanged, m_typesProxy, &TypesProxyModel::setFilterText);

    connect(ui->newStateButton, &QPushButton::clicked, this, &MetadataPage::onNewStateClicked);
    connect(ui->editStateButton, &QPushButton::clicked, this, &MetadataPage::onEditStateClicked);
    connect(ui->deleteStateButton, &QPushButton::clicked, this, &MetadataPage::onDeleteStateClicked);
    connect(ui->refreshStatesButton, &QPushButton::clicked, this, &MetadataPage::onRefreshStatesClicked);
    connect(ui->searchStateLineEdit, &QLineEdit::textChanged, m_statesProxy, &StatesProxyModel::setFilterText);

    connect(ui->newLocationButton, &QPushButton::clicked, this, &MetadataPage::onNewLocationClicked);
    connect(ui->editLocationButton, &QPushButton::clicked, this, &MetadataPage::onEditLocationClicked);
    connect(ui->deleteLocationButton, &QPushButton::clicked, this, &MetadataPage::onDeleteLocationClicked);
    connect(ui->refreshLocationsButton, &QPushButton::clicked, this, &MetadataPage::onRefreshLocationsClicked);
    connect(ui->searchLocationLineEdit, &QLineEdit::textChanged, m_locationsProxy, &LocationsProxyModel::setFilterText);
}

void MetadataPage::reconnectTypesSelectionModel()
{
    QItemSelectionModel* sel = ui->typesTableView->selectionModel();
    connect(sel, &QItemSelectionModel::selectionChanged,
            this, &MetadataPage::onTypesSelectionChanged);
}

void MetadataPage::reconnectStatesSelectionModel()
{
    QItemSelectionModel* sel = ui->statesTableView->selectionModel();
    connect(sel, &QItemSelectionModel::selectionChanged,
            this, &MetadataPage::onStatesSelectionChanged);
}

void MetadataPage::reconnectLocationsSelectionModel()
{
    QItemSelectionModel* sel = ui->locationsTableView->selectionModel();
    connect(sel, &QItemSelectionModel::selectionChanged,
            this, &MetadataPage::onLocationsSelectionChanged);
}

int MetadataPage::getSelectedTypeId() const
{
    QModelIndex proxyIndex = ui->typesTableView->currentIndex();
    if (!proxyIndex.isValid())
        return -1;

    QModelIndex sourceIndex = m_typesProxy->mapToSource(proxyIndex);
    return sourceIndex.data(Qt::UserRole).toInt();
}

int MetadataPage::getSelectedStateId() const
{
    QModelIndex proxyIndex = ui->statesTableView->currentIndex();
    if (!proxyIndex.isValid())
        return -1;

    QModelIndex sourceIndex = m_statesProxy->mapToSource(proxyIndex);
    return sourceIndex.data(Qt::UserRole).toInt();
}

int MetadataPage::getSelectedLocationId() const
{
    QModelIndex proxyIndex = ui->locationsTableView->currentIndex();
    if (!proxyIndex.isValid())
        return -1;

    QModelIndex sourceIndex = m_locationsProxy->mapToSource(proxyIndex);
    return sourceIndex.data(Qt::UserRole).toInt();
}
