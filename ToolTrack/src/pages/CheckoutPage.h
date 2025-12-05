#pragma once

#include <QWidget>
#include <QString>
#include <QVector>

#include "src/dao/ItemsDAO.h"
#include "src/dao/CheckoutDAO.h"
#include "src/dao/UsersDAO.h"
#include "src/models/CartTableModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class CheckoutPage; }
QT_END_NAMESPACE

class CheckoutPage : public QWidget
{
    Q_OBJECT

public:
    CheckoutPage(ItemsDAO& itemsDAO,
                 CheckoutDAO& checkoutDAO,
                 UsersDAO& usersDAO,
                 QWidget* parent = nullptr);

    ~CheckoutPage();

private slots:
    void onUserBarcodeEntered();
    void onAddItemEntered();
    void onConfirmCheckout();

private:
    void resetUI();
    void activateItemEntry();
    void refreshCartModel();

private:
    Ui::CheckoutPage* ui;
    CartTableModel* m_cartModel;
    ItemsDAO& m_itemsDAO;
    CheckoutDAO& m_checkoutDAO;
    UsersDAO& m_usersDAO;
    int m_currentUserId = -1;

    struct CheckoutEntry {
        int itemId;
        QString name;
        int quantity{1};
    };

    QVector<CheckoutEntry> m_checkoutItems;
};
