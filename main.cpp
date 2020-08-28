#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("JunQiReplay");
    a.setOrganizationName("tootal");
    MainWindow w;
    w.show();
    return a.exec();
}
