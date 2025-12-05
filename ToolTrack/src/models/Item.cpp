#include "Item.h"

Item::Item()
    : m_id(0)
    , m_typeId(0)
    , m_stateId(0)
    , m_stock(0)
    , m_ownerTypeId(0)
    , m_ownerId(0)
    , m_locationId(0)
{
}

void Item::setId(int id)
{
    m_id = id;
}

void Item::setName(const QString& name)
{
    m_name = name;
}

void Item::setBarcode(const QString& barcode)
{
    m_barcode = barcode;
}

void Item::setTypeId(int id)
{
    m_typeId = id;
}

void Item::setStateId(int id)
{
    m_stateId = id;
}

void Item::setStock(int stock)
{
    m_stock = stock;
}

void Item::setBrand(const QString& brand)
{
    m_brand = brand;
}

void Item::setModel(const QString& model)
{
    m_model = model;
}

void Item::setOwner(int ownerTypeId, int ownerId)
{
    m_ownerTypeId = ownerTypeId;
    m_ownerId = ownerId;
}

void Item::setLocationId(int id)
{
    m_locationId = id;
}

void Item::setNotes(const QString& notes)
{
    m_notes = notes;
}

int Item::id() const
{
    return m_id;
}

QString Item::name() const
{
    return m_name;
}

QString Item::barcode() const
{
    return m_barcode;
}

int Item::typeId() const
{
    return m_typeId;
}

int Item::stateId() const
{
    return m_stateId;
}

int Item::stock() const
{
    return m_stock;
}

QString Item::brand() const
{
    return m_brand;
}

QString Item::model() const
{
    return m_model;
}

int Item::ownerTypeId() const
{
    return m_ownerTypeId;
}

int Item::ownerId() const
{
    return m_ownerId;
}

int Item::locationId() const
{
    return m_locationId;
}

QString Item::notes() const
{
    return m_notes;
}

bool Item::hasOwner() const
{
    return (m_ownerTypeId > 0 && m_ownerId > 0);
}

bool Item::isValid() const
{
    if (m_name.trimmed().isEmpty())
        return false;

    if (m_barcode.trimmed().isEmpty())
        return false;

    if (m_typeId <= 0)
        return false;

    if (m_stateId <= 0)
        return false;

    if (m_stock < 0)
        return false;

    // Owner consistency
    const bool ownerTypeValid = (m_ownerTypeId > 0);
    const bool ownerIdValid = (m_ownerId > 0);

    if (ownerTypeValid != ownerIdValid)
        return false;

    if (m_locationId < 0)
        return false;

    return true;
}
