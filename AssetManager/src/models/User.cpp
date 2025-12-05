#include "User.h"

User::User(const int id, const QString& firstName,
           const QString& lastName, const QString& barcode)
    : m_id(id)
    , m_firstName(firstName)
    , m_lastName(lastName)
    , m_barcode(barcode)
{
}

/* --- setters --- */

void User::setId(const int id)
{
    m_id = id;
}

void User::setFirstName(const QString& firstName)
{
    m_firstName = firstName.trimmed();
}

void User::setLastName(const QString& lastName)
{
    m_lastName = lastName.trimmed();
}

void User::setBarcode(const QString& barcode)
{
    m_barcode = barcode.trimmed();
}

/* --- getters --- */

int User::id() const
{
    return m_id;
}

QString User::firstName() const
{
    return m_firstName;
}

QString User::lastName() const
{
    return m_lastName;
}

QString User::barcode() const
{
    return m_barcode;
}
