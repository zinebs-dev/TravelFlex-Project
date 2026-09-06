#include "homeadmin.h"

HomeAdmin::HomeAdmin(Database* db, QWidget *parent)
    : QWidget(parent), m_db(db)
{
    setWindowTitle("Acceuil Admin");
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
    QPushButton *btnAddPackage = createSidebarButton("Ajouter Packages", ":/icons/add.png");
    QPushButton *btnEditPackage = createSidebarButton("Modifier Packages", ":/icons/edit.png");
    QPushButton *btnDeletePackage = createSidebarButton("Supprimer Packages", ":/icons/list.png");

    sidebarLayout->addWidget(btnHome);
    sidebarLayout->addWidget(btnAddPackage);
    sidebarLayout->addWidget(btnEditPackage);
    sidebarLayout->addWidget(btnDeletePackage);
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

    // StackedWidget pour les pages
    stackedWidget = new QStackedWidget();
    stackedWidget->addWidget(createHomePage());
    addPackagePage = new AddPackage(m_db, this);
    stackedWidget->addWidget(addPackagePage);
    DeletePackagePage = new DeletePackage(m_db, this);
    stackedWidget->addWidget(DeletePackagePage);
    EditPackagePage = new EditPackage(m_db, this);
    stackedWidget->addWidget(EditPackagePage);


    contentLayout->addWidget(navbar);
    contentLayout->addWidget(stackedWidget);

    // Connexions
    connect(btnHome, &QPushButton::clicked, [this](){
        stackedWidget->setCurrentIndex(0);
        pageTitle->setText("Tableau de Bord");
    });

    connect(btnAddPackage, &QPushButton::clicked, this, &HomeAdmin::showAddPackagePage);
    connect(btnEditPackage, &QPushButton::clicked, this, &HomeAdmin::showEditPackagePage);
    connect(btnDeletePackage, &QPushButton::clicked, this, &HomeAdmin::showDeletePackagePage);
    connect(btnLogout, &QPushButton::clicked, this, &HomeAdmin::logout);
    connect(addPackagePage, &AddPackage::addpackDone, this, &HomeAdmin::refreshStats);
    connect(DeletePackagePage , &DeletePackage::deltepackDone, this, &HomeAdmin::refreshStats);
    connect(EditPackagePage, &EditPackage::editpackDone, this, &HomeAdmin::refreshStats);
    mainLayout->addWidget(sidebar);
    mainLayout->addWidget(mainContent);
}



void HomeAdmin::logout()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Déconnexion",
                                  "Êtes-vous sûr de vouloir vous déconnecter?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_adminlogin = new AdminLogin(m_db);
        m_adminlogin->show();
        this->close();
    }
}


void HomeAdmin::showAddPackagePage()
{
    stackedWidget->setCurrentWidget(addPackagePage);
    pageTitle->setText("Ajouter Packages");
}
void HomeAdmin::showEditPackagePage()
{
    stackedWidget->setCurrentWidget(EditPackagePage);
    pageTitle->setText("Modifier Packages");
}
void HomeAdmin::showDeletePackagePage()
{
    stackedWidget->setCurrentWidget(DeletePackagePage);
    pageTitle->setText("Supprimer Packages");
}

QWidget* HomeAdmin::createHomePage()
{
    QWidget *page = new QWidget();
    page->setStyleSheet("background-color: #F5F6FA;");
    QVBoxLayout *layout = new QVBoxLayout(page);
    layout->setContentsMargins(30, 30, 30, 30);
    layout->setSpacing(25);

    // Titre de bienvenue
    QLabel *welcome = new QLabel("Bienvenue, Admin");
    welcome->setStyleSheet("font-size: 24px; font-weight: bold; color: #333;");
    layout->addWidget(welcome);

    // Cartes de statistiques
    QHBoxLayout *statsLayout = new QHBoxLayout();
    statsLayout->setSpacing(20);

    // Dans HomeAdmin::createHomePage
    userCountLabel = new QLabel;
    packageCountLabel = new QLabel;
    bookingCountLabel = new QLabel;

    statsLayout->addWidget(createStatCard("Utilisateurs", userCountLabel, "#3498DB"));
    statsLayout->addWidget(createStatCard("Packages", packageCountLabel, "#2ECC71"));
    statsLayout->addWidget(createStatCard("Réservations", bookingCountLabel, "#E74C3C"));

    layout->addLayout(statsLayout);

    // Section "Réservations Récentes"
    QLabel *recentTitle = new QLabel("Réservations récentes");
    recentTitle->setStyleSheet("font-size: 18px; font-weight: bold; color: #333;");
    layout->addWidget(recentTitle);

    QTableWidget *bookingTable = new QTableWidget();
    bookingTable->setColumnCount(3);
    QStringList headers = {"Touriste", "Package", "Date"};
    bookingTable->setHorizontalHeaderLabels(headers);
    bookingTable->verticalHeader()->setVisible(false);
    bookingTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    bookingTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    bookingTable->horizontalHeader()->setStretchLastSection(true);
    bookingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    bookingTable->setStyleSheet(
        "QTableWidget {"
        "   background-color: white;"
        "   border-radius: 10px;"
        "   padding: 10px;"
        "   border: none;"
        "}"
        "QHeaderView::section {"
        "   background-color: #38b6ff;"
        "   color: white;"
        "   padding: 10px;"
        "}"
        );

    // Remplir avec les vraies réservations
    QList<QVariantMap> bookings = m_db->getLastBookings(5);
    bookingTable->setRowCount(bookings.isEmpty() ? 1 : bookings.size());
    if (bookings.isEmpty()) {
        bookingTable->setSpan(0,0,1,3);
        bookingTable->setItem(0, 0, new QTableWidgetItem("Aucune réservation récente."));
    } else {
        int row = 0;
        for (const QVariantMap& book : bookings) {
            bookingTable->setItem(row, 0, new QTableWidgetItem(book["user"].toString()));
            bookingTable->setItem(row, 1, new QTableWidgetItem(book["package"].toString()));
            bookingTable->setItem(row, 2, new QTableWidgetItem(book["date"].toString()));
            row++;
        }
    }


    layout->addWidget(bookingTable);
    refreshStats();
    return page;
}
QWidget* HomeAdmin::createStatCard(const QString &title, QLabel* valueLabel, const QString &color)
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
QPushButton* HomeAdmin::createSidebarButton(const QString &text, const QString &iconPath)
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
void HomeAdmin::refreshStats()
{
    if (userCountLabel)    userCountLabel->setText(QString::number(m_db->getTouristCount()));
    if (packageCountLabel) packageCountLabel->setText(QString::number(m_db->getPackageCount()));
    if (bookingCountLabel) bookingCountLabel->setText(QString::number(m_db->getBookingCount()));
}
