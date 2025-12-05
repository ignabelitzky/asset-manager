#pragma once

#include <QString>
#include "src/core/Dashboard.h"
#include "src/dao/UsersDAO.h"

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

    Dashboard* m_dashboard{nullptr};

    bool prepareDatabaseDirectory() const;
    bool openDatabase() const;
    bool runMigrations() const;
};
