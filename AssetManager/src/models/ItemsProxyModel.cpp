#include "ItemsProxyModel.h"

ItemsProxyModel::ItemsProxyModel(QObject *parent)
    : QSortFilterProxyModel{parent}
    , m_filterAvailableStock(false)
{
    setDynamicSortFilter(true);
    setFilterCaseSensitivity(Qt::CaseInsensitive);
}

void ItemsProxyModel::setSearchText(const QString& text)
{
    m_searchText = text.trimmed();
    invalidateFilter();
}

void ItemsProxyModel::setAvailableStockFilter(bool enabled)
{
    m_filterAvailableStock = enabled;
    invalidateFilter();
}

bool ItemsProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const
{
    auto model = sourceModel();
    if (!model)
        return false;

    // Search text filter
    if (!m_searchText.isEmpty())
    {
        QModelIndex nameIndex = model->index(sourceRow, 0, sourceParent);
        QModelIndex barcodeIndex = model->index(sourceRow, 1, sourceParent);

        const QString name = model->data(nameIndex).toString();
        const QString barcode = model->data(barcodeIndex).toString();

        if (!name.contains(m_searchText, Qt::CaseInsensitive) &&
            !barcode.contains(m_searchText, Qt::CaseInsensitive))
        {
            return false;
        }
    }

    // Available stock filter
    if (m_filterAvailableStock)
    {
        QModelIndex availableStockIndex = model->index(sourceRow, 5, sourceParent);
        int availableStock = model->data(availableStockIndex).toInt();
        if (availableStock <= 0)
            return false;
    }

    return true;
}
