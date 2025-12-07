#pragma once

#include <QDialog>
#include "src/dao/LocationsDAO.h"

namespace Ui {
class NewLocationDialog;
}

class NewLocationDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewLocationDialog(LocationsDAO& locationsDAO,
                               QWidget *parent = nullptr);
    ~NewLocationDialog();

    void loadLocation(int locationId);

private slots:
    void onSave();

private:
    bool validateInput(QString& errorMessage) const;

private:
    Ui::NewLocationDialog *ui;
    LocationsDAO& m_locationsDAO;
    int m_locationId{-1};
};
