#pragma once

#include <QVector>
#include <QString>

struct ItemState
{
    int id;
    QString name;
};

class ItemStatesDAO
{
public:
    ItemStatesDAO() = default;
    ~ItemStatesDAO() = default;

    QVector<ItemState> getAll() const;
    ItemState getById(int id) const;

    bool insert(const QString& name);
    bool update(int id, const QString& name);
    bool remove(int id);
};
