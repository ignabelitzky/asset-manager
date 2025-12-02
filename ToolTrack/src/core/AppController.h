#pragma once

#include <QString>

class AppController
{
public:
    AppController();
    ~AppController();

    bool initialize();
    void shutdown();

    QString databasePath() const;

private:
    QString m_dbPath;

    bool prepareDatabaseDirectory() const;
    bool openDatabase() const;
    bool runMigrations() const;
};
