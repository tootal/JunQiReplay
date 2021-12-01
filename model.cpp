#include "model.h"

#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QDataStream>
#include <QTextCodec>
#include <QDesktopServices>
#include <QUrl>
#include <QProgressDialog>

Model::Model(QString path, QObject *parent) 
    : QAbstractTableModel(parent)
{
    QTimer::singleShot(0, this, [this, path]() {
        reload(path);
    });
}

int Model::rowCount(const QModelIndex &) const
{
    return replays.size();
}

int Model::columnCount(const QModelIndex &) const
{
    return 7;
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    Replay replay = replays[row];
    if (role == Qt::DisplayRole) {
        switch (col) {
        case 0: // Time
            return replay.time.toString("yyyy/MM.dd hh:mm:ss");
        case 1: // This
            return replay.thisName();
        case 2: // That
            return replay.thatName();
        case 3: // Previous
            return replay.prevName();
        case 4: // Next
            return replay.nextName();
        case 5: // Steps
            return replay.steps;
        case 6: // Result
            if (replay.result == Replay::Result::Win) return tr("Win");
            else if (replay.result == Replay::Result::Tie) return tr("Tie");
            else if (replay.result == Replay::Result::Lose) return tr("Lose");
        default:
            return QVariant();
        }
    } else if (role == Qt::BackgroundRole) {
        switch (col) {
        case 1:
            return replay.thisBrush();
        case 2:
            return replay.thatBrush();
        case 3:
            return replay.prevBrush();
        case 4:
            return replay.nextBrush();
        }
    } else if (role == Qt::ForegroundRole) {
        if (col >= 1 && col <= 4) {
            return QBrush(QColor(Qt::white));
        } else if (col == 6) {
            return replay.resultBrush();
        }
    }
    return QVariant();
}

QVariant Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0:
            return tr("Time");
        case 1:
            return tr("This");
        case 2:
            return tr("That");
        case 3:
            return tr("Previous");
        case 4:
            return tr("Next");
        case 5:
            return tr("Steps");
        case 6:
            return tr("Result");
        }
    }
    return QVariant();
}

void Model::openReplay(const QModelIndex &index)
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(
                                  replays[index.row()].filePath));
}

void Model::reload(QString path)
{
    if (path.isEmpty()) return ;
    QDir dir(path);
    dir.setNameFilters({"*.jgs"});
    dir.setSorting(QDir::Time);
    auto infoList = dir.entryInfoList();
    auto win = qobject_cast<QWidget*>(parent());
    QProgressDialog dialog(
        tr("Parsing files..."),
        tr("Abort"),
        0,
        infoList.size(),
        win);
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
        if (dialog.wasCanceled()) {
            break;
        }
    }
    dialog.setValue(infoList.size());
    emit dataChanged(index(0, 0),
                     index(rowCount() - 1, columnCount() - 1));
}
