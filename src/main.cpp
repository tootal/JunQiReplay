#include "mainwindow.h"

#include <QApplication>
#include <QTranslator>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("JunQiReplay");
    app.setOrganizationName("tootal");
    QString locale = QLocale::system().name();
    QLocale::setDefault(QLocale(locale));
    QTranslator translator;
    {
        auto loaded = translator.load("JunQiReplay_" + locale, ":/");
        if (!loaded) {
            qWarning() << "Translator load failed";
        }
    }
    app.installTranslator(&translator);

    bool qmlApp = false;
    if (qmlApp) {
        QQmlApplicationEngine engine;
        const QUrl url("qrc:/MainWindow.qml");
        QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                         &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
        engine.load(url);
        return app.exec();
    } else {
        MainWindow win;
        win.show();
        return app.exec();
    }
}
