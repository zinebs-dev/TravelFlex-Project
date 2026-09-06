#ifndef EDITPACKAGE_H
#define EDITPACKAGE_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QTextEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "database.h"

class EditPackage : public QWidget
{
    Q_OBJECT

public:
    explicit EditPackage(Database* db, QWidget *parent = nullptr);

private slots:
    void searchPackage();
    void saveChanges();

signals:
    void editpackDone();

private:
    Database* m_db;
    QLabel* titleLabel;
    QLabel* searchLabel;
    QLineEdit* searchLineEdit;
    QPushButton* searchButton;

    QLabel* nameLabel;
    QLineEdit* nameLineEdit;
    QLabel* destLabel;
    QLineEdit* destLineEdit;
    QLabel* descLabel;
    QTextEdit* descTextEdit;
    QLabel* priceLabel;
    QLineEdit* priceLineEdit;
    QLabel* durLabel;
    QLineEdit* durLineEdit;

    QPushButton* saveButton;

    int currentPackageId = -1;
    void clearFields();
    void setFieldsEnabled(bool enabled);
};

#endif // EDITPACKAGE_H
