#include "welcometourist.h"
#include "welcome.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QGraphicsDropShadowEffect>
WelcomeTourist::WelcomeTourist(Database* db, QWidget *parent)
    : QWidget(parent), m_db(db)
{
    setWindowTitle("Bienvenue Touriste");
    setFixedSize(600, 600);
    setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #38b6ff, stop:1 #2076d6);");

    // Titre sans cadre, ombre douce
    titleLabel = new QLabel("Bienvenue !", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size:45px;"
        "font-weight:bold;"
        "color:#fff;"
        "background:transparent;"
        "border:none;"
        "margin-bottom:80px;"
        "margin-top:80px;"
        "letter-spacing:2px;"
        );
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(18);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 120));
    titleLabel->setGraphicsEffect(shadow);

    // Boutons doux, arrondis
    QString buttonStyle = R"(
QPushButton {
    padding: 14px 32px;
    font-size: 18px;
    font-weight: 600;
    min-width: 190px;
    border: none;
    border-radius: 18px;
    color: #38b6ff;
    background: #fff;
    margin: 10px 0;
    box-shadow: 0 2px 12px rgba(56,182,255,0.10);
    transition: background 0.3s, color 0.3s;
}
QPushButton:hover {
    background: #95d0f3;
    color: #fff;
}
)";
    loginButton = new QPushButton("Se connecter", this);
    registerButton = new QPushButton("S'inscrire", this);
    loginButton->setStyleSheet(buttonStyle);
    registerButton->setStyleSheet(buttonStyle);

    // Bouton "Retour" plus discret mais cohérent
    backButton = new QPushButton("Retour", this);
    backButton->setStyleSheet(R"(
QPushButton {
    padding: 8px 22px;
    font-size: 15px;
    border-radius: 12px;
    color: #fff;
    background: #38b6ff;
    border: 2px solid #fff;
}
QPushButton:hover {
    background: #95d0f3;
    color: #2076d6;
}
)");

    // Disposition aérée et centrée
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(50, 60, 50, 60);
    mainLayout->setSpacing(30);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(loginButton, 0, Qt::AlignCenter);
    mainLayout->addWidget(registerButton, 0, Qt::AlignCenter);
    mainLayout->addStretch();

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(backButton);
    bottomLayout->addStretch();

    mainLayout->addLayout(bottomLayout);

    // Connexions des signaux
    connect(loginButton, &QPushButton::clicked, this, &WelcomeTourist::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &WelcomeTourist::onRegisterClicked);
    connect(backButton, &QPushButton::clicked, this, &WelcomeTourist::onBackClicked);
}

WelcomeTourist::~WelcomeTourist()
{
}

void WelcomeTourist::onLoginClicked()
{
    LoginTourist *logintouristwindow = new LoginTourist(m_db);
    logintouristwindow->show();
    this->close();
}

void WelcomeTourist::onRegisterClicked()
{
    RegisterTourist *registerWindow = new RegisterTourist(m_db);
    registerWindow->show();
    this->close();
}

void WelcomeTourist::onBackClicked()
{
    Welcome *welcome = new Welcome(m_db); // Passe aussi la base ici si Welcome en a besoin
    welcome->show();
    this->close();
}
