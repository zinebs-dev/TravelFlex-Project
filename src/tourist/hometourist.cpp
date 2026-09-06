#include "hometourist.h"
#include <QSqlQuery>


HomeTourist::HomeTourist(Database* db, int touristId, QWidget *parent)
    :  m_db(db), m_touristId(touristId),QWidget(parent)
{
    setWindowTitle("Acceuil Touriste");
    resize(900, 600);
    setStyleSheet("font-family: 'Segoe UI';");

    // Layout principal

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // =====================================
    // SIDEBAR
    // =====================================
    QWidget *sidebar = new QWidget();
    sidebar->setFixedWidth(220);
    sidebar->setStyleSheet(
        "background-color: #38b6ff;"
        "border-top-right-radius: 15px;"
        "border-bottom-right-radius: 15px;"
        );

    QVBoxLayout *sidebarLayout = new QVBoxLayout(sidebar);
    sidebarLayout->setAlignment(Qt::AlignTop);
    sidebarLayout->setSpacing(15);
    sidebarLayout->setContentsMargins(20, 30, 20, 30);

    // Logo
    QLabel *logo = new QLabel("TravelFlex ");
    QPixmap pixmap(":/images/image/2.png");

    if (!pixmap.isNull()) {
        pixmap = pixmap.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation);
        logo->setPixmap(pixmap);
        logo->setAlignment(Qt::AlignCenter);
    } else {
        qWarning() << "Erreur: Impossible de charger l'image depuis les ressources.";
    }

    logo->setStyleSheet(
        "font-size: 22px;"
        "font-weight: bold;"
        "color: white;"
        "padding-bottom: 20px;"
        "border-bottom: 2px solid rgba(255,255,255,0.2);"
        );
    sidebarLayout->addWidget(logo);

    // Boutons du menu
    QPushButton *btnHome = createSidebarButton("Accueil", ":/icons/add.png");
    QPushButton *btnReservation = createSidebarButton("Réservations", ":/icons/booking.png");
    QPushButton *btnPaiement = createSidebarButton("Gérer réservations ", ":/icons/payement.png");

    sidebarLayout->addWidget(btnHome);
    sidebarLayout->addWidget(btnReservation);
    sidebarLayout->addWidget(btnPaiement);
    sidebarLayout->addStretch();

    // Bouton Déconnexion
    QPushButton *btnLogout = new QPushButton("Déconnexion");
    btnLogout->setIcon(QIcon(":/icons/logout.png"));
    btnLogout->setIconSize(QSize(20, 20));
    btnLogout->setStyleSheet(
        "QPushButton {"
        "   text-align: left;"
        "   padding: 12px 15px;"
        "   font-size: 15px;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "   background-color: rgba(255,255,255,0.1);"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255,255,255,0.2);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(255,255,255,0.3);"
        "}"
        );
    btnLogout->setCursor(Qt::PointingHandCursor);
    sidebarLayout->addWidget(btnLogout);

    QLabel *copyright = new QLabel("© 2024 TravelFlex\nVersion 1.0.0");
    copyright->setStyleSheet("color: rgba(255,255,255,0.7); font-size: 11px;");
    sidebarLayout->addWidget(copyright);

    // =====================================
    // PARTIE PRINCIPALE
    // =====================================
    QWidget *mainContent = new QWidget();
    QVBoxLayout *contentLayout = new QVBoxLayout(mainContent);
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->setSpacing(0);

    // Navbar
    QWidget *navbar = new QWidget();
    navbar->setFixedHeight(70);
    navbar->setStyleSheet(
        "background-color: white;"
        "border-bottom: 1px solid #E0E0E0;"
        "border-top-right-radius: 15px;"
        );

    QHBoxLayout *navbarLayout = new QHBoxLayout(navbar);
    navbarLayout->setContentsMargins(30, 0, 30, 0);

    pageTitle = new QLabel("Tableau de Bord");
    pageTitle->setStyleSheet("font-size: 20px; font-weight: bold; color: #555;");
    navbarLayout->addWidget(pageTitle);
    navbarLayout->addStretch();

    QMap<QString, QString> infos = m_db->getTouristInfo(m_touristId);

    QString initials = infos.value("Firstname").left(1).toUpper() + infos.value("Lastname").left(1).toUpper();
    QPushButton* avatarBtn = new QPushButton(initials, this);
    avatarBtn->setFixedSize(44, 44);
    avatarBtn->setStyleSheet(
        "QPushButton {"
        " background-color: #38b6ff;"
        " color: white;"
        " font: bold 20px 'Segoe UI', sans-serif;"
        " border: none;"
        " border-radius: 22px;"
        "}"
        "QPushButton:hover {"
        " background-color: #50c9ff;"
        "}"
        );
    avatarBtn->setCursor(Qt::PointingHandCursor);
    navbarLayout->addWidget(avatarBtn);


    // StackedWidget pour les pages
    stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(createHomePage());
    m_reserverPackage = new ReserverPackage(m_db, m_touristId, this);
    stackedWidget->addWidget(m_reserverPackage);
    m_paymentPackage = new PaymentPackage(m_db, m_touristId, this);
    stackedWidget->addWidget(m_paymentPackage);

    ProfilePage* profilePage = new ProfilePage(m_db, m_touristId, this);
    stackedWidget->addWidget(profilePage);

    contentLayout->addWidget(navbar);
    contentLayout->addWidget(stackedWidget);

    // Connexions
    connect(btnHome, &QPushButton::clicked, [this](){
        stackedWidget->setCurrentIndex(0);
        pageTitle->setText("Tableau de Bord");
    });
    connect(btnReservation, &QPushButton::clicked, this, &HomeTourist::openReservation);
    connect(btnLogout, &QPushButton::clicked, this, &HomeTourist::logout);
    connect(btnPaiement, &QPushButton::clicked, [this]() {
         m_paymentPackage->refresh();
        stackedWidget->setCurrentWidget(m_paymentPackage);
        pageTitle->setText("Paiement");
    });
    connect(avatarBtn, &QPushButton::clicked, [this, profilePage]() {
        stackedWidget->setCurrentWidget(profilePage);
        pageTitle->setText("Mes informations personnelles");
    });
    connect(m_reserverPackage, &ReserverPackage::bookingAdded, this, &HomeTourist::refresh);
    connect(m_paymentPackage, &PaymentPackage::paymentDone, this, &HomeTourist::refresh);

    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(mainContent);
}

