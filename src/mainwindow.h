#pragma once

#include <QMainWindow>

#include "replay.h"

class Model;
class QAbstractItemModel;
class QVBoxLayout;
class QHBoxLayout;
class QLineEdit;
class QPushButton;
class QTableView;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private slots:
    void action_Open_Folder_triggered();
    
    void pushButton_clicked();
    
    void lineEdit_returnPressed();
    
private:
    int WINDOW_WIDTH = 1000;
    int WINDOW_HEIGHT = 600;

    QAbstractItemModel *aModel;
    QVector<Replay> replays;

    QAction *action_Open_Folder;
    QAction *action_About;
    QAction *action_AboutQt;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QLineEdit *lineEdit_2;
    QTableView *tableView;
    QMenuBar *menubar;
    QMenu *menu_File;
    QMenu *menu_Help;
    QStatusBar *statusbar;

    void reload();
    void setupMenu();
    void setupUI();
    void setupModel();
    void retranslateUi();
    void connectSlots();
};
