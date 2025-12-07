#pragma once

#include <QDialog>
#include "src/dao/ItemStatesDAO.h"

namespace Ui {
class NewStateDialog;
}

class NewStateDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewStateDialog(ItemStatesDAO& itemStatesDAO,
                            QWidget *parent = nullptr);
    ~NewStateDialog();

    void loadState(int stateId);

private slots:
    void onSave();

private:
    bool validateInput(QString& errorMessage) const;

private:
    Ui::NewStateDialog *ui;
    ItemStatesDAO& m_itemStatesDAO;
    int m_stateId{-1};
};
