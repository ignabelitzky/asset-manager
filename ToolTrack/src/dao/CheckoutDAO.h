#pragma once

#include <QDateTime>
#include <optional>
#include <QSqlRecord>
#include <QMap>
#include "src/dao/ItemsDAO.h"

struct Checkout {
    int id;
    int userId;
    int itemId;
    QDateTime checkoutAt;
    std::optional<QDateTime> returnedAt;
};

class CheckoutDAO
{
public:
    CheckoutDAO(ItemsDAO& itemsDAO);
    ~CheckoutDAO() = default;

    bool checkoutItem(int userId, int itemId);
    bool returnItem(int checkoutId);

    std::optional<Checkout> getActiveCheckout(int itemId);

    QVector<Checkout> listActive();

    bool checkoutMultiple(int userId, const QMap<int, int>& items);

private:
    std::optional<Checkout> buildCheckout(const QSqlRecord& record) const;

private:
    ItemsDAO& m_itemsDAO;
};
