#include "DBMigrationService.h"
#include "DatabaseManager.h"

#include <QFile>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QFileInfo>
#include <QDebug>

DBMigrationService::DBMigrationService(DatabaseManager& dbManager)
    : m_dbManager(dbManager)
{
}

bool DBMigrationService::applyMigrationsFromResources(const QStringList& resourceFiles)
{
    if (!m_dbManager.isOpen()) {
        qDebug() << "[MIGRATIONS] Database not open.";
        return false;
    }

    if (!ensureMigrationsTable())
        return false;

    QStringList applied = appliedMigrations();

    for (const QString& filePath : resourceFiles) {
        QString fileName = QFileInfo(filePath).fileName();

        if (applied.contains(fileName)) {
            qDebug() << "[MIGRATIONS] Already applied:" << fileName;
            continue;
        }

        qDebug() << "[MIGRATIONS] Applying migration:" << fileName;

        QSqlDatabase db = m_dbManager.db();
        if (!db.transaction()) {
            qDebug() << "[MIGRATIONS] Failed to start transaction for" << fileName;
            return false;
        }

        if (!executeSqlFile(db, filePath)) {
            qDebug() << "[MIGRATIONS] Migration failed, rolling back:" << fileName;
            db.rollback();
            return false;
        }

        if (!db.commit()) {
            qDebug() << "[MIGRATIONS] Commit failed for" << fileName << ":" << db.lastError().text();
            return false;
        }

        qDebug() << "[MIGRATIONS] Applied successfully:" << fileName;
    }

    qDebug() << "[MIGRATIONS] All migrations applied.";
    return true;
}

bool DBMigrationService::ensureMigrationsTable()
{
    QSqlQuery query(m_dbManager.db());

    const char* sql =
        "CREATE TABLE IF NOT EXISTS schema_migrations ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " filename TEXT UNIQUE NOT NULL,"
        " applied_at TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP"
        ");";

    if (!query.exec(sql)) {
        qDebug() << "[MIGRATIONS] Failed creating schema_migrations:" << query.lastError().text();
        return false;
    }

    return true;
}

QStringList DBMigrationService::appliedMigrations() const
{
    QStringList list;
    QSqlQuery query(m_dbManager.db());

    if (!query.exec("SELECT filename FROM schema_migrations ORDER BY filename ASC;"))
        return list;

    while (query.next())
        list << query.value(0).toString();

    return list;
}

bool DBMigrationService::executeSqlFile(QSqlDatabase& db, const QString& filePath)
{
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "[MIGRATIONS] Cannot open SQL file:" << filePath;
        return false;
    }

    QTextStream in(&file);
    QString sql = in.readAll();
    file.close();

    QStringList statements = splitSqlStatements(sql);

    QSqlQuery query(db);
    for (const QString& stmt : statements) {
        if (!query.exec(stmt)) {
            qDebug() << "[MIGRATIONS] SQL execution error in file" << filePath
                     << "\nError:" << query.lastError().text()
                     << "\nStatement:" << stmt;
            return false;
        }
    }

    // Record migration
    QSqlQuery insert(db);
    insert.prepare("INSERT INTO schema_migrations (filename) VALUES (:filename)");
    insert.bindValue(":filename", QFileInfo(filePath).fileName());
    if (!insert.exec()) {
        qDebug() << "[MIGRATIONS] Failed to log migration" << filePath << ":" << insert.lastError().text();
        return false;
    }

    return true;
}

QStringList DBMigrationService::splitSqlStatements(const QString& sql) const
{
    QStringList statements;
    QString current;
    bool inString = false;
    QChar stringChar;

    for (int i = 0; i < sql.size(); ++i) {
        QChar c = sql[i];

        if (inString) {
            if (c == stringChar) {
                // Handle escaped quote
                if (i + 1 < sql.size() && sql[i + 1] == stringChar) {
                    current += c;
                    ++i;
                    continue;
                }
                inString = false;
            }
            current += c;
        } else {
            if (c == '\'' || c == '"') {
                inString = true;
                stringChar = c;
                current += c;
            } else if (c == ';') {
                QString stmt = current.trimmed();
                if (!stmt.isEmpty())
                    statements.append(stmt);
                current.clear();
            } else {
                current += c;
            }
        }
    }

    QString remaining = current.trimmed();
    if (!remaining.isEmpty())
        statements.append(remaining);

    return statements;
}
