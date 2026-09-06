#include "paymentpackage.h"
#include <QVBoxLayout>
#include <QTableWidget>
#include <QPushButton>
#include <QMessageBox>
#include <QHeaderView>
#include <QLabel>

PaymentPackage::PaymentPackage(Database* db, int touristId, QWidget* parent)
    : QWidget(parent), m_db(db), m_touristId(touristId)
{
    QVBoxLayout* layout = new QVBoxLayout(this);

    QLabel* title = new QLabel("Gestion des réservations impayées");
    title->setStyleSheet("font-size: 22px; font-weight: bold; color: #222; margin-bottom: 15px;");
    layout->addWidget(title);

    table = new QTableWidget(this);
    layout->addWidget(table);

    refresh();
}

void PaymentPackage::refresh()
{
    QList<QList<QString>> unpaidRes = m_db->getUnpaidReservationsForTourist(m_touristId);

    table->clear();
    table->setColumnCount(6);
    table->setHorizontalHeaderLabels({"ID", "Package", "Date", "Montant", "Payer", "Annuler"});
    table->verticalHeader()->setVisible(false);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);
    table->setShowGrid(false);
    table->setAlternatingRowColors(true);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    table->setMinimumHeight(240);

    int rowCount = unpaidRes.size();
    table->setRowCount(rowCount > 0 ? rowCount : 1);

    if (rowCount == 0) {
        QTableWidgetItem* item = new QTableWidgetItem("Aucune réservation impayée.");
        item->setFlags(Qt::ItemIsEnabled);
        item->setTextAlignment(Qt::AlignCenter);
        table->setItem(0, 0, item);
        for (int j = 1; j < 6; ++j)
            table->setItem(0, j, new QTableWidgetItem(""));
        return;
    }

    for (int i = 0; i < rowCount; ++i) {
        // ID, Package, Date, Montant
        for (int j = 0; j < 4; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem(unpaidRes[i][j]);
            item->setTextAlignment(Qt::AlignCenter);
            table->setItem(i, j, item);
        }
        // ---- Bouton "Payer" ----
        QPushButton* payBtn = new QPushButton("Payer");
        payBtn->setStyleSheet(
            "background-color: #2ECC71;"
            "color: white;"
            "font-weight: bold;"
            "border-radius: 5px;"
            "padding: 4px 12px;"
            );
        table->setCellWidget(i, 4, payBtn);

        // ---- Bouton "Annuler" ----
        QPushButton* cancelBtn = new QPushButton("Annuler");
        cancelBtn->setStyleSheet(
            "background-color: #E74C3C;"
            "color: white;"
            "font-weight: bold;"
            "border-radius: 5px;"
            "padding: 4px 12px;"
            );
        table->setCellWidget(i, 5, cancelBtn);

        QString bookingId = unpaidRes[i][0];

        connect(payBtn, &QPushButton::clicked, this, [this, bookingId]() {
            if (m_db->setBookingPaid(bookingId)) {
                QMessageBox::information(this, "Paiement", "Réservation payée avec succès !");
                emit paymentDone();
                this->refresh();
            } else {
                QMessageBox::warning(this, "Erreur", "Le paiement a échoué.");
            }
        });

        connect(cancelBtn, &QPushButton::clicked, this, [this, bookingId]() {
            auto reply = QMessageBox::question(this, "Annulation", "Voulez-vous vraiment annuler cette réservation ?");
            if (reply == QMessageBox::Yes) {
                if (m_db->deleteBooking(bookingId)) {
                    QMessageBox::information(this, "Annulation", "Réservation annulée.");
                    emit paymentDone();
                    this->refresh();
                } else {
                    QMessageBox::warning(this, "Erreur", "L'annulation a échoué.");
                }
            }
        });
    }
}
