#ifndef ADDPACKAGE_H
#define ADDPACKAGE_H

#include <QWidget>
#include <QLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QTextEdit>
#include <QLineEdit>
#include "database.h"

class QLineEdit;
class QTextEdit;
class QDoubleSpinBox;
class QSpinBox;
class QLabel;
class QFrame;

class AddPackage : public QWidget
{
    Q_OBJECT

public:
    explicit AddPackage(Database* db, QWidget *parent = nullptr);
    void setupUI();
    void clearForm();

private slots:
    void savePackage();

signals:
    void addpackDone();

private:
    Database* m_db;

    QLineEdit *nameEdit;
    QLineEdit *destinationEdit;
    QTextEdit *descriptionEdit;
    QDoubleSpinBox *priceSpinBox;
    QSpinBox *durationSpinBox;
};

#endif // ADDPACKAGE_H
