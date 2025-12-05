#pragma once

#include <QString>
#include "src/core/Dashboard.h"
#include "src/dao/UsersDAO.h"
#include "src/dao/ItemsDAO.h"
#include "src/dao/ItemTypesDAO.h"
#include "src/dao/ItemStatesDAO.h"
#include "src/dao/OwnerTypesDAO.h"
#include "src/dao/PersonsDAO.h"
#include "src/dao/InstitutionsDAO.h"
#include "src/dao/LocationsDAO.h"

class AppController
{
public:
    AppController();
    ~AppController();

    bool initialize();
    void shutdown();
    void showDashboard();

    QString databasePath() const;

private:
    QString m_dbPath;
    UsersDAO* m_usersDAO;
    ItemsDAO* m_itemsDAO;
    ItemTypesDAO* m_itemTypesDAO;
    ItemStatesDAO* m_itemStatesDAO;
    OwnerTypesDAO* m_ownerTypesDAO;
    PersonsDAO* m_personsDAO;
    InstitutionsDAO* m_institutionsDAO;
    LocationsDAO* m_locationsDAO;

    Dashboard* m_dashboard{nullptr};

    bool prepareDatabaseDirectory() const;
    bool openDatabase() const;
    bool runMigrations() const;
};