void HomeTourist::logout()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Déconnexion",
                                  "Êtes-vous sûr de vouloir vous déconnecter?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        if (m_touristlogin) {
            delete m_touristlogin;
        }
        m_touristlogin = new LoginTourist(m_db);
        m_touristlogin->show();
        this->close();
    }
}


// ... (reste du code inchangé)

QWidget* HomeTourist::createHomePage()
{
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #F5F6FA;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(25);

    // Titre de bienvenue
    QString nomTourist = m_db->getTouristName(m_touristId);
    QLabel *welcome = new QLabel("Bienvenue " + nomTourist + " !");
    welcome->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    layout->addWidget(welcome);

    // Cartes de statistiques
    QHBoxLayout *statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(20);

    nbResaLabel = new QLabel;
    nbResaLabel->setAlignment(Qt::AlignCenter);
    nbPackagesLabel = new QLabel;
    nbPackagesLabel->setAlignment(Qt::AlignCenter);

    statsLayout->addWidget(createStatCard("Réservations", nbResaLabel, "#3498DB"));
    statsLayout->addWidget(createStatCard("Packages",nbPackagesLabel , "#2ECC71"));
    layout->addLayout(statsLayout);

    // Ajout de l'historique des activités récentes
    layout->addSpacing(15);
    layout->addWidget(createActivityHistory());

    return page;
}

