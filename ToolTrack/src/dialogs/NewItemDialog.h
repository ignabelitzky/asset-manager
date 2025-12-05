#pragma once

#include <QDialog>
#include "src/dao/ItemsDAO.h"
#include "src/dao/ItemTypesDAO.h"
#include "src/dao/ItemStatesDAO.h"
#include "src/dao/OwnerTypesDAO.h"
#include "src/dao/PersonsDAO.h"
#include "src/dao/InstitutionsDAO.h"
#include "src/dao/LocationsDAO.h"
#include "src/models/Item.h"

namespace Ui {
class NewItemDialog;
}

class NewItemDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewItemDialog(ItemsDAO& itemsDAO,
                           ItemTypesDAO& itemTypesDAO,
                           ItemStatesDAO& itemStatesDAO,
                           OwnerTypesDAO& ownerTypesDAO,
                           PersonsDAO& personsDAO,
                           InstitutionsDAO& institutionsDAO,
                           LocationsDAO& locationsDAO,
                           QWidget *parent = nullptr);
    ~NewItemDialog();

    void setItemToEdit(const Item& item);
    Item getItem() const;

private slots:
    void onOwnerTypeChanged(int index);
    void onSaveClicked();
    void onCancelClicked();

private:
    void loadCombos();
    void loadOwnersForType(int ownerTypeId);
    bool validateFields();

private:
    Ui::NewItemDialog *ui;

    ItemsDAO& m_itemsDAO;
    ItemTypesDAO& m_itemTypesDAO;
    ItemStatesDAO& m_itemStatesDAO;
    OwnerTypesDAO& m_ownerTypesDAO;
    PersonsDAO& m_personsDAO;
    InstitutionsDAO& m_institutionsDAO;
    LocationsDAO& m_locationsDAO;

    bool m_isEditMode = false;
    Item m_originalItem;
};
