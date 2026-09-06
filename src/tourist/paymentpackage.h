#ifndef PAYMENTPACKAGE_H
#define PAYMENTPACKAGE_H

#pragma once
#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include "database.h"

class PaymentPackage : public QWidget {
    Q_OBJECT
public:
    PaymentPackage(Database* db, int touristId, QWidget* parent = nullptr);
    void refresh();

signals:
    void paymentDone();

private:
    Database* m_db;
    int m_touristId;
    QLabel* title;
    QTableWidget* table;
};

#endif // PAYMENTPACKAGE_H
