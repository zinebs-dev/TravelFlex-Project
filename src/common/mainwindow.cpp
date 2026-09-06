#include "mainwindow.h"
#include "database.h"
#include "deletepackage.h"
#include "homeadmin.h"
#include "addpackage.h"
#include <QSqlError>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSql>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
    m_database(nullptr),
    m_deletePackage(nullptr)
{
    // Initialisation de la base de données (QSqlDatabase pour le modèle)
    if (!initDatabase()) {
        QMessageBox::critical(this, "Erreur", "Échec de l'initialisation de la base de données");
    }

    // Création de l'instance Database partagée
    m_database = new Database("C:/Users/MEGA/OneDrive/Documents/Bureau/Projet_C++/database_tourist_management/datab.db", this);

    m_homeAdmin = new HomeAdmin(m_database, this);
    setCentralWidget(m_homeAdmin);
    m_addPackage = new AddPackage(m_database, this);
    setCentralWidget( m_addPackage);
    // Exemple : création de la page de suppression en transmettant le Database*
    m_deletePackage = new DeletePackage(m_database, this);
    // Affichage de la page de suppression pour test (à adapter selon ton système de navigation)
    setCentralWidget(m_deletePackage);
}

MainWindow::~MainWindow()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
    // m_database est supprimé automatiquement car QObject parenté à MainWindow
    // m_deletePackage aussi
}

bool MainWindow::initDatabase()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", "MyConnection"); // Important d'utiliser MyConnection partout
    m_db.setDatabaseName("C:/Users/MEGA/OneDrive/Documents/Bureau/Projet_C++/database_tourist_management/datab.db");

    if (!m_db.open()) {
        QMessageBox::critical(this, "Erreur",
                              "Impossible d'ouvrir la base: " + m_db.lastError().text());
        return false;
    } else {
        QMessageBox::information(this, "Succès",
                                 "Base de données ouverte avec succès!\n"
                                 "Chemin: " + m_db.databaseName());
    }
    return true;
}
