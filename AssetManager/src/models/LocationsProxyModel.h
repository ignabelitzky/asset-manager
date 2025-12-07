#pragma once

#include <QObject>
#include <QSortFilterProxyModel>

class LocationsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit LocationsProxyModel(QObject *parent = nullptr);

    void setFilterText(const QString& text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const;

private:
    QString m_filterText;
};
