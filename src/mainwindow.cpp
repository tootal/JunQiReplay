#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QStandardItemModel>
#include <QProgressDialog>
#include <QDesktopServices>

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
    setupModel();
    auto proxy = new Proxy(this);
    proxy->setSourceModel(aModel);
    ui->tableView->setModel(proxy);
    ui->tableView->resizeColumnsToContents();
    connect(ui->tableView, &QTableView::doubleClicked,
            this, [this, proxy](const QModelIndex &index) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(
                                      replays[index.row()].filePath));
    });
    connect(ui->lineEdit_2, &QLineEdit::textChanged,
            proxy, &Proxy::setFilterFixedString);
    reload();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_action_Open_Folder_triggered()
{
    auto path = QFileDialog::getExistingDirectory(this);
    if (path.isEmpty()) return ;
    ui->lineEdit->setText(path);
    reload();
}

void MainWindow::on_pushButton_clicked()
{
    reload();
}

void MainWindow::reload()
{
    auto path = ui->lineEdit->text();
    QSettings settings;
    settings.setValue("path", path);
    aModel->removeRows(0, aModel->rowCount(QModelIndex()), QModelIndex());
    if (path.isEmpty()) return ;
    QDir dir(path);
    dir.setNameFilters({"*.jgs"});
    dir.setSorting(QDir::Time);
    auto infoList = dir.entryInfoList();
    aModel->insertRows(0, infoList.size(), QModelIndex());
    QProgressDialog dialog(
        tr("Parsing files..."),
        tr("Abort"),
        0,
        infoList.size(),
        this);
    dialog.setModal(true);
    dialog.show();
    for (int i = 0; i < infoList.size(); i++) {
        dialog.setValue(i);
        dialog.setLabelText(infoList[i].fileName());
        auto replay = Replay::fromJGSFile(infoList[i].absoluteFilePath());
        QCoreApplication::processEvents();
        if (replay.hasError()) {
            qWarning() << "replay load failed: " << replay.errorString;
            continue;
        }
        replays.append(replay);
        aModel->setData(aModel->index(i, 0, QModelIndex()),
                        replay.time.toString("yyyy/MM.dd hh:mm:ss"), Qt::DisplayRole);
        aModel->setData(aModel->index(i, 1, QModelIndex()),
                        replay.thisName(), Qt::DisplayRole);
        aModel->setData(aModel->index(i, 1, QModelIndex()),
                        replay.thisBrush(), Qt::BackgroundRole);
        aModel->setData(aModel->index(i, 2, QModelIndex()),
                        replay.thatName(), Qt::DisplayRole);
        aModel->setData(aModel->index(i, 2, QModelIndex()),
                        replay.thatBrush(), Qt::BackgroundRole);
        aModel->setData(aModel->index(i, 3, QModelIndex()),
                        replay.prevName(), Qt::DisplayRole);
        aModel->setData(aModel->index(i, 3, QModelIndex()),
                        replay.prevBrush(), Qt::BackgroundRole);
        aModel->setData(aModel->index(i, 4, QModelIndex()),
                        replay.nextName(), Qt::DisplayRole);
        aModel->setData(aModel->index(i, 4, QModelIndex()),
                        replay.nextBrush(), Qt::BackgroundRole);
        aModel->setData(aModel->index(i, 5, QModelIndex()),
                        replay.steps, Qt::DisplayRole);
        aModel->setData(aModel->index(i, 6, QModelIndex()),
                        replay.resultString(), Qt::DisplayRole);
        aModel->setData(aModel->index(i, 6, QModelIndex()),
                        replay.resultBrush(), Qt::ForegroundRole);
        for (int col = 1; col <= 4; col++) {
            aModel->setData(aModel->index(i, col, QModelIndex()),
                            QBrush(QColor(Qt::white)), Qt::ForegroundRole);
        }
        if (dialog.wasCanceled()) {
            break;
        }
    }
    ui->tableView->resizeColumnsToContents();
    dialog.setValue(infoList.size());
}

void MainWindow::on_lineEdit_returnPressed()
{
    reload();
}

void MainWindow::setupModel()
{
    aModel = new QStandardItemModel(0, 7, this);
    aModel->setHeaderData(0, Qt::Horizontal, tr("Time"));
    aModel->setHeaderData(1, Qt::Horizontal, tr("This"));
    aModel->setHeaderData(2, Qt::Horizontal, tr("That"));
    aModel->setHeaderData(3, Qt::Horizontal, tr("Previous"));
    aModel->setHeaderData(4, Qt::Horizontal, tr("Next"));
    aModel->setHeaderData(5, Qt::Horizontal, tr("Steps"));
    aModel->setHeaderData(6, Qt::Horizontal, tr("Result"));
}
