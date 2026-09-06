#ifndef DELETEPACKAGE_H
#define DELETEPACKAGE_H

#include <QWidget>
#include <QSqlTableModel>

QT_BEGIN_NAMESPACE
class QTableView;
class QPushButton;
class QVBoxLayout;
class QLabel;
class QFrame;
QT_END_NAMESPACE

class Database; // Ajout du forward declaration

class DeletePackage : public QWidget
{
    Q_OBJECT

public:
    explicit DeletePackage(Database* db, QWidget *parent = nullptr); // Passage du pointeur Database*

protected:
    void showEvent(QShowEvent *event) override; // Pour rafraîchir à chaque affichage

private slots:
    void deleteSelectedPackage();
    void refreshTable();

signals:
    void deltepackDone();

private:
    void setupUI();
    void setupModel();

    Database* m_db;               // Ajout du membre Database*
    QSqlTableModel *model;
    QTableView *tableView;
};

#endif // DELETEPACKAGE_H
