#pragma once

#include <QVector>
#include <QString>

struct Institution
{
    int id;
    QString name;
};

class InstitutionsDAO
{
public:
    InstitutionsDAO() = default;
    ~InstitutionsDAO() = default;

    QVector<Institution> getAll() const;
    Institution getById(int id) const;
    QString getNameById(int id) const;

    bool insert(const QString& name);
    bool update(int id, const QString& name);
    bool remove(int id);
};
