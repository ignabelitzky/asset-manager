#pragma once

#include <QString>

class Item
{
public:
    Item();
    ~Item() = default;

    void setId(int id);
    void setName(const QString& name);
    void setBarcode(const QString& barcode);
    void setTypeId(int id);
    void setStateId(int id);
    void setStock(int stock);
    void setAvailableStock(int val);
    void setBrand(const QString& brand);
    void setModel(const QString& model);
    void setOwner(int ownerTypeId, int ownerId);
    void setLocationId(int id);
    void setNotes(const QString& notes);

    int id() const;
    QString name() const;
    QString barcode() const;
    int typeId() const;
    int stateId() const;
    int stock() const;
    int availableStock() const;
    QString brand() const;
    QString model() const;
    int ownerTypeId() const;
    int ownerId() const;
    int locationId() const;
    QString notes() const;

    bool hasOwner() const;
    bool isValid() const;

private:
    int m_id = 0;
    QString m_name;
    QString m_barcode;

    int m_typeId = 0;
    int m_stateId = 0;

    int m_stock = 0;
    int m_availableStock;
    QString m_brand;
    QString m_model;

    int m_ownerTypeId = 0;
    int m_ownerId = 0;

    int m_locationId = 0;
    QString m_notes;
};
