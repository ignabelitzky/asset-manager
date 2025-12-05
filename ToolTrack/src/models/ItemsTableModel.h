#pragma once

#include <QAbstractTableModel>
#include <QObject>

#include "src/dao/ItemsDAO.h"
#include "src/dao/ItemTypesDAO.h"
#include "src/dao/ItemStatesDAO.h"
#include "src/dao/OwnerTypesDAO.h"
#include "src/dao/PersonsDAO.h"
#include "src/dao/InstitutionsDAO.h"
#include "src/dao/LocationsDAO.h"

#include "src/models/Item.h"

class ItemsTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ItemsTableModel(ItemsDAO& itemsDAO,
                             ItemTypesDAO& itemTypesDAO,
                             ItemStatesDAO& itemStatesDAO,
                             OwnerTypesDAO& ownerTypesDAO,
                             PersonsDAO& personsDAO,
                             InstitutionsDAO& institutionsDAO,
                             LocationsDAO& locationsDAO,
                             QObject* parent = nullptr);

    ~ItemsTableModel() = default;

    enum Columns {
        Name,
        Barcode,
        Type,
        State,
        Stock,
        Owner,
        Location,
        COLUMN_COUNT
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    void refresh();
    const Item& getItem(int row) const;

private:
    ItemsDAO& m_itemsDAO;
    ItemTypesDAO& m_itemTypesDAO;
    ItemStatesDAO& m_itemStatesDAO;
    OwnerTypesDAO& m_ownerTypesDAO;
    PersonsDAO& m_personsDAO;
    InstitutionsDAO& m_institutionsDAO;
    LocationsDAO& m_locationsDAO;

    QVector<Item> m_items;

    // caches opcionales para mejorar rendimiento
    mutable QHash<int, QString> m_typeCache;
    mutable QHash<int, QString> m_stateCache;
    mutable QHash<int, QString> m_locationCache;
    mutable QHash<int, QString> m_ownerCache;
    mutable QHash<int, QString> m_personCache;
    mutable QHash<int, QString> m_institutionCache;

    QString resolveOwnerName(int ownerTypeId, int ownerId) const;
};
