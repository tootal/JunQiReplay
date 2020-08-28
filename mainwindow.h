#pragma once

#include <QMainWindow>

class Model;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void on_action_Open_Folder_triggered();
    
private:
    Ui::MainWindow *ui;
    Model *model;
};
