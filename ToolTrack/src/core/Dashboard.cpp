#include "Dashboard.h"
#include "ui_Dashboard.h"
#include "src/pages/UsersPage.h"
#include "src/pages/ItemsPage.h"
#include "src/pages/CheckoutPage.h"

Dashboard::Dashboard(UsersDAO& usersDAO,
                     ItemsDAO& itemsDAO,
                     ItemTypesDAO& itemTypesDAO,
                     ItemStatesDAO& itemStatesDAO,
                     OwnerTypesDAO& ownerTypesDAO,
                     PersonsDAO& personsDAO,
                     InstitutionsDAO& institutionsDAO,
                     LocationsDAO& locationsDAO,
                     CheckoutDAO& checkoutDAO,
                     QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Dashboard)
    , m_usersDAO(usersDAO)
    , m_itemsDAO(itemsDAO)
    , m_itemTypesDAO(itemTypesDAO)
    , m_itemStatesDAO(itemStatesDAO)
    , m_ownerTypesDAO(ownerTypesDAO)
    , m_personsDAO(personsDAO)
    , m_institutionsDAO(institutionsDAO)
    , m_locationsDAO(locationsDAO)
    , m_checkoutDAO(checkoutDAO)
{
    ui->setupUi(this);

    QWidget* usersPage = new UsersPage(m_usersDAO,
                                       this);
    QWidget* itemsPage = new ItemsPage(m_itemsDAO,
                                       m_itemTypesDAO,
                                       m_itemStatesDAO,
                                       m_ownerTypesDAO,
                                       m_personsDAO,
                                       m_institutionsDAO,
                                       m_locationsDAO,
                                       this);
    QWidget* checkoutPage = new CheckoutPage(m_itemsDAO,
                                             m_checkoutDAO,
                                             m_usersDAO,
                                             this);

    registerPage("users", usersPage, "Usuarios");
    registerPage("items", itemsPage, "Items");
    registerPage("checkout", checkoutPage, "Retiro");
    switchToPage("users");

    setupConnections();
}

Dashboard::~Dashboard()
{
    delete ui;
}

void Dashboard::registerPage(const QString& name,
                             QWidget* page,
                             const QString& pageTitle)
{
    if (!m_pages.contains(name))
    {
        ui->stackedWidget->addWidget(page);
        m_pages.insert(name, PageInfo{page, pageTitle});
    }
}

void Dashboard::switchToPage(const QString& name)
{
    if(m_pages.contains(name))
    {
        const PageInfo& info = m_pages[name];
        ui->stackedWidget->setCurrentWidget(info.page);
        ui->titleLabel->setText(info.title);
    }
    else
    {
        qDebug() << "[DASHBOARD] Unknown page";
    }
}

void Dashboard::setupConnections()
{
    auto connectPageButton = [this](QPushButton* button, const QString& page) {
        connect(button, &QPushButton::clicked, this, [this, page]() {
            switchToPage(page);
        });
    };

    connectPageButton(ui->usersButton, "users");
    connectPageButton(ui->itemsButton, "items");
    connectPageButton(ui->checkoutButton, "checkout");
}
