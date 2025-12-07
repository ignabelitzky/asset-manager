#pragma once

#include <QAbstractTableModel>
#include <QObject>
#include "src/dao/LocationsDAO.h"

class LocationsTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit LocationsTableModel(LocationsDAO& locationsDAO,
                                 QObject *parent = nullptr);
    ~LocationsTableModel() = default;

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
    const Location& getLocation(int row) const;

private:
    LocationsDAO& m_locationsDAO;
    QVector<Location> m_locations;
};
