#include "deletepackage.h"
#include "database.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QMessageBox>
#include <QHeaderView>
#include <QLabel>
#include <QFrame>
#include <QShowEvent>

DeletePackage::DeletePackage(Database* db, QWidget *parent)
    : QWidget(parent), m_db(db)
{
    setupUI();
    setupModel();
}

void DeletePackage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 10, 20, 10);
    mainLayout->setSpacing(15);


    // Cadre du tableau
    QFrame *tableFrame = new QFrame();
    tableFrame->setStyleSheet("background-color: white; border-radius: 8px; padding: 15px;");

    tableView = new QTableView(tableFrame);
    tableView->setStyleSheet("border: 1px solid #ddd; border-radius: 4px;");
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QVBoxLayout *frameLayout = new QVBoxLayout(tableFrame);
    frameLayout->addWidget(tableView);
    mainLayout->addWidget(tableFrame, 1);

    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    QPushButton *deleteButton = new QPushButton("Supprimer");
    deleteButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #E9573F;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover { background-color: #D94332; }"
        "QPushButton:pressed { background-color: #C0392B; }"
        );
    buttonLayout->addWidget(deleteButton);
    mainLayout->addLayout(buttonLayout);

    // Connexions
    connect(deleteButton, &QPushButton::clicked, this, &DeletePackage::deleteSelectedPackage);
}

void DeletePackage::setupModel()
{
    model = new QSqlTableModel(this, QSqlDatabase::database("MyConnection")); // Utilise la connexion partagée
    model->setTable("package");
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, tr("Destination"));
    model->setHeaderData(3, Qt::Horizontal, tr("Description"));
    model->setHeaderData(4, Qt::Horizontal, tr("Prix"));
    model->setHeaderData(5, Qt::Horizontal, tr("Durée"));

    tableView->setModel(model);
    tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tableView->setColumnHidden(0, true); // Cache la colonne ID
}

void DeletePackage::deleteSelectedPackage()
{
    QModelIndexList selected = tableView->selectionModel()->selectedRows();
    if (selected.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un package à supprimer");
        return;
    }

    int row = selected.first().row();
    int id = model->data(model->index(row, 0)).toInt();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation",
                                  "Êtes-vous sûr de vouloir supprimer ce package?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (!m_db->isConnected()) {
            QMessageBox::critical(this, "Erreur", "Connexion à la base de données échouée");
            return;
        }

        if (m_db->deletePackage(id)) {
            QMessageBox::information(this, "Succès", "Package supprimé avec succès");
            emit deltepackDone();
            refreshTable();
        } else {
            QMessageBox::critical(this, "Erreur", "Échec de la suppression du package");
        }
    }
}

void DeletePackage::refreshTable()
{
    model->select();
}

// Rafraîchissement automatique à chaque retour sur la page
void DeletePackage::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    refreshTable();
}
