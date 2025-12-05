#pragma once

#include <QSqlRecord>
#include <QVector>
#include <QString>
#include <optional>
#include "src/models/Item.h"

class ItemsDAO
{
public:
    ItemsDAO() = default;
    ~ItemsDAO() = default;

    QVector<Item> getAll() const;
    Item getById(int id) const;
    std::optional<Item> getByBarcode(const QString& barcode) const;

    bool insert(const Item& item) const;
    bool update(const Item& item) const;
    bool remove(int id) const;

private:
    Item gatherItem(const QSqlRecord& record) const;
};
