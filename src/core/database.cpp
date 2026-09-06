#include "database.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QMessageBox>

Database::Database(const QString &dbPath, QObject *parent)
    : QObject(parent), m_dbPath(dbPath)
{
    connect();
}

Database::~Database()
{
    disconnectFromDatabase();
}

bool Database::connect()
{
    if (m_db.isOpen()) {
        return true;
    }

    if (QSqlDatabase::contains("MyConnection")) {
        m_db = QSqlDatabase::database("MyConnection");
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "MyConnection");
        m_db.setDatabaseName(m_dbPath);
    }

    if (!m_db.open()) {
        m_lastError = m_db.lastError();
        qDebug() << "Erreur de connexion:" << m_lastError.text();
        return false;
    }

    qDebug() << "Connexion réussie à la base de données";
    return true;
}

void Database::disconnectFromDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
        qDebug() << "Déconnexion de la base de données";
    }
}

bool Database::isConnected() const
{
    return m_db.isOpen();
}

QSqlError Database::lastError() const
{
    return m_lastError;
}

bool Database::checkAdminCredentials(const QString &username, const QString &password)
{
    if (!isConnected() && !connect()) {
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT password FROM admin WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        m_lastError = query.lastError();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        return password == storedPassword;
    }

    return false;
}

bool Database::addPackage(const QString& name, const QString& destination,
                          const QString& description, double price, int duration)
{
    if (!isConnected()) {
        if (!connect()) {
            return false;
        }
    }

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO package (name, destination, description, price, duration) "
                  "VALUES (:name, :destination, :description, :price, :duration)");
    query.bindValue(":name", name);
    query.bindValue(":destination", destination);
    query.bindValue(":description", description);
    query.bindValue(":price", price);
    query.bindValue(":duration", duration);

    if (!query.exec()) {
        m_lastError = query.lastError();
        qDebug() << "Error adding package:" << m_lastError.text();
        return false;
    }
    return true;
}

// Suppression d'un package
bool Database::deletePackage(int packageId)
{
    if (!isConnected()) {
        if (!connect()) {
            return false;
        }
    }

    QSqlQuery query(m_db);
    query.prepare("DELETE FROM package WHERE id = :id");
    query.bindValue(":id", packageId);

    if (!query.exec()) {
        m_lastError = query.lastError();
        qDebug() << "Error deleting package:" << m_lastError.text();
        return false;
    }
    return true;
}


QVariantMap Database::getPackageByName(const QString& name)
{
    QVariantMap package;
    QSqlQuery query(m_db);

    // Recherche insensible à la casse et ignore les accents de base
    query.prepare("SELECT id, name,destination, description, price,duration FROM package WHERE name LIKE :name COLLATE NOCASE");
    query.bindValue(":name", "%" + name + "%");

    if (query.exec() && query.next()) {
        package["id"] = query.value("id");
        package["name"] = query.value("name");
        package["destination"] = query.value("destination");
        package["description"] = query.value("description");
        package["price"] = query.value("price");
        package["duration"] = query.value("duration");
    }
    return package;
}

bool Database::updatePackage(int id, const QString& name,const QString& dest ,const QString& desc,double price,const QString& dur)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE package SET name = :name , destination= :dest, description = :desc, price = :price, duration= :dur WHERE id = :id");
    query.bindValue(":name", name);
    query.bindValue(":dest", dest);
    query.bindValue(":desc", desc);
    query.bindValue(":price", price);
    query.bindValue(":dur", dur);
    query.bindValue(":id", id);
    if (!query.exec()) {
        qWarning() << "Échec updatePackage:" << query.lastError().text();
        return false;
    }
    return query.numRowsAffected() > 0;
}

bool Database::emailExists(const QString& email)
{
    if (!isConnected() && !connect()) return false;
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM Tourist WHERE email = :email");
    query.bindValue(":email", email);
    if (!query.exec() || !query.next()) return false;
    return query.value(0).toInt() > 0;
}

bool Database::usernameExists(const QString& username)
{
    if (!isConnected() && !connect()) return false;
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM Tourist WHERE username = :username");
    query.bindValue(":username", username);
    if (!query.exec() || !query.next()) return false;
    return query.value(0).toInt() > 0;
}

