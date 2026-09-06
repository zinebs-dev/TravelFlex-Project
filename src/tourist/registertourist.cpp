#include "registertourist.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QFrame>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

RegisterTourist::RegisterTourist(Database *db, QWidget *parent)
    : QWidget(parent), db(db)
{
    // Layout principal
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 10, 20, 10);
    mainLayout->setSpacing(15);
    setFixedSize(600, 600);

    // Titre
    QLabel *titleLabel = new QLabel("S'inscrire");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #333;");
    titleLabel->setAlignment(Qt::AlignLeft);
    mainLayout->addWidget(titleLabel);

    // Cadre du formulaire
    QFrame *formFrame = new QFrame();
    formFrame->setStyleSheet("background-color: white; border-radius: 8px; padding: 15px;");
    QFormLayout *formLayout = new QFormLayout(formFrame);
    formLayout->setContentsMargins(10, 10, 10, 10);
    formLayout->setSpacing(12);

    // Style des champs
    QString fieldStyle = "padding: 8px; border: 1px solid #ddd; border-radius: 4px;";

    // Champs de saisie
    firstnameEdit = new QLineEdit();
    firstnameEdit->setPlaceholderText("Prénom");
    firstnameEdit->setStyleSheet(fieldStyle);

    lastnameEdit = new QLineEdit();
    lastnameEdit->setPlaceholderText("Nom");
    lastnameEdit->setStyleSheet(fieldStyle);

    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Nom d'utilisateur");
    usernameEdit->setStyleSheet(fieldStyle);

    emailEdit = new QLineEdit();
    emailEdit->setPlaceholderText("Email");
    emailEdit->setStyleSheet(fieldStyle);

    phoneEdit = new QLineEdit();
    phoneEdit->setPlaceholderText("Téléphone");
    phoneEdit->setStyleSheet(fieldStyle);

    passportNumberEdit = new QLineEdit();
    passportNumberEdit->setPlaceholderText("N° Passeport");
    passportNumberEdit->setStyleSheet(fieldStyle);

    nationalityEdit = new QLineEdit();
    nationalityEdit->setPlaceholderText("Nationalité");
    nationalityEdit->setStyleSheet(fieldStyle);

    dateOfBirthEdit = new QDateEdit();
    dateOfBirthEdit->setCalendarPopup(true);
    dateOfBirthEdit->setDisplayFormat("dd/MM/yyyy");
    dateOfBirthEdit->setMaximumDate(QDate::currentDate().addYears(-10));
    dateOfBirthEdit->setStyleSheet(fieldStyle);

    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Mot de passe");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setStyleSheet(fieldStyle);

    confirmPasswordEdit = new QLineEdit();
    confirmPasswordEdit->setPlaceholderText("Confirmer le mot de passe");
    confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    confirmPasswordEdit->setStyleSheet(fieldStyle);

    // Ajout des champs au formulaire
    formLayout->addRow("Prénom", firstnameEdit);
    formLayout->addRow("Nom", lastnameEdit);
    formLayout->addRow("Nom d'utilisateur", usernameEdit);
    formLayout->addRow("Email", emailEdit);
    formLayout->addRow("Téléphone", phoneEdit);
    formLayout->addRow("N° Passeport", passportNumberEdit);
    formLayout->addRow("Nationalité", nationalityEdit);
    formLayout->addRow("Date de naissance", dateOfBirthEdit);
    formLayout->addRow("Mot de passe", passwordEdit);
    formLayout->addRow("Confirmation", confirmPasswordEdit);

    mainLayout->addWidget(formFrame, 1);

    // Boutons
    QPushButton* backButton = new QPushButton("Retour");
    backButton->setStyleSheet(
        "QPushButton {"
        "   background: #fff;"
        "   color: #38b6ff;"
        "   border: 2.5px solid #38b6ff;"
        "   border-radius: 10px;"
        "   padding: 12px 0;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   min-width: 130px;"
        "}"
        "QPushButton:hover {"
        "   background: #e7f6fd;"
        "   color: #2994c8;"
        "   border-color: #2994c8;"
        "}"
        );

    QPushButton* registerButton = new QPushButton("S'inscrire");
    registerButton->setStyleSheet(
        "QPushButton {"
        "   background: #38b6ff;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 10px;"
        "   padding: 12px 0;"
        "   font-size: 18px;"
        "   font-weight: bold;"
        "   min-width: 130px;"
        "}"
        "QPushButton:hover {"
        "   background: #2994c8;"
        "}"
        );

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget(backButton);
    buttonLayout->addSpacing(18);
    buttonLayout->addWidget(registerButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    // Connexions des signaux
    connect(registerButton, &QPushButton::clicked, this, &RegisterTourist::onRegisterClicked);
    connect(backButton, &QPushButton::clicked, this, &RegisterTourist::onBackClicked);

    // Définir le layout principal
    this->setLayout(mainLayout);

    // Option: Définir une taille minimale recommandée
    this->setMinimumSize(500, 700);
}

RegisterTourist::~RegisterTourist()
{
}


void RegisterTourist::onBackClicked()
{
    WelcomeTourist *welcometourist = new WelcomeTourist(db);
    welcometourist->show();
    this->close();
}

void RegisterTourist::onRegisterClicked()
{
    if (!db) {
        QMessageBox::critical(this, "Erreur critique", "Connexion à la base de données perdue.");
        return;
    }

    QString firstname = firstnameEdit->text().trimmed();
    QString lastname = lastnameEdit->text().trimmed();
    QString username = usernameEdit->text().trimmed();
    QString email = emailEdit->text().trimmed();
    QString phone = phoneEdit->text().trimmed();
    QString passportNumber = passportNumberEdit->text().trimmed();
    QString nationality = nationalityEdit->text().trimmed();
    QDate dateOfBirth = dateOfBirthEdit->date();
    QString password = passwordEdit->text();
    QString confirmPassword = confirmPasswordEdit->text();

    if (firstname.isEmpty() || lastname.isEmpty() || username.isEmpty() ||
        email.isEmpty() || phone.isEmpty() || passportNumber.isEmpty() ||
        nationality.isEmpty() || password.isEmpty() || confirmPassword.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }

    if (password != confirmPassword) {
        QMessageBox::warning(this, "Erreur", "Les mots de passe ne correspondent pas.");
        return;
    }

    if (!dateOfBirth.isValid()) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une date de naissance valide.");
        return;
    }

    // Debug pour suivre le flot
    qDebug() << "Tentative inscription:" << firstname << lastname << username << email<<phone<<passportNumber<<nationality<<dateOfBirth<<password;

    // Utilise la méthode Database
    if (!db->registerTourist(firstname, lastname, username, email, phone, passportNumber, nationality, dateOfBirth, password)) {
        // Affiche le message d'erreur précis venant de la base
        QMessageBox::critical(this, "Erreur", db->lastError().text());
        return;
    }
    QMessageBox::information(this, "Succès", "Inscription réussie !");
    WelcomeTourist *welcomeTourist = new WelcomeTourist(db);
    welcomeTourist->show();
    this->close();
}
