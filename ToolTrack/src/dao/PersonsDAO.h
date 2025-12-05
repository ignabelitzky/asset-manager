#pragma once

#include <QVector>
#include <QString>

struct Person
{
    int id;
    QString firstName;
    QString lastName;
};

class PersonsDAO
{
public:
    PersonsDAO() = default;
    ~PersonsDAO() = default;

    QVector<Person> getAll() const;
    Person getById(int id) const;
    QString getFullNameById(int id) const;

    bool insert(const QString& firstName, const QString& lastName);
    bool update(int id, const QString& firstName, const QString& lastName);
    bool remove(int id);
};
