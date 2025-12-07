#include "LocationsProxyModel.h"

LocationsProxyModel::LocationsProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
    setFilterCaseSensitivity(Qt::CaseInsensitive);
    setSortCaseSensitivity(Qt::CaseInsensitive);
    setDynamicSortFilter(true);
}

void LocationsProxyModel::setFilterText(const QString& text)
{
    setFilterFixedString(text);
    invalidateFilter();
}

bool LocationsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    if (filterRegularExpression().pattern().isEmpty())
        return true;

    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    QString value = sourceModel()->data(index, Qt::DisplayRole).toString();

    if (value.contains(m_filterText, Qt::CaseInsensitive))
        return true;

    return false;
}
