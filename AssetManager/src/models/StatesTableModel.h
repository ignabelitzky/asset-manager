#pragma once

#include <QAbstractTableModel>
#include <QObject>
#include "src/dao/ItemStatesDAO.h"

class StatesTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit StatesTableModel(ItemStatesDAO& itemStatesDAO,
                              QObject *parent = nullptr);
    ~StatesTableModel() = default;

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
    const ItemState& getState(int row) const;

private:
    ItemStatesDAO& m_itemStatesDAO;
    QVector<ItemState> m_states;
};
