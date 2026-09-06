#ifndef REGISTERTOURIST_H
#define REGISTERTOURIST_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QDateEdit>
#include "database.h"
#include "welcome.h"
#include "welcometourist.h"

class Welcome;
class WelcomeTourist;

class RegisterTourist : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterTourist(Database *db, QWidget *parent = nullptr);
    ~RegisterTourist();

signals:

private slots:
    void onRegisterClicked();
    void onBackClicked();

private:
    QLabel *titleLabel;

    QLineEdit *firstnameEdit;
    QLineEdit *lastnameEdit;
    QLineEdit *usernameEdit;
    QLineEdit *emailEdit;
    QLineEdit *phoneEdit;
    QLineEdit *passportNumberEdit;
    QLineEdit *nationalityEdit;
    QDateEdit *dateOfBirthEdit;
    QLineEdit *passwordEdit;
    QLineEdit *confirmPasswordEdit;

    QPushButton *registerButton;
    QPushButton *backButton;
    Database *db;

    bool registerTourist(
        const QString &firstname,
        const QString &lastname,
        const QString &username,
        const QString &email,
        const QString &phone,
        const QString &passportNumber,
        const QString &nationality,
        const QDate &dateOfBirth,
        const QString &password
        );
};

#endif // REGISTERTOURIST_H
