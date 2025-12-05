#include "ItemsProxyModel.h"

ItemsProxyModel::ItemsProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void ItemsProxyModel::setSearchText(const QString& text)
{
    m_searchText = text.trimmed();
    invalidateFilter();
}

bool ItemsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    if (m_searchText.isEmpty())
        return true;

    auto model = sourceModel();

    QModelIndex nameIndex = model->index(sourceRow, 0, sourceParent);
    QModelIndex barcodeIndex = model->index(sourceRow, 1, sourceParent);

    const QString name = model->data(nameIndex).toString();
    const QString barcode = model->data(barcodeIndex).toString();

    return name.contains(m_searchText, Qt::CaseInsensitive) ||
           barcode.contains(m_searchText, Qt::CaseInsensitive);
}
