#include <QMessageBox>
#include "OwnersPage.h"
#include "ui_OwnersPage.h"
#include "src/dialogs/NewPersonDialog.h"
#include "src/dialogs/NewInstitutionDialog.h"

OwnersPage::OwnersPage(PersonsDAO& personsDAO,
                       InstitutionsDAO& institutionsDAO,
                       QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::OwnersPage)
    , m_personsDAO(personsDAO)
    , m_institutionsDAO(institutionsDAO)
{
    ui->setupUi(this);

    ui->editPersonButton->setEnabled(false);
    ui->deletePersonButton->setEnabled(false);
    ui->editInstitutionButton->setEnabled(false);
    ui->deleteInstitutionButton->setEnabled(false);

    m_personsModel = new PersonsTableModel(m_personsDAO, this);
    m_personsProxy = new PersonsProxyModel(this);
    m_institutionsModel = new InstitutionsTableModel(m_institutionsDAO, this);
    m_institutionsProxy = new InstitutionsProxyModel(this);

    m_personsProxy->setSourceModel(m_personsModel);
    m_institutionsProxy->setSourceModel(m_institutionsModel);

    setupPersonsTable();
    setupInstitutionsTable();

    setupConnections();
}

OwnersPage::~OwnersPage()
{
    delete ui;
}

void OwnersPage::onPersonsSelectionChanged(const QItemSelection& selected,
                                           const QItemSelection& deselected)
{
    Q_UNUSED(deselected);
    const bool hasSelection = !selected.indexes().isEmpty();
    ui->editPersonButton->setEnabled(hasSelection);
    ui->deletePersonButton->setEnabled(hasSelection);
}

void OwnersPage::onNewPersonClicked()
{
    NewPersonDialog dialog(m_personsDAO, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_personsModel->refresh();
        reconnectPersonsSelectionModel();
        onPersonsSelectionChanged({},{});
    }
}

void OwnersPage::onEditPersonClicked()
{
    int personId = getSelectedPersonId();
    if (personId <= 0)
        return;

    NewPersonDialog dialog(m_personsDAO, this);
    dialog.loadPerson(personId);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_personsModel->refresh();
        reconnectPersonsSelectionModel();
        onPersonsSelectionChanged({}, {});
    }
}

void OwnersPage::onDeletePersonClicked()
{
    int personId = getSelectedPersonId();
    if (personId <= 0)
        return;

    if (QMessageBox::question(this, "Eliminar persona", "¿Está seguro que desea eliminar esta persona?") == QMessageBox::Yes)
    {
        m_personsDAO.remove(personId);
        m_personsModel->refresh();
        reconnectPersonsSelectionModel();
        onPersonsSelectionChanged({}, {});
    }
}

void OwnersPage::onRefreshPersonsClicked()
{
    m_personsModel->refresh();
    ui->personsTableView->clearSelection();
    onPersonsSelectionChanged({}, {});
}

void OwnersPage::onInstitutionsSelectionChanged(const QItemSelection& selected,
                                                const QItemSelection& deselected)
{
    Q_UNUSED(deselected);
    const bool hasSelection = !selected.indexes().isEmpty();
    ui->editInstitutionButton->setEnabled(hasSelection);
    ui->deleteInstitutionButton->setEnabled(hasSelection);
}

void OwnersPage::onNewInstitutionClicked()
{
    NewInstitutionDialog dialog(m_institutionsDAO, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_institutionsModel->refresh();
        reconnectInstitutionsSelectionModel();
        onInstitutionsSelectionChanged({}, {});
    }
}

void OwnersPage::onEditInstitutionClicked()
{
    int institutionId = getSelectedInstitutionId();
    if (institutionId <= 0)
        return;

    NewInstitutionDialog dialog(m_institutionsDAO, this);
    dialog.loadInstitution(institutionId);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_institutionsModel->refresh();
        reconnectInstitutionsSelectionModel();
        onInstitutionsSelectionChanged({}, {});
    }
}

void OwnersPage::onDeleteInstitutionClicked()
{
    int institutionId = getSelectedInstitutionId();
    if (institutionId <= 0)
        return;

    if (QMessageBox::question(this, "Eliminar institución", "¿Está seguro que desea eliminar esta institución?") == QMessageBox::Yes)
    {
        m_institutionsDAO.remove(institutionId);
        m_institutionsModel->refresh();
        reconnectInstitutionsSelectionModel();
        onInstitutionsSelectionChanged({}, {});
    }
}

