#pragma once

#include <QVector>
#include <QString>

struct Location
{
    int id;
    QString name;
};

class LocationsDAO
{
public:
    LocationsDAO() = default;
    ~LocationsDAO() = default;

    QVector<Location> getAll() const;
    Location getById(int id) const;
    QString getNameById(int id) const;

    bool insert(const QString& name);
    bool update(int id, const QString& name);
    bool remove(int id);
};
