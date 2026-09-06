#ifndef ADMINLOGIN_H
#define ADMINLOGIN_H

#include "homeadmin.h"

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>
#include "database.h"
#include "addpackage.h"
#include "deletepackage.h"
#include "editpackage.h"

class HomeAdmin;
class AdminLogin : public QWidget
{
    Q_OBJECT

public:
    explicit AdminLogin(Database* db, QWidget *parent = nullptr);
    ~AdminLogin() override;

private slots:
    void attemptLogin();

private:

    QLabel *titleLabel;
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QCheckBox *showPasswordCheckBox;
    QPushButton *loginButton;
    QPushButton *backButton;
    HomeAdmin *m_homeAdmin = nullptr;
    Database* m_db;

};

#endif // ADMINLOGIN_H
