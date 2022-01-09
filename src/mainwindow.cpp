#include "mainwindow.h"

#include <QCoreApplication>
#include <QMenu>
#include <QFileDialog>
#include <QSettings>
#include <QSortFilterProxyModel>
#include <QDebug>
#include <QStandardItemModel>
#include <QProgressDialog>
#include <QDesktopServices>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QTableView>
#include <QHeaderView>
#include <QPushButton>
#include <QMenuBar>
#include <QStatusBar>

#include "proxy.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUI();
    setupModel();

    QSettings settings;
    QString path;
    if (settings.contains("path")) {
        path = settings.value("path").toString();
        lineEdit->setText(path);
    }
    auto proxy = new Proxy(this);
    proxy->setSourceModel(aModel);
    tableView->setModel(proxy);
    tableView->resizeColumnsToContents();
    connect(tableView, &QTableView::doubleClicked,
            this, [this, proxy](const QModelIndex &index) {
        QDesktopServices::openUrl(QUrl::fromLocalFile(
                                      replays[index.row()].filePath));
    });
    connect(lineEdit_2, &QLineEdit::textChanged,
            proxy, &Proxy::setFilterFixedString);
    reload();
}

MainWindow::~MainWindow()
{
}


void MainWindow::action_Open_Folder_triggered()
{
    auto path = QFileDialog::getExistingDirectory(this);
    if (path.isEmpty()) return ;
    lineEdit->setText(path);
    reload();
}

void MainWindow::pushButton_clicked()
{
    reload();
}

void MainWindow::reload()
{
    auto path = lineEdit->text();
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
    tableView->resizeColumnsToContents();
    dialog.setValue(infoList.size());
}

void MainWindow::setupUI()
{
    const int WINDOW_WIDTH = 1000;
    const int WINDOW_HEIGHT = 600;
    this->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    QIcon icon;
    icon.addFile(QString::fromUtf8(":/JunQiReplay.png"), QSize(), QIcon::Normal, QIcon::Off);
    this->setWindowIcon(icon);
    action_Open_Folder = new QAction(this);
    action_Open_Folder->setObjectName(QString::fromUtf8("action_Open_Folder"));
    centralwidget = new QWidget(this);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    verticalLayout = new QVBoxLayout(centralwidget);
    verticalLayout->setSpacing(0);
    verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(0);
    horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
    lineEdit = new QLineEdit(centralwidget);
    lineEdit->setObjectName(QString::fromUtf8("lineEdit"));

    horizontalLayout->addWidget(lineEdit);

    pushButton = new QPushButton(centralwidget);
    pushButton->setObjectName(QString::fromUtf8("pushButton"));
    QIcon icon1;
    icon1.addFile(QString::fromUtf8(":/refresh.png"), QSize(), QIcon::Normal, QIcon::Off);
    pushButton->setIcon(icon1);

    horizontalLayout->addWidget(pushButton);

    lineEdit_2 = new QLineEdit(centralwidget);
    lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));

    horizontalLayout->addWidget(lineEdit_2);


    verticalLayout->addLayout(horizontalLayout);

    tableView = new QTableView(centralwidget);
    tableView->setObjectName(QString::fromUtf8("tableView"));
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSortingEnabled(true);
    tableView->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
    tableView->horizontalHeader()->setStretchLastSection(true);

    verticalLayout->addWidget(tableView);

    this->setCentralWidget(centralwidget);
    menubar = new QMenuBar(this);
    menubar->setObjectName(QString::fromUtf8("menubar"));
    menubar->setGeometry(QRect(0, 0, WINDOW_WIDTH, 26));
    menu_File = new QMenu(menubar);
    menu_File->setObjectName(QString::fromUtf8("menu_File"));
    this->setMenuBar(menubar);
    statusbar = new QStatusBar(this);
    statusbar->setObjectName(QString::fromUtf8("statusbar"));
    this->setStatusBar(statusbar);

    menubar->addAction(menu_File->menuAction());
    menu_File->addAction(action_Open_Folder);

    retranslateUi();

    connectSlots();
}


void MainWindow::retranslateUi()
{
    this->setWindowTitle(QCoreApplication::translate("MainWindow", "JunQiReplay", nullptr));
    action_Open_Folder->setText(QCoreApplication::translate("MainWindow", "&Open Folder", nullptr));
    lineEdit->setPlaceholderText(QCoreApplication::translate("MainWindow", "Folder Path", nullptr));
    pushButton->setText(QString());
    lineEdit_2->setPlaceholderText(QCoreApplication::translate("MainWindow", "Filter Name", nullptr));
    menu_File->setTitle(QCoreApplication::translate("MainWindow", "&File", nullptr));
}

void MainWindow::connectSlots()
{
    connect(action_Open_Folder, &QAction::triggered,
            this, &MainWindow::action_Open_Folder_triggered);
    connect(pushButton, &QPushButton::clicked,
            this, &MainWindow::pushButton_clicked);
    connect(lineEdit, &QLineEdit::returnPressed,
            this, &MainWindow::lineEdit_returnPressed);
}

void MainWindow::lineEdit_returnPressed()
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
