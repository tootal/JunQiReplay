#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QSettings>
#include <QSortFilterProxyModel>

#include "model.h"
#include "proxy.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QSettings settings;
    QString path;
    if (settings.contains("path")) {
        path = settings.value("path").toString();
        ui->lineEdit->setText(path);
    }
    model = new Model(path, this);
    auto proxy = new Proxy(this);
    proxy->setSourceModel(model);
    ui->tableView->setModel(proxy);
    ui->tableView->resizeColumnsToContents();
    connect(ui->tableView, &QTableView::doubleClicked,
            model, &Model::openReplay);
    connect(ui->lineEdit_2, &QLineEdit::textChanged,
            proxy, &Proxy::setFilterFixedString);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_Open_Folder_triggered()
{
    auto path = QFileDialog::getExistingDirectory(this);
    if (path.isEmpty()) return ;
    QSettings settings;
    settings.setValue("path", path);
    ui->lineEdit->setText(path);
}
