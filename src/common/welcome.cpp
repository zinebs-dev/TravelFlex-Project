#include "welcome.h"
#include "adminlogin.h"
#include "welcometourist.h"
#include <QVBoxLayout>
#include <QGraphicsDropShadowEffect>

Welcome::Welcome(Database* db, QWidget *parent)
    : QWidget(parent), m_db(db)
{
    setWindowTitle("Bienvenue");
    setFixedSize(600, 600);
    setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #38b6ff, stop:1 #2076d6);");

    titleLabel = new QLabel("Bienvenue!", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(
        "font-size:45px;"
        "font-weight:bold;"
        "color:#fff;"
        "background:transparent;"
        "border:none;"
        "margin-bottom: 80px;"
        "margin-top: 80px;"
        "letter-spacing:2px;"
        );
    QGraphicsDropShadowEffect* shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(18);
    shadow->setOffset(0, 4);
    shadow->setColor(QColor(0, 0, 0, 120));
    titleLabel->setGraphicsEffect(shadow);

    adminButton = new QPushButton("Admin", this);
    touristButton = new QPushButton("Touriste", this);

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
    adminButton->setStyleSheet(buttonStyle);
    touristButton->setStyleSheet(buttonStyle);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(50, 60, 50, 60);
    layout->setSpacing(30);
    layout->addWidget(titleLabel);
    layout->addWidget(adminButton, 0, Qt::AlignCenter);
    layout->addWidget(touristButton, 0, Qt::AlignCenter);
    layout->addStretch();

    connect(adminButton, &QPushButton::clicked, this, &Welcome::onAdminClicked);
    connect(touristButton, &QPushButton::clicked, this, &Welcome::onTouristClicked);
}

Welcome::~Welcome()
{
}

void Welcome::onAdminClicked()
{
    AdminLogin *adminLogin = new AdminLogin(m_db);
    adminLogin->show();
    this->close();
}

void Welcome::onTouristClicked()
{
    WelcomeTourist *welcomeTourist = new WelcomeTourist(m_db); // Passage du pointeur DB !
    welcomeTourist->show();
    this->close();
}
