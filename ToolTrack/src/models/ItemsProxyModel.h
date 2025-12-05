#pragma once

#include <QObject>
#include <QSortFilterProxyModel>
#include <QString>

class ItemsProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit ItemsProxyModel(QObject *parent = nullptr);
    ~ItemsProxyModel() = default;

    void setSearchText(const QString& text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex& sourceParent) const override;

private:
    QString m_searchText;
};
