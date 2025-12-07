#pragma once

#include <QAbstractTableModel>
#include <QObject>
#include "src/dao/ItemTypesDAO.h"

class TypesTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit TypesTableModel(ItemTypesDAO& itemTypesDAO,
                             QObject *parent = nullptr);
    ~TypesTableModel() = default;

    enum {
        Name,
        COLUMN_COUNT
    };

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    // Custom actions
    void refresh();
    const ItemType& getType(int row) const;

private:
    ItemTypesDAO& m_itemTypesDAO;
    QVector<ItemType> m_types;
};
