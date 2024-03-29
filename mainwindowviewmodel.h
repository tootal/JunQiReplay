#pragma once

#include <QObject>
#include <QString>
#include <QQmlEngine>

class MainWindowViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)
    Q_PROPERTY(QString qtVersion READ qtVersion CONSTANT)
    QML_ELEMENT

public:
    explicit MainWindowViewModel(QObject *parent = nullptr);

    QString userName();
    void setUserName(const QString &userName);

    QString qtVersion() const;

signals:
    void userNameChanged();

private:
    QString m_userName;
};
