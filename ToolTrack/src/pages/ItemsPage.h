#pragma once

#include <QWidget>
#include <QItemSelection>
#include "src/dao/ItemsDAO.h"
#include "src/models/ItemsTableModel.h"
#include "src/models/ItemsProxyModel.h"

namespace Ui {
class ItemsPage;
}

class ItemsPage : public QWidget
{
    Q_OBJECT

public:
    explicit ItemsPage(ItemsDAO& itemsDAO,
                       ItemTypesDAO& itemTypesDAO,
                       ItemStatesDAO& itemStatesDAO,
                       OwnerTypesDAO& ownerTypesDAO,
                       PersonsDAO& personsDAO,
                       InstitutionsDAO& institutionsDAO,
                       LocationsDAO& locationsDAO,
                       QWidget *parent = nullptr);
    ~ItemsPage();

private slots:
    void onSelectionChanged(const QItemSelection& selected,
                            const QItemSelection& deselected);
    void onNewItemClicked();
    void onEditItemClicked();
    void onDeleteItemClicked();
    void onRefreshClicked();

private:
    void setupItemsTable();
    void setupConnections();
    void reconnectSelectionModel();
    int getSelectedItemId() const;

private:
    Ui::ItemsPage *ui;
    ItemsDAO& m_itemsDAO;
    ItemTypesDAO& m_itemTypesDAO;
    ItemStatesDAO& m_itemStatesDAO;
    OwnerTypesDAO& m_ownerTypesDAO;
    PersonsDAO& m_personsDAO;
    InstitutionsDAO& m_institutionsDAO;
    LocationsDAO& m_locationsDAO;
    ItemsTableModel* m_model;
    ItemsProxyModel* m_proxy;
};
