#pragma once

#include <QSortFilterProxyModel>

class ItemsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ItemsProxyModel(QObject* parent = nullptr);

    // Search text filter
    void setSearchText(const QString& text);

    // Filter only items with available stock > 0
    void setAvailableStockFilter(bool enabled);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    QString m_searchText;
    bool m_filterAvailableStock;
};
