#pragma once

#include <QWidget>
#include <QItemSelection>
#include "src/dao/ItemTypesDAO.h"
#include "src/dao/ItemStatesDAO.h"
#include "src/dao/LocationsDAO.h"
#include "src/models/TypesTableModel.h"
#include "src/models/StatesTableModel.h"
#include "src/models/LocationsTableModel.h"
#include "src/models/TypesProxyModel.h"
#include "src/models/StatesProxyModel.h"
#include "src/models/LocationsProxyModel.h"

namespace Ui {
class MetadataPage;
}

class MetadataPage : public QWidget
{
    Q_OBJECT

public:
    explicit MetadataPage(ItemTypesDAO& itemTypesDAO,
                      ItemStatesDAO& itemStatesDAO,
                      LocationsDAO locationsDAO,
                      QWidget *parent = nullptr);
    ~MetadataPage();

private slots:
    void onTypesSelectionChanged(const QItemSelection& selected,
                                 const QItemSelection& deselected);
    void onNewTypeClicked();
    void onEditTypeClicked();
    void onDeleteTypeClicked();
    void onRefreshTypesClicked();

    void onStatesSelectionChanged(const QItemSelection& selected,
                                  const QItemSelection& deselected);
    void onNewStateClicked();
    void onEditStateClicked();
    void onDeleteStateClicked();
    void onRefreshStatesClicked();

    void onLocationsSelectionChanged(const QItemSelection& selected,
                                     const QItemSelection& deselected);
    void onNewLocationClicked();
    void onEditLocationClicked();
    void onDeleteLocationClicked();
    void onRefreshLocationsClicked();

private:
    void setupTypesTable();
    void setupStatesTable();
    void setupLocationsTable();
    void setupConnections();
    void reconnectTypesSelectionModel();
    void reconnectStatesSelectionModel();
    void reconnectLocationsSelectionModel();
    int getSelectedTypeId() const;
    int getSelectedStateId() const;
    int getSelectedLocationId() const;

private:
    Ui::MetadataPage *ui;
    ItemTypesDAO& m_itemTypesDAO;
    ItemStatesDAO& m_itemStatesDAO;
    LocationsDAO& m_locationsDAO;
    TypesTableModel* m_typesModel;
    StatesTableModel* m_statesModel;
    LocationsTableModel* m_locationsModel;
    TypesProxyModel* m_typesProxy;
    StatesProxyModel* m_statesProxy;
    LocationsProxyModel* m_locationsProxy;
};
