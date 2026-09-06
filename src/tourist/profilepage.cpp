#include "profilepage.h"
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QFrame>
#include <QSpacerItem>
#include <QPainter>
#include "database.h"

class AvatarLabel : public QLabel {
public:
    AvatarLabel(const QString& initials, QWidget* parent = nullptr)
        : QLabel(parent) {
        setText(initials);
        setStyleSheet("background-color: #38b6ff; color: white; font-size: 32px; font-weight: bold; border-radius: 40px;");
        setAlignment(Qt::AlignCenter);
        setFixedSize(80, 80);
    }
};

ProfilePage::ProfilePage(Database* db, int touristId, QWidget* parent)
    : QWidget(parent), m_db(db), m_touristId(touristId)
{
    setStyleSheet(R"(
        QWidget#profileCard {
            background: white;
            border-radius: 20px;
            padding: 32px 40px;
            border: 1px solid #eaeaea;
        }
        QLabel.title {
            font-size: 28px;
            font-weight: bold;
            color: #222;
            margin-bottom: 28px;
            letter-spacing: 1px;
        }
        QFormLayout QLabel {
            font-size: 16px;
        }
        QLabel.keyLabel {
            color: #888;
            font-weight: bold;
            min-width: 160px;
        }
        QLabel.valueLabel {
            color: #333;
            font-size: 17px;
        }
    )");

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    QLabel* title = new QLabel("Mes informations personnelles");
    title->setObjectName("title");
    title->setProperty("class", "title");
    mainLayout->addWidget(title, 0, Qt::AlignHCenter);

    // Card (fond blanc, arrondi, ombre)
    QFrame* card = new QFrame();
    card->setObjectName("profileCard");
    card->setMaximumWidth(580);
    QVBoxLayout* cardLayout = new QVBoxLayout(card);
    cardLayout->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    // Données du touriste
    QMap<QString, QString> infos = m_db->getTouristInfo(m_touristId);
    // Formulaire d'infos
    QFormLayout* form = new QFormLayout();
    form->setLabelAlignment(Qt::AlignRight | Qt::AlignVCenter);
    form->setFormAlignment(Qt::AlignTop | Qt::AlignHCenter);
    form->setHorizontalSpacing(20);
    form->setVerticalSpacing(16);

    QMap<QString, QString> labels = {
        {"Firstname", "Prénom"},
        {"Lastname", "Nom"},
        {"username", "Nom d'utilisateur"},
        {"email", "Email"},
        {"phone", "Téléphone"},
        {"passportNumber", "Numéro passeport"},
        {"nationality", "Nationalité"},
        {"dateOfBirth", "Date de naissance"}
    };
    QStringList champs = labels.keys();
    for (const QString& key : champs) {
        QString val = infos.value(key, "<Non renseigné>");
        QLabel* keyLabel = new QLabel(labels[key] + " :");
        keyLabel->setProperty("class", "keyLabel");
        QLabel* valueLabel = new QLabel(val.isEmpty() ? "<Non renseigné>" : val);
        valueLabel->setProperty("class", "valueLabel");
        form->addRow(keyLabel, valueLabel);
    }

    cardLayout->addLayout(form);
    mainLayout->addWidget(card, 0, Qt::AlignHCenter);
    mainLayout->addStretch();
}
