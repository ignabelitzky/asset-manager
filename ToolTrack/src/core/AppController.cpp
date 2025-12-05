#include "src/core/AppController.h"
#include "src/core/DatabaseManager.h"
#include "src/core/DBMigrationService.h"

#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QDebug>

AppController::AppController()
    : m_usersDAO(new UsersDAO())
    , m_itemsDAO(new ItemsDAO())
    , m_itemTypesDAO(new ItemTypesDAO())
    , m_itemStatesDAO(new ItemStatesDAO())
    , m_ownerTypesDAO(new OwnerTypesDAO())
    , m_personsDAO(new PersonsDAO())
    , m_institutionsDAO(new InstitutionsDAO())
    , m_locationsDAO(new LocationsDAO())
{
    const QString appDataDir =
        QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    m_dbPath = appDataDir + "/tooltrack.db";
}

AppController::~AppController()
{
    delete m_usersDAO;
    delete m_itemsDAO;
    delete m_itemTypesDAO;
    delete m_itemStatesDAO;
    delete m_ownerTypesDAO;
    delete m_personsDAO;
    delete m_institutionsDAO;
    delete m_locationsDAO;
    shutdown();
}

bool AppController::initialize()
{
    qDebug() << "[APPCONTROLLER] Starting initialization...";
    qDebug() << "[APPCONTROLLER] Database path:" << m_dbPath;

    if (!prepareDatabaseDirectory())
        return false;

    if (!openDatabase())
        return false;

    if (!runMigrations())
        return false;

    qDebug() << "[APPCONTROLLER] Initialization complete.";
    return true;
}

void AppController::shutdown()
{
    qDebug() << "[APPCONTROLLER] Shutting down...";
    DatabaseManager::instance().close();
}

void AppController::showDashboard()
{
    m_dashboard = new Dashboard(*m_usersDAO,
                                *m_itemsDAO,
                                *m_itemTypesDAO,
                                *m_itemStatesDAO,
                                *m_ownerTypesDAO,
                                *m_personsDAO,
                                *m_institutionsDAO,
                                *m_locationsDAO);
    m_dashboard->show();
}

QString AppController::databasePath() const
{
    return m_dbPath;
}

bool AppController::prepareDatabaseDirectory() const
{
    QFileInfo info(m_dbPath);
    QDir dir;

    if (!dir.mkpath(info.path()))
    {
        qDebug() << "[APPCONTROLLER] Failed to create directory:" << info.path();
        return false;
    }

    return true;
}

bool AppController::openDatabase() const
{
    if (!DatabaseManager::instance().open(m_dbPath))
    {
        qDebug() << "[APPCONTROLLER] Failed to open database.";
        return false;
    }

    return true;
}

bool AppController::runMigrations() const
{
    DBMigrationService migrator(DatabaseManager::instance());

    // List all resource SQL files explicitly
    QStringList migrations = {
        ":/migrations/resources/sql/001_init.sql"
        // Add more here as needed
    };

    if (!migrator.applyMigrationsFromResources(migrations)) {
        qDebug() << "[APPCONTROLLER] Migration process failed.";
        return false;
    }

    return true;
}

