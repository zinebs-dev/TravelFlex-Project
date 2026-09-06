#include "addpackage.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>
#include <QFrame>

AddPackage::AddPackage(Database* db, QWidget *parent)
    : QWidget(parent), m_db(db)
{
    setupUI();
}

void AddPackage::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 10, 20, 10);
    mainLayout->setSpacing(15);

    // Cadre du formulaire
    QFrame *formFrame = new QFrame();
    formFrame->setStyleSheet("background-color: white; border-radius: 8px; padding: 15px;");
    QFormLayout *formLayout = new QFormLayout(formFrame);
    formLayout->setContentsMargins(10, 10, 10, 10);
    formLayout->setSpacing(12);

    // Champs du formulaire
    nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Nom du package");
    nameEdit->setStyleSheet("padding: 8px; border: 1px solid #ddd; border-radius: 4px;");

    destinationEdit = new QLineEdit();
    destinationEdit->setPlaceholderText("Destination");
    destinationEdit->setStyleSheet("padding: 8px; border: 1px solid #ddd; border-radius: 4px;");

    descriptionEdit = new QTextEdit();
    descriptionEdit->setPlaceholderText("Description détaillée");
    descriptionEdit->setStyleSheet("padding: 8px; border: 1px solid #ddd; border-radius: 4px; min-height: 100px;");

    priceSpinBox = new QDoubleSpinBox();
    priceSpinBox->setRange(0, 999999.99);
    priceSpinBox->setPrefix("$ ");
    priceSpinBox->setValue(0);
    priceSpinBox->setStyleSheet("padding: 8px; border: 1px solid #ddd; border-radius: 4px;");

    durationSpinBox = new QSpinBox();
    durationSpinBox->setRange(1, 365);
    durationSpinBox->setValue(0);
    durationSpinBox->setStyleSheet("padding: 8px; border: 1px solid #ddd; border-radius: 4px;");

    // Ajout des champs au formulaire
    formLayout->addRow("Nom:", nameEdit);
    formLayout->addRow("Destination:", destinationEdit);
    formLayout->addRow("Description:", descriptionEdit);
    formLayout->addRow("Prix:", priceSpinBox);
    formLayout->addRow("Durée:", durationSpinBox);

    mainLayout->addWidget(formFrame, 1);

    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    QPushButton *cancelButton = new QPushButton("Annuler");
    cancelButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #f0f0f0;"
        "   color: #555;"
        "   border: 1px solid #ddd;"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #e0e0e0;"
        "}"
        );

    QPushButton *saveButton = new QPushButton("Enregistrer");
    saveButton->setStyleSheet(
        "QPushButton {"
        "   background-color: #5D9CEC;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #4A89DC;"
        "}"
        );

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(saveButton);
    mainLayout->addLayout(buttonLayout);

    // Connexions
    connect(saveButton, &QPushButton::clicked, this, &AddPackage::savePackage);
    connect(cancelButton, &QPushButton::clicked, this, [this]() {
        clearForm();
    });
}

void AddPackage::savePackage()
{
    // 1. Validation des champs
    if (nameEdit->text().isEmpty() || destinationEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Les champs obligatoires sont vides");
        return;
    }

    // 2. Appel à la couche Database partagée
    if (!m_db->isConnected()) {
        QMessageBox::critical(this, "Erreur", "Connexion à la base de données échouée");
        return;
    }

    bool success = m_db->addPackage(
        nameEdit->text(),
        destinationEdit->text(),
        descriptionEdit->toPlainText(),
        priceSpinBox->value(),
        durationSpinBox->value()
        );

    // 3. Gestion du résultat
    if (!success) {
        QMessageBox::critical(this, "Erreur Technique",
                              "Détails de l'erreur ont été écrits dans les logs.\n"
                              "Veuillez contacter le support.");
    } else {
        QMessageBox::information(this, "Succès", "Package ajouté !");
        emit addpackDone();
        clearForm();
    }
}

void AddPackage::clearForm()
{
    nameEdit->clear();
    destinationEdit->clear();
    descriptionEdit->clear();
    priceSpinBox->setValue(0);
    durationSpinBox->setValue(7);
}
