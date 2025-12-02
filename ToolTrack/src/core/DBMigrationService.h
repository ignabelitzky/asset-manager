#pragma once

#include <QString>
#include <QStringList>
#include <QSqlDatabase>

class DatabaseManager;

class DBMigrationService
{
public:
    explicit DBMigrationService(DatabaseManager& dbManager);

    // Apply all SQL migrations from a list of resource files
    bool applyMigrationsFromResources(const QStringList& resourceFiles);

private:
    DatabaseManager& m_dbManager;

    // Ensure the schema_migrations table exists
    bool ensureMigrationsTable();

    // Get list of already applied migration filenames
    QStringList appliedMigrations() const;

    // Execute all SQL statements from a single SQL file
    bool executeSqlFile(QSqlDatabase& db, const QString& filePath);

    // Split SQL content into individual statements, respecting string literals
    QStringList splitSqlStatements(const QString& sql) const;
};
