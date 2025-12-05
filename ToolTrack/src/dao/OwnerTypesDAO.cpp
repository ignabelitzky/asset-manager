#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "OwnerTypesDAO.h"
#include "src/core/DatabaseManager.h"

QVector<OwnerType> OwnerTypesDAO::getAll() const
{
    QVector<OwnerType> result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name
        FROM owner_types
    )");

    if (!query.exec())
    {
        qDebug() << "[OWNERTYPESDAO] Error in getAll:" << query.lastError().text();
        return result;
    }
    while (query.next())
    {
        QSqlRecord record = query.record();
        result.append({record.value("id").toInt(), record.value("name").toString()});
    }
    return result;
}

OwnerType OwnerTypesDAO::getById(int id) const
{
    OwnerType result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name
        FROM owner_types
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[OWNERTYPESDAO] Error in getById:" <<query.lastError().text();
        return result;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        result = {record.value("id").toInt(), record.value("name").toString()};
    }
    return result;
}

bool OwnerTypesDAO::insert(const QString& name)
{
    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        INSERT INTO owner_types (name) VALUES (:name)
    )");
    query.bindValue(":name", name);

    if (!query.exec())
    {
        qDebug() << "[OWNERTYPESDAO] Error in insert:" << query.lastError().text();
        return false;
    }
    return true;
}

bool OwnerTypesDAO::update(int id, const QString& name)
{
    if (id <= 0)
    {
        qDebug() << "[OWNERTYPESDAO] update called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        UPDATE owner_types
        SET name = :name
        WHERE id = :id
    )");
    query.bindValue(":name", name);
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[OWNERTYPESDAO] Error in update:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool OwnerTypesDAO::remove(int id)
{
    if (id <= 0)
    {
        qDebug() << "[OWNERTYPESDAO] remove called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        DELETE FROM owner_types
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[OWNERTYPESDAO] Error in remove:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}
