#ifndef HOMETOURIST_H
#define HOMETOURIST_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QStackedWidget>
#include <QTableWidget>
#include <QHeaderView>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QHBoxLayout>
#include "database.h"
#include "logintourist.h"
#include "reserverpackage.h"
#include "paymentpackage.h"
#include "profilepage.h"
class LoginTourist;
class PackageSelection;
class HomeTourist: public QWidget
{
    Q_OBJECT

public:
    explicit HomeTourist(Database* db, int touristId, QWidget *parent = nullptr);
    void updateStatistics();
    virtual ~HomeTourist();

signals:
    void userLoggedOut();

private slots:
    void logout();
    void openReservation();

public slots:
     void refresh();
    void refreshStats();

private:
    QPushButton* createSidebarButton(const QString &text, const QString &iconPath);
    QWidget* createHomePage();
    QWidget* createStatCard(const QString &title, QLabel* valueLabel,
                            const QString &color);
    QWidget* createActivityHistory();

    void loadRecentActivity();

    QLabel *userCountLabel;
    QLabel *packageCountLabel;
    QLabel *bookingCountLabel;
    QLabel *revenueLabel;
    QTableWidget *activityTable;
    QStackedWidget *stackedWidget;
    QLabel *pageTitle;
    QSqlDatabase database;
    LoginTourist *m_touristlogin = nullptr;
    Database* m_db;
    ReserverPackage* m_reserverPackage;
    PaymentPackage* m_paymentPackage;
    int m_touristId;
    QTableWidget* m_historyTable = nullptr;
    QLabel* nbResaLabel;
    QLabel* nbPackagesLabel;

};

#endif // HOMETOURIST_H
