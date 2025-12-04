#include "Dashboard.h"
#include "ui_Dashboard.h"
#include "src/pages/UsersPage.h"

Dashboard::Dashboard(UserDAO& userDAO, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Dashboard)
    , m_userDAO(userDAO)
{
    ui->setupUi(this);

    QWidget* usersPage = new UsersPage(m_userDAO, this);

    registerPage("users", usersPage, "Usuarios");
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
}
