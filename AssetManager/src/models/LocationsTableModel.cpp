#include "LocationsTableModel.h"

LocationsTableModel::LocationsTableModel(LocationsDAO& locationsDAO,
                                         QObject *parent)
    : QAbstractTableModel(parent)
    , m_locationsDAO(locationsDAO)
{
    refresh();
}

int LocationsTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_locations.size();
}

int LocationsTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}

QVariant LocationsTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    if (!index.isValid() || index.row() >= m_locations.size())
        return QVariant();

    const Location& location = m_locations[index.row()];

    if (role == Qt::UserRole)
        return location.id;

    if (role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case Name:
            return location.name;
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QVariant LocationsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case Name:
            return "Ubicación";
        default:
            return QVariant();
        }
    }
    return section + 1;
}

// Custom actions
void LocationsTableModel::refresh()
{
    beginResetModel();
    m_locations = m_locationsDAO.getAll();
    endResetModel();
}

const Location& LocationsTableModel::getLocation(int row) const
{
    if (row < 0 || row >= m_locations.size())
    {
        static Location emptyLocation;
        return emptyLocation;
    }
    return m_locations[row];
}
