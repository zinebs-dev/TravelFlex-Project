#include <QApplication>
#include <QCoreApplication>
#include "welcome.h"
#include "database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // datab.db is copied next to the executable by CMake at build time
    // (see CMakeLists.txt), so it is always found here regardless of
    // where the project is built or deployed.
    Database* db = new Database(QCoreApplication::applicationDirPath() + "/datab.db");
    Welcome* w = new Welcome(db);
    w->show();
    return a.exec();
}
