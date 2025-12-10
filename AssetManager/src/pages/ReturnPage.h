#pragma once

#include <QWidget>
#include "src/dao/ItemsDAO.h"
#include "src/dao/CheckoutDAO.h"
#include "src/dao/UsersDAO.h"

struct ReturnEntry {
    int itemId;
    QString name;
    int quantity{1};
};

namespace Ui {
class ReturnPage;
}

class ReturnPage : public QWidget
{
    Q_OBJECT

public:
    explicit ReturnPage(ItemsDAO& itemsDAO,
                        CheckoutDAO& checkoutDAO,
                        UsersDAO& usersDAO,
                        QWidget *parent = nullptr);
    ~ReturnPage();

private slots:
    void onUserBarcodeEntered();
    void onAddItemEntered();
    void onConfirmReturnClicked();
    void onCancelClicked();

private:
    void resetUI();
    void setupConnections();
    void activateItemEntry();
    void refreshReturnTableView();

private:
    Ui::ReturnPage *ui;
    ReturnTableModel* m_returnModel;
    ItemsDAO& m_itemsDAO;
    CheckoutDAO& m_checkoutDAO;
    UsersDAO& m_usersDAO;
    int m_currentUserId = -1;
    QVector<ReturnEntry> m_returnItems;
};
