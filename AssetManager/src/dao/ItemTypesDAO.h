#pragma once

#include <QVector>
#include <QString>

struct ItemType
{
    int id;
    QString name;
};

class ItemTypesDAO
{
public:
    ItemTypesDAO() = default;
    ~ItemTypesDAO() = default;

    QVector<ItemType> getAll() const;
    ItemType getById(int id) const;
    QString getNameById(int id) const;

    bool insert(const QString& name);
    bool update(int id, const QString& name);
    bool remove(int id);
};
