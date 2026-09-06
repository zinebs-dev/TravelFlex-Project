#include "reserverpackage.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QScrollArea>
#include <QVariant>
#include <QDebug>

ReserverPackage::ReserverPackage(Database* db, int touristId, QWidget* parent)
    : QWidget(parent), m_db(db), m_touristId(touristId)
{
    setStyleSheet("background-color: #F5F6FA;");

    QVBoxLayout* globalLayout = new QVBoxLayout(this);
    globalLayout->setContentsMargins(20, 20, 20, 20);

    QLabel* titre = new QLabel("Liste des Packages Disponibles");
    titre->setStyleSheet("font-size: 20px; font-weight: bold; color: #333;");
    globalLayout->addWidget(titre);

    // Scroll area pour la liste des packages
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setWidgetResizable(true);

    QWidget* scrollContent = new QWidget;
    m_layout = new QVBoxLayout(scrollContent);
    m_layout->setAlignment(Qt::AlignTop);

    scrollArea->setWidget(scrollContent);
    globalLayout->addWidget(scrollArea);

    loadPackages();
}

void ReserverPackage::loadPackages()
{
    QList<QVariantMap> packages = m_db->getAllPackages();

    for (const QVariantMap& pkg : packages) {
        QGroupBox* box = new QGroupBox(pkg["name"].toString());
        QVBoxLayout* boxLayout = new QVBoxLayout(box);

        boxLayout->addWidget(new QLabel("Destination : " + pkg["destination"].toString()));
        boxLayout->addWidget(new QLabel("Description : " + pkg["description"].toString()));
        boxLayout->addWidget(new QLabel("Prix : " + QString::number(pkg["price"].toDouble()) + " $"));
        boxLayout->addWidget(new QLabel("Durée : " +(pkg["duration"].toString())));

        QPushButton* btn = new QPushButton("Réserver");
        btn->setProperty("packageId", pkg["id"].toInt());
        connect(btn, &QPushButton::clicked, this, &ReserverPackage::onReserveClicked);

        boxLayout->addWidget(btn);
        m_layout->addWidget(box);
    }
    m_layout->addStretch();
}

void ReserverPackage::onReserveClicked()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    if (!btn) return;

    int packageId = btn->property("packageId").toInt();

    // Vérifier si la réservation existe déjà
    if (m_db->reservationExists(m_touristId, packageId)) {
        QMessageBox::warning(this, "Déjà réservé", "Vous avez déjà réservé ce package.");
        return;
    }

    // Tenter la réservation
    if (m_db->addBooking(m_touristId, packageId)) {
        QMessageBox::information(this, "Succès", "Réservation enregistrée !");
        emit bookingAdded();
    } else {
        QMessageBox::critical(this, "Erreur", "La réservation a échoué.");
    }
}
