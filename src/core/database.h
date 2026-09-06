#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDate>
#include <QVariantMap>
#include <QList>

class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(const QString &dbPath, QObject *parent = nullptr);
    ~Database();

    bool connect();
    void disconnectFromDatabase();
    bool isConnected() const;
    QSqlError lastError() const;

    // Authentification
    bool checkAdminCredentials(const QString &username, const QString &password);
    bool checkTouristCredentials(const QString &username, const QString &password);
    int getTouristId(const QString& username);



    // Gestion packages
    bool addPackage(const QString& name, const QString& destination,
                    const QString& description, double price, int duration);
    bool deletePackage(int packageId);
    QVariantMap getPackageByName(const QString& name);
    bool updatePackage(int id, const QString& name, const QString& dest,
                       const QString& desc, double price, const QString& dur);
    QList<QVariantMap> getAllPackages();

    // Gestion réservation
    bool addBooking(int touristId, int packageId);
    bool reservationExists(int touristId, int packageId);

    // Gestion touristes
    bool emailExists(const QString& email);
    bool usernameExists(const QString& username);
    bool registerTourist(const QString &Firstname,
                         const QString &Lastname,
                         const QString &username,
                         const QString &email,
                         const QString &phone,
                         const QString &passportNumber,
                         const QString &nationality,
                         const QDate &dateOfBirth,
                         const QString &password);


    //les cartes
    int getTouristCount();
    int getPackageCount();
    int getBookingCount();
    int getAvailablePackageCount();
    int getTouristBookingCount(int touristId);
    QString getTouristName(int touristId);

    //lister les reservation et payement
    bool deleteBooking(const QString& bookingId);
    QList<QList<QString>> getUnpaidReservationsForTourist(int touristId);
    bool setBookingPaid(const QString& bookingId);
    QList<QVariantMap> getRecentBookingsForTourist(int touristId, int limit = 5);

    //info personnelles
    QMap<QString, QString> getTouristInfo(int touristId);
    QList<QVariantMap> getLastBookings(int limit = 5);

private:

    QString m_dbPath;
    QSqlDatabase m_db;
    QSqlError m_lastError;

};

#endif // DATABASE_H
