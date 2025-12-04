#pragma once

#include <QObject>
#include <QSortFilterProxyModel>

class UserProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit UserProxyModel(QObject* parent = nullptr);

    void setFilterText(const QString& text);

protected:
    bool filterAcceptsRow(int sourceRow,
                          const QModelIndex& sourceParent) const override;

private:
    QString m_filterText;
};