void OwnersPage::onRefreshInstitutionsClicked()
{
    m_institutionsModel->refresh();
    ui->institutionsTableView->clearSelection();
    onInstitutionsSelectionChanged({}, {});
}

void OwnersPage::setupPersonsTable()
{
    ui->personsTableView->setModel(m_personsProxy);
    ui->personsTableView->setSortingEnabled(true);
    ui->personsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->personsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->personsTableView->verticalHeader()->setVisible(false);
    ui->personsTableView->horizontalHeader()->setStretchLastSection(true);
    ui->personsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->personsTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->personsTableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->personsTableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    reconnectPersonsSelectionModel();
}

void OwnersPage::setupInstitutionsTable()
{
    ui->institutionsTableView->setModel(m_institutionsProxy);
    ui->institutionsTableView->setSortingEnabled(true);
    ui->institutionsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->institutionsTableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->institutionsTableView->verticalHeader()->setVisible(false);
    ui->institutionsTableView->horizontalHeader()->setStretchLastSection(true);
    ui->institutionsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->institutionsTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->institutionsTableView->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->institutionsTableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);

    reconnectInstitutionsSelectionModel();
}

void OwnersPage::setupConnections()
{
    QItemSelectionModel* selectionPersonsModel = ui->personsTableView->selectionModel();
    connect(selectionPersonsModel, &QItemSelectionModel::selectionChanged,
            this, &OwnersPage::onPersonsSelectionChanged);
    QItemSelectionModel* selectionInstitutionsModel = ui->institutionsTableView->selectionModel();
    connect(selectionInstitutionsModel, &QItemSelectionModel::selectionChanged,
            this, &OwnersPage::onInstitutionsSelectionChanged);

    connect(ui->newPersonButton, &QPushButton::clicked, this, &OwnersPage::onNewPersonClicked);
    connect(ui->editPersonButton, &QPushButton::clicked, this, &OwnersPage::onEditPersonClicked);
    connect(ui->deletePersonButton, &QPushButton::clicked, this, &OwnersPage::onDeletePersonClicked);
    connect(ui->personsRefreshButton, &QPushButton::clicked, this, &OwnersPage::onRefreshPersonsClicked);
    connect(ui->personSearchLineEdit, &QLineEdit::textChanged, m_personsProxy, &PersonsProxyModel::setFilterText);

    connect(ui->newInstitutionButton, &QPushButton::clicked, this, &OwnersPage::onNewInstitutionClicked);
    connect(ui->editInstitutionButton, &QPushButton::clicked, this, &OwnersPage::onEditInstitutionClicked);
    connect(ui->deleteInstitutionButton, &QPushButton::clicked, this, &OwnersPage::onDeleteInstitutionClicked);
    connect(ui->institutionRefreshButton, &QPushButton::clicked, this, &OwnersPage::onRefreshInstitutionsClicked);
    connect(ui->institutionSearchLineEdit, &QLineEdit::textChanged, m_institutionsProxy, &InstitutionsProxyModel::setFilterText);
}

void OwnersPage::reconnectPersonsSelectionModel()
{
    QItemSelectionModel* sel = ui->personsTableView->selectionModel();
    connect(sel, &QItemSelectionModel::selectionChanged,
            this, &OwnersPage::onPersonsSelectionChanged);
}

void OwnersPage::reconnectInstitutionsSelectionModel()
{
    QItemSelectionModel* sel = ui->institutionsTableView->selectionModel();
    connect(sel, &QItemSelectionModel::selectionChanged,
            this, &OwnersPage::onInstitutionsSelectionChanged);
}

int OwnersPage::getSelectedPersonId() const
{
    QModelIndex proxyIndex = ui->personsTableView->currentIndex();
    if (!proxyIndex.isValid())
        return -1;

    QModelIndex sourceIndex = m_personsProxy->mapToSource(proxyIndex);
    return sourceIndex.data(Qt::UserRole).toInt();
}

int OwnersPage::getSelectedInstitutionId() const
{
    QModelIndex proxyIndex = ui->institutionsTableView->currentIndex();
    if (!proxyIndex.isValid())
        return -1;

    QModelIndex sourceIndex = m_institutionsProxy->mapToSource(proxyIndex);
    return sourceIndex.data(Qt::UserRole).toInt();
}