bool Database::registerTourist(const QString &Firstname,
                               const QString &Lastname,
                               const QString &username,
                               const QString &email,
                               const QString &phone,
                               const QString &passportNumber,
                               const QString &nationality,
                               const QDate &dateOfBirth,
                               const QString &password)
{
    if (!isConnected() && !connect()) {
        qDebug() << "Echec de connexion à la base pour inscription";
        m_lastError = m_db.lastError();
        return false;
    }

    if (emailExists(email)) {
        qDebug() << "Email déjà utilisé";
        m_lastError = QSqlError("Cet email est déjà utilisé", "", QSqlError::UnknownError);
        return false;
    }
    if (usernameExists(username)) {
        qDebug() << "Username déjà utilisé";
        m_lastError = QSqlError("Ce nom d'utilisateur est déjà utilisé", "", QSqlError::UnknownError);
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("INSERT INTO Tourist (Firstname, Lastname, username, email, phone, passportNumber, nationality, dateOfBirth, password)"
                  " VALUES (:firstname, :lastname, :username, :email, :phone, :passportNumber, :nationality, :dateOfBirth, :password)");
    query.bindValue(":firstname", Firstname);
    query.bindValue(":lastname", Lastname);
    query.bindValue(":username", username);
    query.bindValue(":email", email);
    query.bindValue(":phone", phone);
    query.bindValue(":passportNumber", passportNumber);
    query.bindValue(":nationality", nationality);
    query.bindValue(":dateOfBirth", dateOfBirth.toString("yyyy-MM-dd"));
    query.bindValue(":password", password);

    if (!query.exec()) {
        m_lastError = query.lastError();
        qDebug() << "Erreur insertion touriste:" << m_lastError.text();
        return false;
    }
    m_lastError = QSqlError(); // reset
    return true;
}

bool Database::checkTouristCredentials(const QString &username, const QString &password)
{
    if (!isConnected() && !connect()) {
        return false;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT password FROM tourist WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        m_lastError = query.lastError();
        return false;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        return password == storedPassword;
    }

    return false;
}

QList<QVariantMap> Database::getAllPackages()
{
    QList<QVariantMap> list;
    QSqlQuery query(m_db);
    if (query.exec("SELECT id, name, destination, description, price, duration FROM package")) {
        while (query.next()) {
            QVariantMap pkg;
            pkg["id"] = query.value("id");
            pkg["name"] = query.value("name");
            pkg["destination"] = query.value("destination");
            pkg["description"] = query.value("description");
            pkg["price"] = query.value("price");
            pkg["duration"] = query.value("duration");
            list.append(pkg);
        }
    }
    return list;
}

bool Database::addBooking(int touristId, int packageId)
{
    if (!isConnected() && !connect()) {
        return false;
    }
    QSqlQuery query(m_db);
    query.prepare("INSERT INTO booking (touristId, packageId, bookingDate) VALUES (:tid, :pid, :date)");
    query.bindValue(":tid", touristId);
    query.bindValue(":pid", packageId);
    query.bindValue(":date", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));    if (!query.exec()) {
        qDebug() << "Erreur réservation:" << query.lastError().text();
        return false;
    }
    return true;
}
int Database::getTouristId(const QString& username)
{
    if (!isConnected() && !connect()) {
        return -1;
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT id FROM Tourist WHERE username = :username");
    query.bindValue(":username", username);

    if (!query.exec()) {
        m_lastError = query.lastError();
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }

    return -1;
}

bool Database::reservationExists(int touristId, int packageId) {
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM booking WHERE touristId = :touristId AND packageId = :packageId");
    query.bindValue(":touristId", touristId);
    query.bindValue(":packageId", packageId);
    if (!query.exec() || !query.next())
        return false;
    return query.value(0).toInt() > 0;
}
// database.cpp

int Database::getTouristCount() {
    QSqlQuery query(m_db);
    if (query.exec("SELECT COUNT(*) FROM Tourist") && query.next())
        return query.value(0).toInt();
    return 0;
}

int Database::getPackageCount() {
    QSqlQuery query(m_db);
    if (query.exec("SELECT COUNT(*) FROM Package") && query.next())
        return query.value(0).toInt();
    return 0;
}

int Database::getBookingCount() {
    QSqlQuery query(m_db);
    if (query.exec("SELECT COUNT(*) FROM Booking") && query.next())
        return query.value(0).toInt();
    return 0;
}
QString Database::getTouristName(int touristId) {
    QSqlQuery query(m_db);
    query.prepare("SELECT Firstname FROM Tourist WHERE id = :id");
    query.bindValue(":id", touristId);
    if (query.exec() && query.next())
        return query.value(0).toString();
    return QString();
}
int Database::getTouristBookingCount(int touristId) {
    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) FROM Booking WHERE touristId = :tid");
    query.bindValue(":tid", touristId);
    if (query.exec() && query.next())
        return query.value(0).toInt();
    return 0;
}
int Database::getAvailablePackageCount() {
    QSqlQuery query(m_db);
    if (query.exec("SELECT COUNT(*) FROM Package") && query.next())
        return query.value(0).toInt();
    return 0;
}