// Historique des réservations récentes (activité)
QWidget* HomeTourist::createActivityHistory()
{
    QWidget *activityWidget = new QWidget();
    QVBoxLayout *vbox = new QVBoxLayout(activityWidget);
    vbox->setContentsMargins(0,0,0,0);
    vbox->setSpacing(10);

    QLabel *title = new QLabel("Historique récent");
    title->setStyleSheet("font-size: 18px; font-weight: bold; color: #444;");
    vbox->addWidget(title);

    // Utilise le membre !
    m_historyTable = new QTableWidget(activityWidget);
    m_historyTable->setColumnCount(3);
    QStringList headers;
    headers << "Date" << "Package" << "Paiement";
    m_historyTable->setHorizontalHeaderLabels(headers);

    m_historyTable->horizontalHeader()->setStretchLastSection(true);
    m_historyTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_historyTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_historyTable->setSelectionMode(QAbstractItemView::NoSelection);

    vbox->addWidget(m_historyTable);

    // Remplit la table la première fois
    refresh();

    return activityWidget;
}
QWidget* HomeTourist::createStatCard(const QString &title, QLabel* valueLabel, const QString &color)
{
    QWidget *card = new QWidget();
    card->setStyleSheet(
        QString(
            "background-color: white;"
            "border-radius: 12px;"
            "border-left: 2px solid %1;"
            "padding: 8px 10px;"
            ).arg(color)
        );

    QVBoxLayout *cardLayout = new QVBoxLayout(card);
    cardLayout->setSpacing(4);
    cardLayout->setContentsMargins(10,10,10,10);

    QLabel *titleLabel = new QLabel(title, card);
    titleLabel->setStyleSheet(QString("font-size: 15px; font-weight: bold; color: %1;").arg(color));
    titleLabel->setAlignment(Qt::AlignLeft);

    valueLabel->setStyleSheet(QString("font-size: 32px; font-weight: bold; color: %1;").arg(color));
    titleLabel->setAlignment(Qt::AlignLeft);

    cardLayout->addWidget(titleLabel);
    cardLayout->addSpacing(8);
    cardLayout->addWidget(valueLabel);


    return card;
}
QPushButton* HomeTourist::createSidebarButton(const QString &text, const QString &iconPath)
{
    QPushButton *button = new QPushButton(text);
    button->setIcon(QIcon(iconPath));
    button->setIconSize(QSize(20, 20));

    button->setStyleSheet(
        "QPushButton {"
        "   text-align: left;"
        "   padding: 12px 15px;"
        "   font-size: 15px;"
        "   color: white;"
        "   border: none;"
        "   border-radius: 8px;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgba(255,255,255,0.2);"
        "}"
        "QPushButton:pressed {"
        "   background-color: rgba(255,255,255,0.3);"
        "}"
        );

    button->setCursor(Qt::PointingHandCursor);
    return button;
}

void HomeTourist::openReservation()
{
    stackedWidget->setCurrentWidget(m_reserverPackage);
    pageTitle->setText("Réservations");
}

void HomeTourist::refresh()
{
    if (!m_historyTable) return;
    QList<QVariantMap> bookings = m_db->getRecentBookingsForTourist(m_touristId, 5);

    m_historyTable->setRowCount(0);
    if (bookings.isEmpty()) {
        m_historyTable->setRowCount(1);
        m_historyTable->setSpan(0, 0, 1, 3);
        m_historyTable->setItem(0, 0, new QTableWidgetItem("Aucune activité récente."));
    } else {
        m_historyTable->setRowCount(bookings.size());
        int row = 0;
        for (const QVariantMap& booking : bookings) {
            m_historyTable->setItem(row, 0, new QTableWidgetItem(booking["bookingDate"].toString()));
            m_historyTable->setItem(row, 1, new QTableWidgetItem(booking["packageName"].toString()));
            QString payment = booking["paymentStatus"].toString() == "paid" ? "Payé" : "Non payé";
            m_historyTable->setItem(row, 2, new QTableWidgetItem(payment));
            row++;
        }
    }
    refreshStats();
}
void HomeTourist::refreshStats()
{
    if (nbResaLabel) {
        int nbResa = m_db->getTouristBookingCount(m_touristId);
        nbResaLabel->setText(QString::number(nbResa));
    }
    if (nbPackagesLabel) {
        int nbPackages = m_db->getAvailablePackageCount();
        nbPackagesLabel->setText(QString::number(nbPackages));
    }
}

HomeTourist::~HomeTourist() {

}
