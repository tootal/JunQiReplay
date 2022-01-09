#include "mainwindowviewmodel.h"

MainWindowViewModel::MainWindowViewModel(QObject *parent)
    : QObject(parent)
{

}

QString MainWindowViewModel::userName()
{
    return m_userName;
}

void MainWindowViewModel::setUserName(const QString &userName)
{
    if (userName == m_userName) {
        return ;
    }
    m_userName = userName;
    emit userNameChanged();
}

QString MainWindowViewModel::qtVersion() const
{
    return qVersion();
}