QList<QList<QString>> Database::getUnpaidReservationsForTourist(int touristId) {
    QList<QList<QString>> reservations;
    QSqlQuery query(m_db);
    query.prepare(
        "SELECT B.id, P.name, B.bookingDate , P.price "
        "FROM Booking B "
        "JOIN Package P ON B.packageId = P.id "
        "WHERE B.touristId = :tid AND B.paymentStatus = 'unpaid'"
        "ORDER BY datetime(B.bookingDate) DESC"
        );
    query.bindValue(":tid", touristId);
    if (query.exec()) {
        while (query.next()) {
            QList<QString> row;
            row << query.value(0).toString(); // id réservation
            row << query.value(1).toString(); // nom package
            row << query.value(2).toString(); // date
            row << query.value(3).toString(); // montant
            reservations.append(row);
        }
    }
    return reservations;
}
bool Database::setBookingPaid(const QString& bookingId) {
    QSqlQuery query(m_db);
    query.prepare("UPDATE Booking SET paymentStatus = 'paid' WHERE id = :id");
    query.bindValue(":id", bookingId);
    return query.exec();
}
bool Database::deleteBooking(const QString& bookingId) {
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Booking WHERE id = :id");
    query.bindValue(":id", bookingId);
    return query.exec();
}
QMap<QString, QString> Database::getTouristInfo(int touristId) {
    QMap<QString, QString> infos;
    QSqlQuery query(m_db);
    query.prepare("SELECT Firstname, Lastname, username, email, phone, passportNumber, nationality, dateOfBirth FROM Tourist WHERE id = :id");
    query.bindValue(":id", touristId);
    if (query.exec() && query.next()) {
        infos["Firstname"]      = query.value(0).toString();
        infos["Lastname"]       = query.value(1).toString();
        infos["username"]       = query.value(2).toString();
        infos["email"]          = query.value(3).toString();
        infos["phone"]          = query.value(4).toString();
        infos["passportNumber"] = query.value(5).toString();
        infos["nationality"]    = query.value(6).toString();
        infos["dateOfBirth"]    = query.value(7).toString();
    }
    return infos;
}
QList<QVariantMap> Database::getRecentBookingsForTourist(int touristId, int limit)
{
    QList<QVariantMap> result;
    QSqlQuery query(m_db);
    query.prepare(
        "SELECT B.bookingDate, P.name, B.paymentStatus "
        "FROM Booking B "
        "JOIN Package P ON B.packageId = P.id "
        "WHERE B.touristId = :tid "
        "ORDER BY datetime(B.bookingDate) DESC LIMIT :limit"
        );
    query.bindValue(":tid", touristId);
    query.bindValue(":limit", limit); // Attention: SQLite ne supporte pas bindValue pour LIMIT, voir plus bas!

    // Astuce pour SQLite : concaténer la limite dans la requête si bindValue ne marche pas
    QString sql = QString(
                      "SELECT B.bookingDate, P.name, B.paymentStatus "
                      "FROM Booking B "
                      "JOIN Package P ON B.packageId = P.id "
                      "WHERE B.touristId = :tid "
                      "ORDER BY B.bookingDate DESC LIMIT %1"
                      ).arg(limit);
    query.prepare(sql);
    query.bindValue(":tid", touristId);

    if (query.exec()) {
        while (query.next()) {
            QVariantMap row;
            row["bookingDate"] = query.value(0);
            row["packageName"] = query.value(1);
            row["paymentStatus"] = query.value(2);
            result.append(row);
        }
    }
    return result;
}
QList<QVariantMap> Database::getLastBookings(int limit)
{
    QList<QVariantMap> result;
    QString sql = QString(
                      "SELECT T.firstname || ' ' || T.lastname AS user, "
                      "P.name AS package, "
                      "B.bookingDate "
                      "FROM Booking B "
                      "JOIN Tourist T ON B.touristId = T.id "
                      "JOIN Package P ON B.packageId = P.id "
                      "ORDER BY datetime(B.bookingDate) DESC "
                      "LIMIT %1"
                      ).arg(limit);

    QSqlQuery query(m_db);
    if (query.exec(sql)) {
        while (query.next()) {
            QVariantMap row;
            row["user"] = query.value(0).toString();
            row["package"] = query.value(1).toString();
            row["date"] = query.value(2).toString();
            result.append(row);
        }
    }
    return result;
}
