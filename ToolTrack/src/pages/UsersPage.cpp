#include <QMessageBox>
#include "UsersPage.h"
#include "ui_UsersPage.h"
#include "src/dialogs/NewUserDialog.h"

UsersPage::UsersPage(UserDAO& userDAO, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UsersPage)
    , m_userDAO(userDAO)
    , m_model(new UserTableModel(m_userDAO))
    , m_proxy(new UserProxyModel(this))
{
    ui->setupUi(this);
    ui->editUserButton->setEnabled(false);
    ui->deleteUserButton->setEnabled(false);

    m_proxy->setSourceModel(m_model);

    setupUsersTable();
    setupConnections();
}

UsersPage::~UsersPage()
{
    delete ui;
}

void UsersPage::onSelectionChanged(const QItemSelection& selected,
                                   const QItemSelection& deselected)
{
    const bool hasSelection = !selected.indexes().isEmpty();
    ui->editUserButton->setEnabled(hasSelection);
    ui->deleteUserButton->setEnabled(hasSelection);
}

void UsersPage::onNewUserClicked()
{
    NewUserDialog dialog(m_userDAO, this);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_model->refresh();
        reconnectSelectionModel();
        onSelectionChanged({}, {});
    }
}

void UsersPage::onEditUserClicked()
{
    int userId = getSelectedUserId();
    if (userId < 0)
        return;

    NewUserDialog dialog(m_userDAO, this);
    dialog.loadUser(userId);
    if (dialog.exec() == QDialog::Accepted)
    {
        m_model->refresh();
        reconnectSelectionModel();
        onSelectionChanged({}, {});
    }
}

void UsersPage::onDeleteUserClicked()
{
    int userId = getSelectedUserId();
    if (userId < 0)
        return;

    if (QMessageBox::question(this, "Eliminar usuario", "¿Está seguro que desea eliminar este usuario?") == QMessageBox::Yes)
    {
        m_userDAO.deleteUser(userId);
        m_model->refresh();
        reconnectSelectionModel();
        onSelectionChanged({}, {});
    }
}

void UsersPage::onRefreshClicked()
{
    m_model->refresh();
    ui->tableView->clearSelection();
    onSelectionChanged({}, {});
}

void UsersPage::setupUsersTable()
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

void UsersPage::setupConnections()
{
    QItemSelectionModel* selectionModel = ui->tableView->selectionModel();
    connect(selectionModel, &QItemSelectionModel::selectionChanged,
            this, &UsersPage::onSelectionChanged);
    connect(ui->newUserButton, &QPushButton::clicked, this, &UsersPage::onNewUserClicked);
    connect(ui->editUserButton, &QPushButton::clicked, this, &UsersPage::onEditUserClicked);
    connect(ui->deleteUserButton, &QPushButton::clicked, this, &UsersPage::onDeleteUserClicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &UsersPage::onRefreshClicked);
    connect(ui->searchLineEdit, &QLineEdit::textChanged, m_proxy, &UserProxyModel::setFilterText);
}

void UsersPage::reconnectSelectionModel()
{
    QItemSelectionModel* sel = ui->tableView->selectionModel();
    connect(sel, &QItemSelectionModel::selectionChanged,
            this, &UsersPage::onSelectionChanged);
}

int UsersPage::getSelectedUserId() const
{
    QModelIndex proxyIndex = ui->tableView->currentIndex();
    if(!proxyIndex.isValid())
        return -1;

    QModelIndex sourceIndex = m_proxy->mapToSource(proxyIndex);
    return sourceIndex.data(Qt::UserRole).toInt();
}
