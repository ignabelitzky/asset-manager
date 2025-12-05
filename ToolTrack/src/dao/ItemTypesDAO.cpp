#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include "ItemTypesDAO.h"
#include "src/core/DatabaseManager.h"

QVector<ItemType> ItemTypesDAO::getAll() const
{
    QVector<ItemType> result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name
        FROM item_types
    )");

    if (!query.exec())
    {
        qDebug() << "[ITEMTYPESDAO] Error in getAll:" << query.lastError().text();
        return result;
    }
    while (query.next())
    {
        QSqlRecord record = query.record();
        result.append({record.value("id").toInt(), record.value("name").toString()});
    }
    return result;
}

ItemType ItemTypesDAO::getById(int id) const
{
    ItemType result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name
        FROM item_types
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[ITEMTYPESDAO] Error in getById:" << query.lastError().text();
        return result;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        result = {record.value("id").toInt(), record.value("name").toString()};
    }
    return result;
}

bool ItemTypesDAO::insert(const QString& name)
{
    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        INSERT INTO item_types (name) VALUES (:name)
    )");
    query.bindValue(":name", name);

    if (!query.exec())
    {
        qDebug() << "[ITEMTYPESDAO] Error in insert:" << query.lastError().text();
        return false;
    }
    return true;
}

bool ItemTypesDAO::update(int id, const QString& name)
{
    if (id <= 0)
    {
        qDebug() << "[ITEMTYPESDAO] update called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        UPDATE item_types
        SET name = :name
        WHERE id = :id
    )");
    query.bindValue(":name", name);
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[ITEMTYPESDAO] Error in update:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool ItemTypesDAO::remove(int id)
{
    if (id <= 0)
    {
        qDebug() << "[ITEMTYPESDAO] remove called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        DELETE FROM item_types
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[ITEMTYPESDAO] Error in remove:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}
