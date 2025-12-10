#include "ReturnPage.h"
#include "ui_ReturnPage.h"

ReturnPage::ReturnPage(ItemsDAO& itemsDAO,
                       CheckoutDAO& checkoutDAO,
                       UsersDAO& usersDAO,
                       QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ReturnPage)
    , m_itemsDAO{itemsDAO}
    , m_checkoutDAO{checkoutDAO}
    , m_usersDAO{usersDAO}
{
    ui->setupUi(this);

    // TODO: Set model for the TableView

    resetUI();

    setupConnections();
}

ReturnPage::~ReturnPage()
{
    delete ui;
}

void ReturnPage::onUserBarcodeEntered()
{
    // TODO
}

void ReturnPage::onAddItemEntered()
{
    // TODO
}

void ReturnPage::onConfirmReturnClicked()
{
    // TODO
}

void ReturnPage::onCancelClicked()
{
    // TODO
}

void ReturnPage::resetUI()
{
    m_currentUserId = -1;
    m_returnItems.clear();

    // TODO: Refresh TableView model

    ui->userBarcodeLineEdit->clear();
    ui->itemBarcodeLineEdit->clear();
    ui->userNameLabel->setText("");

    ui->itemBarcodeLineEdit->setEnabled(false);
    ui->confirmButton->setEnabled(false);
    ui->itemHintLabel->setHidden(true);
}

void ReturnPage::setupConnections()
{
    connect(ui->searchUserButton, &QPushButton::clicked, this, &ReturnPage::onUserBarcodeEntered);
    connect(ui->userBarcodeLineEdit, &QLineEdit::returnPressed, this, &ReturnPage::onUserBarcodeEntered);
    connect(ui->searchItemButton, &QPushButton::clicked, &ReturnPage::onAddItemEntered);
    connect(ui->itemBarcodeLineEdit,  &QLineEdit::returnPressed, this, &ReturnPage::onAddItemEntered);
    connect(ui->confirmButton, &QPushButton::clicked, this, &ReturnPage::onConfirmReturnClicked);
    connect(ui->cancelButton, &QPushButton::clicked, this, &ReturnPage::onCancelClicked);
}

void ReturnPage::activateItemEntry()
{
    ui->itemBarcodeLineEdit->setEnabled(true);
    ui->itemBarcodeLineEdit->setFocus();
    ui->itemHintLabel->setHidden(false);
}

void ReturnPage::refreshReturnTableView()
{
    // TODO
}
