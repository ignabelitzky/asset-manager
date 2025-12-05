#pragma once

#include <QString>
#include <QSqlDatabase>

class DatabaseManager
{
public:
    static DatabaseManager& instance();

    bool open(const QString& path);
    void close();
    bool isOpen() const;

    QSqlDatabase db() const;

private:
    DatabaseManager();
    ~DatabaseManager();

    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    QSqlDatabase m_db;
};
