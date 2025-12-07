#include "StatesTableModel.h"


StatesTableModel::StatesTableModel(ItemStatesDAO& itemStatesDAO,
                                   QObject *parent)
    : QAbstractTableModel(parent)
    , m_itemStatesDAO(itemStatesDAO)
{
    refresh();
}

int StatesTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_states.size();
}
int StatesTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}
QVariant StatesTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    if (!index.isValid() || index.row() >= m_states.size())
        return QVariant();

    const ItemState& state = m_states[index.row()];

    if (role == Qt::UserRole)
        return state.id;

    if (role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case Name:
            return state.name;
        default:
            return QVariant();
        }
    }
    return QVariant();
}
QVariant StatesTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case Name:
            return "Estado";
        default:
            return QVariant();
        }
    }
    return section + 1;
}

// Custom actions
void StatesTableModel::refresh()
{
    beginResetModel();
    m_states = m_itemStatesDAO.getAll();
    endResetModel();
}

const ItemState& StatesTableModel::getState(int row) const
{
    if (row < 0 || row >= m_states.size())
    {
        static ItemState emptyState;
        return emptyState;
    }
    return m_states[row];
}

