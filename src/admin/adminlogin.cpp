#include "adminlogin.h"
#include "welcome.h"


AdminLogin::AdminLogin(Database* db, QWidget *parent)
    : QWidget(parent), m_db(db), m_homeAdmin(nullptr)
{
    setWindowTitle("Connexion Admin");
    setFixedSize(600, 600);

    // Création d'un cadre pour la partie colorée
    QFrame *topColorBar = new QFrame(this);
    topColorBar->setGeometry(0, 0, width(), 200);
    topColorBar->setStyleSheet(
        "background: #38b6ff;"
        "border-radius: 12px;"
        );

    // Création des widgets
    titleLabel = new QLabel(" Se connecter", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size:40px; font-weight: bold; margin-bottom: 80px; margin-top: 80px; color:#fff");

    usernameLabel = new QLabel("Nom d'utilisateur:", this);
    usernameLabel->setStyleSheet(
        "font-size:16px;"
        "margin-left: 100px;"
        "color:#38b6ff;"
        );

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setStyleSheet(
        "border:1px solid #38b6ff;"
        "max-width: 300px;"
        "min-width: 100px;"
        "border-radius:7px;"
        "height:10px;"
        "min-height: 30px;"
        "margin-left: 100px;"
        );

    passwordLabel = new QLabel("Mot de passe :", this);
    passwordLabel->setStyleSheet(
        "font-size:16px;"
        "margin-left: 100px;"
        "color:#38b6ff;"
        );

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setStyleSheet(
        "border:1px solid #38b6ff;"
        "max-width: 300px;"
        "min-width: 100px;"
        "border-radius:7px;"
        "height:10px;"
        "min-height: 30px;"
        "margin-left: 100px;"
        );
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    showPasswordCheckBox = new QCheckBox("Afficher le mot de passe", this);
    showPasswordCheckBox->setStyleSheet("margin-left: 100px;");

    loginButton = new QPushButton("Se connecter", this);
    backButton = new QPushButton("Retour", this);

    QString buttonStyle = (
        "QPushButton {"
        "  padding: 10px;"
        "  font-size: 16px;"
        "  min-width: 100px;"
        "  border: 1px solid #38b6ff;"
        "  margin: 20px;"
        "  color: #fff;"
        "  cursor: pointer;"
        "  border-radius: 7px;"
        "  background-color:#38b6ff;"
        "  display:block;"
        "}"
        "QPushButton:hover {"
        "  background-color:#95d0f3;"
        "}"
        );

    loginButton->setStyleSheet(buttonStyle);
    backButton->setStyleSheet(buttonStyle);

    // Disposition
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(titleLabel);

    QVBoxLayout *formLayout = new QVBoxLayout();
    formLayout->addWidget(usernameLabel);
    formLayout->addWidget(usernameLineEdit);
    formLayout->addSpacing(10);
    formLayout->addWidget(passwordLabel);
    formLayout->addWidget(passwordLineEdit);
    formLayout->addWidget(showPasswordCheckBox);
    formLayout->addSpacing(20);

    mainLayout->addLayout(formLayout);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(backButton);
    buttonLayout->addWidget(loginButton);

    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();

    // Connexions des signaux
    connect(showPasswordCheckBox, &QCheckBox::toggled, [this](bool checked) {
        passwordLineEdit->setEchoMode(checked ? QLineEdit::Normal : QLineEdit::Password);
    });

    connect(backButton, &QPushButton::clicked, [this]() {
        Welcome *welcome = new Welcome(m_db);
        welcome->show();
        this->close();
    });

    connect(loginButton, &QPushButton::clicked, this, &AdminLogin::attemptLogin);
}

void AdminLogin::attemptLogin()
{
    QString username = usernameLineEdit->text().trimmed();
    QString password = passwordLineEdit->text().trimmed();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez remplir tous les champs.");
        return;
    }


    Database* db = new Database("/Users/MEGA/OneDrive/Documents/Bureau/Projet_C++/database_tourist_management/datab.db");
    if (db->checkAdminCredentials(username, password)) {
        QMessageBox::information(this, "Succès", "Connexion réussie");
        m_homeAdmin = new HomeAdmin(db,nullptr);
        m_homeAdmin->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Erreur",
                             "Nom d'utilisateur ou mot de passe incorrect.\n"
                             "Veuillez vérifier vos informations.");
    }
}

AdminLogin::~AdminLogin()
{

}
