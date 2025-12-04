#include "UserTableModel.h"

UserTableModel::UserTableModel(UserDAO& userDAO)
    : m_userDAO(userDAO)
{
    refresh();
}

int UserTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return m_users.size();
}

int UserTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}

QVariant UserTableModel::data(const QModelIndex& index, int role) const
{
    if (role == Qt::TextAlignmentRole)
        return Qt::AlignCenter;
    if (!index.isValid() || index.row() >= m_users.size())
        return {};

    const User& user = m_users[index.row()];

    if (role == Qt::UserRole)
        return user.id();

    if (role == Qt::DisplayRole)
    {
        switch (index.column())
        {
        case First_Name:
            return user.firstName();
        case Last_Name:
            return user.lastName();
        case Barcode:
            return user.barcode();
        default:
            return {};
        }
    }
    return {};
}

QVariant UserTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Horizontal)
    {
        switch (section)
        {
        case First_Name:
            return "Nombre";
        case Last_Name:
            return "Apellido";
        case Barcode:
            return "Código Identificatorio";
        default:
            return {};
        }
    }
    return section + 1;
}

void UserTableModel::refresh()
{
    beginResetModel();
    m_users = m_userDAO.getAll();
    endResetModel();
}

const User& UserTableModel::getUser(int row) const
{
    if (row < 0 || row >= m_users.size())
    {
        static User emptyUser;
        return emptyUser;
    }
    return m_users[row];
}
