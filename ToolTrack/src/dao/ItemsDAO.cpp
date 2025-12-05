#include <QSqlQuery>
#include <QSqlError>
#include "ItemsDAO.h"
#include "src/core/DatabaseManager.h"

QVector<Item> ItemsDAO::getAll() const
{
    QVector<Item> result = {};

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name, barcode, type_id, state_id, stock,
            brand, model, owner_type_id, owner_id, location_id, notes
        FROM items
    )");

    if (!query.exec())
    {
        qDebug() << "[ITEMSDAO] Error in getAll:" << query.lastError().text();
        return result;
    }
    while (query.next())
    {
        QSqlRecord record = query.record();
        result.append(gatherItem(record));
    }

    return result;
}

Item ItemsDAO::getById(int id) const
{
    Item result{};

    if (id <= 0)
    {
        qDebug() << "[ITEMSDAO] getById called with invalid id:" << id;
        return result;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name, barcode, type_id, state_id, stock,
            brand, model, owner_type_id, owner_id, location_id, notes
        FROM items
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[ITEMSDAO] Error in getById:" << query.lastError().text();
        return result;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        result = gatherItem(record);
    }

    return result;
}

std::optional<Item> ItemsDAO::getByBarcode(const QString& barcode) const
{
    std::optional<Item> result = std::nullopt;

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        SELECT id, name, barcode, type_id, state_id, stock,
            brand, model, owner_type_id, owner_id, location_id, notes
        FROM items
        WHERE barcode = :barcode
    )");
    query.bindValue(":barcode", barcode);

    if (!query.exec())
    {
        qDebug() << "[ITEMSDAO] Error in getByBarcode:" << query.lastError().text();
        return result;
    }
    if (query.next())
    {
        QSqlRecord record = query.record();
        result = gatherItem(record);
    }

    return result;
}

bool ItemsDAO::insert(const Item& item) const
{
    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        INSERT INTO items (name, barcode, type_id, state_id, stock, brand,
            model, owner_type_id, owner_id, location_id, notes)
        VALUES (:name, :barcode, :type_id, :state_id, :stock, :brand,
            :model, :owner_type_id, :owner_id, :location_id, :notes)
    )");

    query.bindValue(":name", item.name());
    query.bindValue(":barcode", item.barcode());
    query.bindValue(":type_id", item.typeId());
    query.bindValue(":state_id", item.stateId());
    query.bindValue(":stock", item.stock());
    query.bindValue(":brand", item.brand());
    query.bindValue(":model", item.model());

    if (item.hasOwner())
    {
        query.bindValue(":owner_type_id", item.ownerTypeId());
        query.bindValue(":owner_id", item.ownerId());
    }
    else
    {
        query.bindValue(":owner_type_id", QVariant());
        query.bindValue(":owner_id", QVariant());
    }

    query.bindValue(":location_id", item.locationId() > 0 ? QVariant(item.locationId()) : QVariant());
    query.bindValue(":notes", item.notes());

    if (!query.exec())
    {
        qDebug() << "[ITEMSDAO] Error in insert:" << query.lastError().text();
        return false;
    }

    return true;
}

bool ItemsDAO::update(const Item& item) const
{
    if (item.id() <= 0)
    {
        qDebug() << "[ITEMSDAO] update called with invalid id:" << item.id();
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        UPDATE items
        SET name = :name,
            barcode = :barcode,
            type_id = :type_id,
            state_id = :state_id,
            stock = :stock,
            brand = :brand,
            model = :model,
            owner_type_id = :owner_type_id,
            owner_id = :owner_id,
            location_id = :location_id,
            notes = :notes
        WHERE id = :id
    )");

    query.bindValue(":name", item.name());
    query.bindValue(":barcode", item.barcode());
    query.bindValue(":type_id", item.typeId());
    query.bindValue(":state_id", item.stateId());
    query.bindValue(":stock", item.stock());
    query.bindValue(":brand", item.brand());
    query.bindValue(":model", item.model());

    // If the item has no owner, asign NULL
    if (item.hasOwner())
    {
        query.bindValue(":owner_type_id", item.ownerTypeId());
        query.bindValue(":owner_id", item.ownerId());
    }
    else
    {
        query.bindValue(":owner_type_id", QVariant());
        query.bindValue(":owner_id", QVariant());
    }

    query.bindValue(":location_id", item.locationId() > 0 ? QVariant(item.locationId()) : QVariant());
    query.bindValue(":notes", item.notes());
    query.bindValue(":id", item.id());

    if (!query.exec())
    {
        qDebug() << "[ITEMSDAO] Error in update:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

bool ItemsDAO::remove(int id) const
{
    if (id <= 0)
    {
        qDebug() << "[ITEMSDAO] remove called with invalid id:" << id;
        return false;
    }

    QSqlQuery query(DatabaseManager::instance().db());
    query.prepare(R"(
        DELETE FROM items
        WHERE id = :id
    )");
    query.bindValue(":id", id);

    if (!query.exec())
    {
        qDebug() << "[ITEMSDAO] Error in remove:" << query.lastError().text();
        return false;
    }

    return query.numRowsAffected() > 0;
}

Item ItemsDAO::gatherItem(const QSqlRecord& record) const
{
    Item item;

    item.setId(record.value("id").toInt());
    item.setName(record.value("name").toString());
    item.setBarcode(record.value("barcode").toString());
    item.setTypeId(record.value("type_id").toInt());
    item.setStateId(record.value("state_id").toInt());
    item.setStock(record.value("stock").toInt());
    item.setBrand(record.value("brand").toString());
    item.setModel(record.value("model").toString());
    item.setOwner(record.value("owner_type_id").toInt(),
                  record.value("owner_id").toInt());
    item.setLocationId(record.value("location_id").toInt());
    item.setNotes(record.value("notes").toString());

    return item;

}
