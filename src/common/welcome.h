#ifndef WELCOME_H
#define WELCOME_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>

class Database;

class Welcome : public QWidget
{
    Q_OBJECT

public:
    Welcome(Database* db, QWidget *parent = nullptr);
    ~Welcome();

private slots:
    void onAdminClicked();
    void onTouristClicked();

private:
    QLabel *titleLabel;
    QPushButton *adminButton;
    QPushButton *touristButton;
     Database* m_db;

};

#endif // WELCOME_H
