#ifndef PROFILEPAGE_H
#define PROFILEPAGE_H

#pragma once
#include <QWidget>
#include <QMap>

class Database;

class ProfilePage : public QWidget {
    Q_OBJECT
public:
    ProfilePage(Database* db, int touristId, QWidget* parent = nullptr);
    void refresh();
private:
    Database* m_db;
    int m_touristId;
};

#endif // PROFILEPAGE_H
