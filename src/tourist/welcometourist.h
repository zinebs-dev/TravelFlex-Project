#ifndef WELCOMETOURIST_H
#define WELCOMETOURIST_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "registertourist.h"
#include "logintourist.h"
class RegisterTourist;

class WelcomeTourist : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomeTourist(Database* db, QWidget *parent = nullptr);
    ~WelcomeTourist();

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onBackClicked();

private:
    QLabel *titleLabel;
    QPushButton *loginButton;
    QPushButton *registerButton;
    QPushButton *backButton;
    Database *m_db;
};

#endif // WELCOMETOURIST_H
