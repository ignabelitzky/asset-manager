#include <QMessageBox>
#include <QDebug>
#include "CheckoutPage.h"
#include "ui_CheckoutPage.h"

CheckoutPage::CheckoutPage(ItemsDAO& itemsDAO,
                           CheckoutDAO& checkoutDAO,
                           UsersDAO& usersDAO,
                           QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::CheckoutPage)
    , m_itemsDAO(itemsDAO)
    , m_checkoutDAO(checkoutDAO)
    , m_usersDAO(usersDAO)
{
    ui->setupUi(this);

    m_cartModel = new CartTableModel(this);
    ui->cartTableView->setModel(m_cartModel);
    ui->cartTableView->horizontalHeader()->setStretchLastSection(true);
    ui->cartTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->cartTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    resetUI();

    connect(ui->userBarcodeLineEdit, &QLineEdit::returnPressed,
            this, &CheckoutPage::onUserBarcodeEntered);

    connect(ui->itemBarcodeLineEdit, &QLineEdit::returnPressed,
            this, &CheckoutPage::onAddItemEntered);

    connect(ui->confirmButton, &QPushButton::clicked,
            this, &CheckoutPage::onConfirmCheckout);
}

CheckoutPage::~CheckoutPage()
{
    delete ui;
}

void CheckoutPage::resetUI()
{
    m_currentUserId = -1;
    m_checkoutItems.clear();
    refreshCartModel();

    ui->userBarcodeLineEdit->clear();
    ui->itemBarcodeLineEdit->clear();
    ui->userNameLabel->setText("-");

    ui->itemBarcodeLineEdit->setEnabled(false);
    ui->confirmButton->setEnabled(false);
}

void CheckoutPage::activateItemEntry()
{
    ui->itemBarcodeLineEdit->setEnabled(true);
    ui->itemBarcodeLineEdit->setFocus();
}

void CheckoutPage::refreshCartModel()
{
    QMap<int, CartLine> aggregated;

    for (const auto& entry : m_checkoutItems)
    {
        if (!aggregated.contains(entry.itemId))
        {
            CartLine line;
            line.itemId = entry.itemId;
            line.name = entry.name;
            line.quantity = entry.quantity; // siempre 1
            aggregated.insert(entry.itemId, line);
        }
        else
        {
            aggregated[entry.itemId].quantity += entry.quantity;
        }
    }

    m_cartModel->setItems(aggregated.values().toVector());
}

void CheckoutPage::onUserBarcodeEntered()
{
    QString code = ui->userBarcodeLineEdit->text().trimmed();
    if (code.isEmpty())
        return;

    std::optional<User> user = m_usersDAO.getUserByBarcode(code);
    if (!user.has_value())
    {
        QMessageBox::warning(this, "Error", "Usuario no encontrado.");
        return;
    }

    m_currentUserId = user->id();

    QString fullName = QString("%1 %2").arg(user->firstName(), user->lastName());
    ui->userNameLabel->setText(fullName);

    activateItemEntry();
}

void CheckoutPage::onAddItemEntered()
{
    QString code = ui->itemBarcodeLineEdit->text().trimmed();
    if (code.isEmpty())
        return;

    std::optional<Item> itemOpt = m_itemsDAO.getByBarcode(code);
    if (!itemOpt.has_value())
    {
        QMessageBox::warning(this, "Error", "Ítem no encontrado.");
        return;
    }

    Item item = *itemOpt;

    // Contar cuántos hay ya en el carrito del mismo item
    int alreadyInCart = 0;
    for (const auto& e : m_checkoutItems)
        if (e.itemId == item.id())
            alreadyInCart += e.quantity;

    if (alreadyInCart >= item.availableStock())
    {
        QMessageBox::warning(this, "Sin stock", "No se pueden agregar más unidades, se alcanzó el stock disponible.");
        return;
    }

    // Agregar una nueva entrada por unidad
    m_checkoutItems.append({ item.id(), item.name(), 1 });

    refreshCartModel();
    ui->itemBarcodeLineEdit->clear();
    ui->confirmButton->setEnabled(true);
}

void CheckoutPage::onConfirmCheckout()
{
    if (m_currentUserId <= 0 || m_checkoutItems.empty())
        return;

    for (const auto& entry : m_checkoutItems)
    {
        for (int i = 0; i < entry.quantity; ++i)
        {
            if (!m_checkoutDAO.checkoutItem(m_currentUserId, entry.itemId))
            {
                QMessageBox::warning(this, "Error", "No se pudo procesar uno de los retiros.");
                return;
            }
        }
    }


    QMessageBox::information(this, "Éxito", "Checkout completado.");
    resetUI();
}
