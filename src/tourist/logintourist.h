#ifndef LOGINTOURIST_H
#define LOGINTOURIST_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QCheckBox>
#include <QMessageBox>
#include <QVBoxLayout>
#include "welcometourist.h"
#include "database.h"
#include "hometourist.h"
class HomeTourist;
class LoginTourist : public QWidget
{
    Q_OBJECT

public:
    explicit LoginTourist(Database* db, QWidget *parent = nullptr);
    ~LoginTourist() override;

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
    Database* m_db;
    HomeTourist* m_homeTourist = nullptr;

};

#endif // LOGINTOURIST_H
