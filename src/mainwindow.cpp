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
#include <QMessageBox>

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

void MainWindow::setupMenu()
{
    action_Open_Folder = new QAction(this);
    action_About = new QAction(this);
    action_AboutQt = new QAction(this);

    menubar = new QMenuBar(this);
    menubar->setGeometry(QRect(0, 0, WINDOW_WIDTH, 26));
    menu_File = new QMenu(menubar);
    menu_Help = new QMenu(menubar);
    this->setMenuBar(menubar);
    
    menubar->addMenu(menu_File);
    menubar->addMenu(menu_Help);
    menu_File->addAction(action_Open_Folder);
    menu_Help->addAction(action_About);
    menu_Help->addAction(action_AboutQt);
}

void MainWindow::setupUI()
{
    this->resize(WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setWindowIcon(QIcon(":/JunQiReplay.png"));
    
    centralwidget = new QWidget(this);
    verticalLayout = new QVBoxLayout(centralwidget);
    verticalLayout->setSpacing(0);
    verticalLayout->setContentsMargins(0, 0, 0, 0);
    horizontalLayout = new QHBoxLayout();
    horizontalLayout->setSpacing(0);
    lineEdit = new QLineEdit(centralwidget);

    horizontalLayout->addWidget(lineEdit);

    pushButton = new QPushButton(centralwidget);
    pushButton->setIcon(QIcon(":/refresh.png"));

    horizontalLayout->addWidget(pushButton);

    lineEdit_2 = new QLineEdit(centralwidget);

    horizontalLayout->addWidget(lineEdit_2);

    verticalLayout->addLayout(horizontalLayout);

    tableView = new QTableView(centralwidget);
    tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView->setSortingEnabled(true);
    tableView->horizontalHeader()->setProperty("showSortIndicator", QVariant(true));
    tableView->horizontalHeader()->setStretchLastSection(true);

    verticalLayout->addWidget(tableView);

    this->setCentralWidget(centralwidget);
    
    statusbar = new QStatusBar(this);
    this->setStatusBar(statusbar);

    setupMenu();

    retranslateUi();

    connectSlots();
}

void MainWindow::retranslateUi()
{
    auto translate = [](const char *text){
        return QCoreApplication::translate("MainWindow", text);
    };
    this->setWindowTitle(translate("JunQiReplay"));
    action_Open_Folder->setText(translate("&Open Folder"));
    action_About->setText(translate("&About"));
    action_AboutQt->setText(translate("About &Qt"));
    lineEdit->setPlaceholderText(translate("Folder Path"));
    lineEdit_2->setPlaceholderText(translate("Filter Name"));
    menu_File->setTitle(translate("&File"));
    menu_Help->setTitle(translate("&Help"));
}

void MainWindow::connectSlots()
{
    connect(action_Open_Folder, &QAction::triggered,
            this, &MainWindow::action_Open_Folder_triggered);
    connect(pushButton, &QPushButton::clicked,
            this, &MainWindow::pushButton_clicked);
    connect(lineEdit, &QLineEdit::returnPressed,
            this, &MainWindow::lineEdit_returnPressed);
    connect(action_About, &QAction::triggered,
        this, [this]() {
            QMessageBox::about(this, windowTitle(), tr("An application for QQGame JunQi Replay Manager."));
        });
    connect(action_AboutQt, &QAction::triggered,
        this, [this]() {
            QMessageBox::aboutQt(this, tr("About Qt"));
        });
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
