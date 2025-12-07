#include "TypesTableModel.h"

TypesTableModel::TypesTableModel(ItemTypesDAO& itemTypesDAO,
                         QObject *parent)
    : QAbstractTableModel(parent)
    , m_itemTypesDAO(itemTypesDAO)
{
    refresh();
}

int TypesTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_types.size();

}

int TypesTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}

QVariant TypesTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    if (!index.isValid() || index.row() >= m_types.size())
        return QVariant();

    const ItemType& type = m_types[index.row()];

    if (role == Qt::UserRole)
        return type.id;

    if (role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case Name:
            return type.name;
        default:
            return QVariant();
        }
    }
    return QVariant();
}

QVariant TypesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case Name:
            return "Tipo";
        default:
            return QVariant();
        }
    }
    return section + 1;
}

// Custom actions
void TypesTableModel::refresh()
{
    beginResetModel();
    m_types = m_itemTypesDAO.getAll();
    endResetModel();
}

const ItemType& TypesTableModel::getType(int row) const
{
    if (row < 0 || row >= m_types.size())
    {
        static ItemType emptyType;
        return emptyType;
    }
    return m_types[row];
}
