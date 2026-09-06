#ifndef RESERVERPACKAGE_H
#define RESERVERPACKAGE_H

#include <QWidget>
#include <QList>
#include <QVariantMap>
#include "database.h"

class QVBoxLayout;

class ReserverPackage : public QWidget
{
    Q_OBJECT
public:
    explicit ReserverPackage(Database* db, int touristId, QWidget* parent = nullptr);

private slots:
    void onReserveClicked();


signals:
    void bookingAdded();

private:
    void loadPackages();

    Database* m_db;
    int m_touristId;
    QVBoxLayout* m_layout;
    QList<int> m_packageIds; // Pour garder le mapping boutons/packages
};

#endif // RESERVERPACKAGE_H
