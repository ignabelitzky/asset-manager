#include "src/core/DatabaseManager.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QDebug>

namespace {
constexpr const char* CONNECTION_NAME = "tooltrack_db";
}

DatabaseManager::DatabaseManager()
{
}

DatabaseManager::~DatabaseManager()
{
    close();
}

DatabaseManager& DatabaseManager::instance()
{
    static DatabaseManager instance;
    return instance;
}

bool DatabaseManager::open(const QString& path)
{
    if (m_db.isValid() && m_db.isOpen())
        return true;

    if (!QSqlDatabase::contains(CONNECTION_NAME))
        m_db = QSqlDatabase::addDatabase("QSQLITE", CONNECTION_NAME);
    else
        m_db = QSqlDatabase::database(CONNECTION_NAME);

    m_db.setDatabaseName(path);

    if (!m_db.open()) {
        qDebug() << "[DATABASEMANAGER] Failed to open database:" << m_db.lastError().text();
        return false;
    }

    qDebug() << "[DATABASEMANAGER] Database opened at:" << path;
    return true;
}

void DatabaseManager::close()
{
    if (m_db.isValid() && m_db.isOpen()) {
        m_db.close();
        qDebug() << "[DATABASEMANAGER] Database closed";
    }
}

bool DatabaseManager::isOpen() const
{
    return m_db.isValid() && m_db.isOpen();
}

QSqlDatabase DatabaseManager::db() const
{
    return m_db;
}
