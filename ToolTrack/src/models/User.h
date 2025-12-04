#pragma once

#include <QString>

class User
{
public:
    User() = default;
    User(const int id, const QString& firstName,
         const QString& lastName, const QString& barcode);
    ~User() = default;

    void setId(int id);
    void setFirstName(const QString& firstName);
    void setLastName(const QString& lastName);
    void setBarcode(const QString& barcode);

    int id() const;
    QString firstName() const;
    QString lastName() const;
    QString barcode() const;

private:
    int m_id{-1};
    QString m_firstName;
    QString m_lastName;
    QString m_barcode;
};
