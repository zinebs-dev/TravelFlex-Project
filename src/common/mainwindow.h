#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

class Database; // Ajoute le forward declaration
class DeletePackage;
class AddPackage;
class HomeAdmin;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    bool initDatabase();

private:
    QSqlDatabase m_db;
    Database* m_database; // Ajoute le membre partagé
    DeletePackage* m_deletePackage;// Si besoin d'accéder à la page de suppression
    AddPackage* m_addPackage;
    HomeAdmin* m_homeAdmin;
};

#endif // MAINWINDOW_H
