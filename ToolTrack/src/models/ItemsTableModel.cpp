#include "ItemsTableModel.h"

ItemsTableModel::ItemsTableModel(ItemsDAO& itemsDAO,
                                 ItemTypesDAO& itemTypesDAO,
                                 ItemStatesDAO& itemStatesDAO,
                                 OwnerTypesDAO& ownerTypesDAO,
                                 PersonsDAO& personsDAO,
                                 InstitutionsDAO& institutionsDAO,
                                 LocationsDAO& locationsDAO,
                                 QObject* parent)
    : QAbstractTableModel(parent)
    , m_itemsDAO(itemsDAO)
    , m_itemTypesDAO(itemTypesDAO)
    , m_itemStatesDAO(itemStatesDAO)
    , m_ownerTypesDAO(ownerTypesDAO)
    , m_personsDAO(personsDAO)
    , m_institutionsDAO(institutionsDAO)
    , m_locationsDAO(locationsDAO)
{
    refresh();
}

int ItemsTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_items.size();
}

int ItemsTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}

QVariant ItemsTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.size())
        return QVariant();

    const Item& item = m_items[index.row()];

    if (role == Qt::UserRole)
        return item.id();

    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    if (role != Qt::DisplayRole)
        return QVariant();

    switch (index.column())
    {
    case Name:
        return item.name();

    case Barcode:
        return item.barcode();

    case Type:
    {
        int typeId = item.typeId();
        if (!m_typeCache.contains(typeId))
            m_typeCache[typeId] = m_itemTypesDAO.getNameById(typeId);
        return m_typeCache[typeId];
    }

    case State:
    {
        int stateId = item.stateId();
        if (!m_stateCache.contains(stateId))
            m_stateCache[stateId] = m_itemStatesDAO.getNameById(stateId);
        return m_stateCache[stateId];
    }

    case Stock:
        return item.stock();

    case AvailableStock:
        return item.availableStock();

    case Owner:
        return resolveOwnerName(item.ownerTypeId(), item.ownerId());

    case Location:
    {
        int locId = item.locationId();
        if (!m_locationCache.contains(locId))
            m_locationCache[locId] = m_locationsDAO.getNameById(locId);
        return m_locationCache[locId];
    }

    default:
        return QVariant();
    }
}

QVariant ItemsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case Name:
            return "Nombre";
        case Barcode:
            return "Código";
        case Type:
            return "Tipo";
        case State:
            return "Estado";
        case Stock:
            return "Stock Total";
        case AvailableStock:
            return "Stock Disponible";
        case Owner:
            return "Responsable";
        case Location:
            return "Ubicación";
        default:
            return QVariant();
        }
    }

    return section + 1;
}

void ItemsTableModel::refresh()
{
    beginResetModel();
    m_items = m_itemsDAO.getAll();

    // optional: clean cache for new data
    m_typeCache.clear();
    m_stateCache.clear();
    m_locationCache.clear();
    m_personCache.clear();
    m_institutionCache.clear();

    endResetModel();
}

const Item& ItemsTableModel::getItem(int row) const
{
    if (row < 0 || row >= m_items.size())
    {
        static Item emptyItem;
        return emptyItem;
    }
    return m_items[row];
}


QString ItemsTableModel::resolveOwnerName(int ownerTypeId, int ownerId) const
{
    if (ownerId <= 0)
        return "-";

    // ownerTypeId: 1 = Persona, 2 = Institución
    if (ownerTypeId == 1)   // Persona
    {
        if (!m_personCache.contains(ownerId))
            m_personCache[ownerId] = m_personsDAO.getFullNameById(ownerId);
        return m_personCache[ownerId];
    }
    else if (ownerTypeId == 2) // Institución
    {
        if (!m_institutionCache.contains(ownerId))
            m_institutionCache[ownerId] = m_institutionsDAO.getNameById(ownerId);
        return m_institutionCache[ownerId];
    }

    return "-";
}
