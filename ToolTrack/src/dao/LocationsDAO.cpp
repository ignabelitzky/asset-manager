#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "LocationsDAO.h"
#include "src/core/DatabaseManager.h"

QVector<Location> LocationsDAO::getAll() const
{
    QVector<Location> result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name
        FROM locations
        ORDER BY id;
    )");

    if (!query.exec())
    {
        qDebug() << "[LOCATIONSDAO] Error in getAll:" << query.lastError().text();
        return result;
    }
    while(query.next())
    {
        QSqlRecord record = query.record();
        result.append({record.value("id").toInt(), record.value("name").toString()});
    }
    return result;
}

Location LocationsDAO::getById(int id) const
{
    Location result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name
        FROM locations
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[LOCATIONSDAO] Error in getById:" << query.lastError().text();
        return result;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        result = {record.value("id").toInt(), record.value("name").toString()};
    }
    return result;
}

QString LocationsDAO::getNameById(int id) const
{
    QString result{};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT name
        FROM locations
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[LOCATIONSDAO] Error in getNameById:" << query.lastError().text();
        return result;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        result = record.value("name").toString();
    }
    return result;
}

bool LocationsDAO::insert(const QString& name)
{
    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        INSERT INTO locations (name) VALUES (:name)
    )");
    query.bindValue(":name", name);

    if (!query.exec())
    {
        qDebug() << "[LOCATIONSDAO] Error in insert:" << query.lastError().text();
        return false;
    }
    return true;
}

bool LocationsDAO::update(int id, const QString& name)
{
    if (id <= 0)
    {
        qDebug() << "[LOCATIONSDAO] update called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        UPDATE locations
        SET name = :name
        WHERE id = :id
    )");
    query.bindValue(":name", name);
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[LOCATIONSDAO] Error in update:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool LocationsDAO::remove(int id)
{
    if (id <= 0)
    {
        qDebug() << "[LOCATIONSDAO] remove called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        DELETE FROM locations
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[LOCATIONSDAO] Error in remove:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}
