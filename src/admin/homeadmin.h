#ifndef HOMEADMIN_H
#define HOMEADMIN_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QSqlDatabase>
#include <QMessageBox>
#include "addpackage.h"
#include "deletepackage.h"
#include "database.h" // Ajouté
#include "adminlogin.h"
#include "editpackage.h"
class AdminLogin;

class HomeAdmin : public QWidget
{
    Q_OBJECT

public:
    explicit HomeAdmin(Database* db, QWidget *parent = nullptr); // Modifié
    void updateStatistics();

signals:
    void userLoggedOut();

private slots:
    void showAddPackagePage();
    void showDeletePackagePage();
    void showEditPackagePage();
    void logout();

public slots:
    void refreshStats();

private:
    QPushButton* createSidebarButton(const QString &text, const QString &iconPath);
    QWidget* createHomePage();
    QWidget* createStatCard(const QString &title, QLabel* valueLabel,
                            const QString &color);
    void loadRecentActivity();

    // homeadmin.h
private:
    QLabel* userCountLabel = nullptr;
    QLabel* packageCountLabel = nullptr;
    QLabel* bookingCountLabel = nullptr;
    QLabel *revenueLabel;
    QTableWidget *activityTable;
    QStackedWidget *stackedWidget;
    QLabel *pageTitle;

    AddPackage *addPackagePage;
    DeletePackage *DeletePackagePage;
    EditPackage *EditPackagePage;
    QSqlDatabase database;
    AdminLogin *m_adminlogin = nullptr;
    Database* m_db;

};

#endif // HOMEADMIN_H
