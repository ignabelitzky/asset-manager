#pragma once

#include <QMainWindow>
#include "src/dao/UsersDAO.h"
#include "src/dao/ItemsDAO.h"
#include "src/dao/ItemTypesDAO.h"
#include "src/dao/ItemStatesDAO.h"
#include "src/dao/OwnerTypesDAO.h"
#include "src/dao/PersonsDAO.h"
#include "src/dao/InstitutionsDAO.h"
#include "src/dao/LocationsDAO.h"
#include "src/dao/CheckoutDAO.h"

struct PageInfo
{
    QWidget* page;
    QString title;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class Dashboard;
}
QT_END_NAMESPACE

class Dashboard : public QMainWindow
{
    Q_OBJECT

public:
    Dashboard(UsersDAO& usersDAO,
              ItemsDAO& itemsDAO,
              ItemTypesDAO& itemTypesDAO,
              ItemStatesDAO& itemStatesDAO,
              OwnerTypesDAO& ownerTypesDAO,
              PersonsDAO& personsDAO,
              InstitutionsDAO& institutionsDAO,
              LocationsDAO& locationsDAO,
              CheckoutDAO& checkoutDAO,
              QWidget *parent = nullptr);
    ~Dashboard();

private:
    void registerPage(const QString& name, QWidget* page, const QString& pageTitle);
    void switchToPage(const QString& name);
    void setupConnections();

private:
    Ui::Dashboard *ui;
    UsersDAO& m_usersDAO;
    ItemsDAO& m_itemsDAO;
    ItemTypesDAO& m_itemTypesDAO;
    ItemStatesDAO& m_itemStatesDAO;
    OwnerTypesDAO& m_ownerTypesDAO;
    PersonsDAO& m_personsDAO;
    InstitutionsDAO& m_institutionsDAO;
    LocationsDAO& m_locationsDAO;
    CheckoutDAO& m_checkoutDAO;
    QMap<QString, PageInfo> m_pages;
};
