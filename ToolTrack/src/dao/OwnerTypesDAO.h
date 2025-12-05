#pragma once

#include <QVector>
#include <QString>

struct OwnerType
{
    int id;
    QString name;
};

class OwnerTypesDAO
{
public:
    OwnerTypesDAO() = default;
    ~OwnerTypesDAO() = default;

    QVector<OwnerType> getAll() const;
    OwnerType getById(int id) const;

    bool insert(const QString& name);
    bool update(int id, const QString& name);
    bool remove(int id);
};
