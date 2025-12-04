#pragma once

#include <QMainWindow>
#include "src/dao/UserDAO.h"

struct PageInfo
{
    QWidget* page;
    QString title;
};

QT_BEGIN_NAMESPACE
namespace Ui {
class Dashboard;
}
QT_END_NAMESPACE

class Dashboard : public QMainWindow
{
    Q_OBJECT

public:
    Dashboard(UserDAO& userDAO, QWidget *parent = nullptr);
    ~Dashboard();

private:
    void registerPage(const QString& name, QWidget* page, const QString& pageTitle);
    void switchToPage(const QString& name);
    void setupConnections();

private:
    Ui::Dashboard *ui;
    UserDAO& m_userDAO;
    QMap<QString, PageInfo> m_pages;
};
