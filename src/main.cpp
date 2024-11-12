#include "headers/mainwindow.h"
#include <QApplication>
#include <QSettings>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("MyOrg");
    QCoreApplication::setOrganizationDomain("MyOrg.com");
    QCoreApplication::setApplicationName("MyApp");

    MainWindow *w = new MainWindow;
    w->show();
    return a.exec();
}
