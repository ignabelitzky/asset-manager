#include "CartTableModel.h"

CartTableModel::CartTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

int CartTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_lines.size();
}

int CartTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}

QVariant CartTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() < 0 || index.row() >= m_lines.size())
        return QVariant();

    const CartLine& line = m_lines.at(index.row());

    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
        case Name:     return line.name;
        case Quantity: return line.quantity;
        default:       return QVariant();
        }
    }

    if (role == Qt::UserRole)
        return line.itemId;

    return QVariant();
}

QVariant CartTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();
    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case Name:
            return "Nombre";
        case Quantity:
            return "Cantidad";
        default:
            return QVariant();
        }
    }
    return section + 1;
}

Qt::ItemFlags CartTableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    Qt::ItemFlags f = Qt::ItemIsSelectable | Qt::ItemIsEnabled;
    if (index.column() == Quantity)
        f |= Qt::ItemIsEditable;

    return f;
}

bool CartTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    bool ok = false;
    int newQty = value.toInt(&ok);
    if (!ok || newQty < 0)
        return false;

    int row = index.row();
    if (row < 0 || row >= m_lines.size())
        return false;

    if (newQty == 0)
        removeItem(m_lines[row].itemId);
    else
    {
        m_lines[row].quantity = newQty;
        emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    }

    emit cartChanged();
    return true;
}

int CartTableModel::findRowByItemId(int itemId) const
{
    if (m_indexByItemId.contains(itemId))
        return m_indexByItemId.value(itemId);
    for (int i = 0; i < m_lines.size(); ++i)
        if (m_lines[i].itemId == itemId)
            return i;
    return -1;
}

void CartTableModel::addItem(int itemId, const QString& name)
{
    int row = findRowByItemId(itemId);
    if (row >= 0)
    {
        m_lines[row].quantity += 1;
        emit dataChanged(index(row, Quantity), index(row, Quantity), {Qt::DisplayRole, Qt::EditRole});
    }
    else
    {
        CartLine line;
        line.itemId = itemId;
        line.name = name;
        line.quantity = 1;
        beginInsertRows(QModelIndex(), m_lines.size(), m_lines.size());
        m_lines.append(line);
        m_indexByItemId[line.itemId] = m_lines.size() - 1;
        endInsertRows();
    }
    emit cartChanged();
}

void CartTableModel::setQuantity(int itemId, int qty)
{
    int row = findRowByItemId(itemId);
    if (row < 0)
        return;
    setData(index(row, Quantity), qty, Qt::EditRole);
}

void CartTableModel::removeItem(int itemId)
{
    int row = findRowByItemId(itemId);
    if (row < 0)
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_lines.removeAt(row);
    m_indexByItemId.remove(itemId);

    // rebuild index map
    m_indexByItemId.clear();
    for (int i = 0; i < m_lines.size(); ++i)
        m_indexByItemId[m_lines[i].itemId] = i;

    endRemoveRows();
    emit cartChanged();
}

void CartTableModel::clear()
{
    if (m_lines.isEmpty())
        return;
    beginResetModel();
    m_lines.clear();
    m_indexByItemId.clear();
    endResetModel();
    emit cartChanged();
}

bool CartTableModel::contains(int itemId) const
{
    return findRowByItemId(itemId) >= 0;
}

int CartTableModel::totalLines() const
{
    return m_lines.size();
}

QVector<CartLine> CartTableModel::lines() const
{
    return m_lines;
}

void CartTableModel::setItems(const QVector<CartLine>& items)
{
    beginResetModel();
    m_lines = items;

    m_indexByItemId.clear();
    for (int i = 0; i < m_lines.size(); ++i)
        m_indexByItemId[m_lines[i].itemId] = i;

    endResetModel();
    emit cartChanged();
}
