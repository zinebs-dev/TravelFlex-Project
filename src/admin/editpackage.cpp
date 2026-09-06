#include "editpackage.h"
#include <QFrame>
#include <QFormLayout>

EditPackage::EditPackage(Database* db, QWidget *parent)
    : QWidget(parent), m_db(db)
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 10, 20, 10);
    mainLayout->setSpacing(15);


    // Cadre du formulaire
    QFrame *formFrame = new QFrame();
    formFrame->setStyleSheet("background-color: white; border-radius: 8px; padding: 15px;");
    QFormLayout *formLayout = new QFormLayout(formFrame);
    formLayout->setContentsMargins(10, 10, 10, 10);
    formLayout->setSpacing(12);

    // Ligne de recherche
    QHBoxLayout* searchLayout = new QHBoxLayout();
    searchLineEdit = new QLineEdit();
    searchLineEdit->setStyleSheet("padding: 8px; border: 1px solid #ddd; border-radius: 4px;");
    searchLineEdit->setPlaceholderText("Entrez le nom du package");

    searchButton = new QPushButton("Rechercher");
    searchButton->setStyleSheet(
        "QPushButton {"
        "   padding: 8px 20px;"
        "   font-size: 14px;"
        "   color: white;"
        "   background-color: #4a7dff;"
        "   border: none;"
        "   border-radius: 5px;"
        "   min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #3a6de8;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #2a5dd6;"
        "}"
        );

    searchLayout->addWidget(searchLineEdit);
    searchLayout->addWidget(searchButton);
    searchLayout->setSpacing(10);

    // Champs de formulaire
    nameLineEdit = new QLineEdit();
    nameLineEdit->setStyleSheet("padding: 8px; border: 1px solid #ddd; border-radius: 4px;");

    destLineEdit = new QLineEdit();
    destLineEdit->setStyleSheet("padding: 8px; border: 1px solid #ddd; border-radius: 4px;");

    descTextEdit = new QTextEdit();
    descTextEdit->setStyleSheet("padding: 8px; border: 1px solid #ddd; border-radius: 4px;");
    descTextEdit->setMaximumHeight(100);

    priceLineEdit = new QLineEdit();
    priceLineEdit->setStyleSheet("padding: 8px; border: 1px solid #ddd; border-radius: 4px;");

    durLineEdit = new QLineEdit();
    durLineEdit->setStyleSheet("padding: 8px; border: 1px solid #ddd; border-radius: 4px;");

    // Organisation des champs
    formLayout->addRow("Rechercher un package:", searchLayout);
    formLayout->addRow("Nom du package:", nameLineEdit);
    formLayout->addRow("Destination:", destLineEdit);
    formLayout->addRow("Description:", descTextEdit);
    formLayout->addRow("Prix:", priceLineEdit);
    formLayout->addRow("Durée:", durLineEdit);

    // Bouton Enregistrer
    saveButton = new QPushButton("Enregistrer les Modifications");
    saveButton->setStyleSheet(
        "QPushButton {"
        "   padding: 10px 25px;"
        "   font-size: 15px;"
        "   font-weight: 500;"
        "   color: white;"
        "   background-color: #10b981;"
        "   border: none;"
        "   border-radius: 6px;"
        "   min-width: 150px;"
        "}"
        "QPushButton:hover {"
        "   background-color: #0ea472;"
        "}"
        "QPushButton:pressed {"
        "   background-color: #0d9266;"
        "}"
        "QPushButton:disabled {"
        "   background-color: #d1d5db;"
        "}"
        );

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(saveButton);

    mainLayout->addWidget(formFrame, 1);
    mainLayout->addLayout(buttonLayout);

    setFieldsEnabled(false);

    connect(searchButton, &QPushButton::clicked, this, &EditPackage::searchPackage);
    connect(saveButton, &QPushButton::clicked, this, &EditPackage::saveChanges);
}

void EditPackage::clearFields() {
    searchLineEdit->clear();
    nameLineEdit->clear();
    destLineEdit->clear();
    descTextEdit->clear();
    priceLineEdit->clear();
    durLineEdit->clear();
    currentPackageId = -1;
}

void EditPackage::setFieldsEnabled(bool enabled) {
    nameLineEdit->setEnabled(enabled);
    destLineEdit->setEnabled(enabled);
    descTextEdit->setEnabled(enabled);
    priceLineEdit->setEnabled(enabled);
    durLineEdit->setEnabled(enabled);
    saveButton->setEnabled(enabled);
}

void EditPackage::searchPackage() {
    QString searchValue = searchLineEdit->text().trimmed();
    if (searchValue.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez saisir le nom du package.");
        return;
    }

    QVariantMap pkg = m_db->getPackageByName(searchValue);

    if (!pkg.isEmpty()) {
        currentPackageId = pkg["id"].toInt();
        nameLineEdit->setText(pkg["name"].toString());
        destLineEdit->setText(pkg["destination"].toString());
        descTextEdit->setPlainText(pkg["description"].toString());
        priceLineEdit->setText(QString::number(pkg["price"].toDouble()));
        durLineEdit->setText(pkg["duration"].toString());
        setFieldsEnabled(true);
    } else {
        QMessageBox::information(this, "Aucun résultat", "Aucun package trouvé avec ce nom.");
        setFieldsEnabled(false);
    }
}

void EditPackage::saveChanges() {
    if (currentPackageId == -1) {
        QMessageBox::warning(this, "Erreur", "Aucun package sélectionné.");
        return;
    }
    QString name = nameLineEdit->text().trimmed();
    QString dest = destLineEdit->text().trimmed();
    QString desc = descTextEdit->toPlainText().trimmed();
    double price = priceLineEdit->text().toDouble();
    QString dur = durLineEdit->text().trimmed();

    if (name.isEmpty() || desc.isEmpty() || price < 0 || dest.isEmpty() || dur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs correctement.");
        return;
    }
    // On suppose une méthode updatePackage dans Database
    bool success = m_db->updatePackage(currentPackageId, name, dest, desc, price, dur);
    if (success) {
        QMessageBox::information(this, "Succès", "Le package a été mis à jour !");
        emit editpackDone();
        clearFields();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec de la mise à jour du package.");
    }
}
