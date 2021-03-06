#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName("JunQiReplay");
    a.setOrganizationName("tootal");
    QString locale = QLocale::system().name();
    QLocale::setDefault(QLocale(locale));
    QTranslator translator;
    translator.load("JunQiReplay_" + locale, ":/");
    a.installTranslator(&translator);
    MainWindow w;
    w.show();
    return a.exec();
}
