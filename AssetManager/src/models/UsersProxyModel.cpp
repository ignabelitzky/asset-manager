#include "UsersProxyModel.h"

UsersProxyModel::UsersProxyModel(QObject* parent)
    : QSortFilterProxyModel(parent)
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void UsersProxyModel:: setFilterText(const QString& text)
{
    m_filterText = text;
    invalidateFilter();
}

bool UsersProxyModel::filterAcceptsRow(int sourceRow,
                                      const QModelIndex& sourceParent) const
{
    if (m_filterText.isEmpty())
        return true;

    for (int col = 0; col < 3; ++col)
    {
        QModelIndex idx = sourceModel()->index(sourceRow, col, sourceParent);
        QString value = sourceModel()->data(idx, Qt::DisplayRole).toString();

        if (value.contains(m_filterText, Qt::CaseInsensitive))
            return true;
    }
    return false;
}
