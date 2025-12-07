#pragma once

#include <QDialog>
#include "src/dao/ItemTypesDAO.h"

namespace Ui {
class NewTypeDialog;
}

class NewTypeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewTypeDialog(ItemTypesDAO& itemTypesDAO,
                           QWidget *parent = nullptr);
    ~NewTypeDialog();

    void loadType(int typeId);

private slots:
    void onSave();

private:
    bool validateInput(QString& errorMessage) const;

private:
    Ui::NewTypeDialog *ui;
    ItemTypesDAO& m_itemTypesDAO;
    int m_typeId{-1};
};
