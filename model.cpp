#include "model.h"

#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QDataStream>
#include <QTextCodec>
#include <QDesktopServices>
#include <QUrl>

Model::Model(QString path, QObject *parent) 
    : QAbstractTableModel(parent)
{
    reload(path, false);
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

void Model::reload(QString path, bool notification)
{
    if (path.isEmpty()) return ;
    QDir dir(path);
    dir.setNameFilters({"*.jgs"});
    dir.setSorting(QDir::Time);
    auto infoList = dir.entryInfoList();
    replays.resize(infoList.size());
    for (int i = 0; i < infoList.size(); i++) {
        auto info = infoList[i];
        replays[i].filePath = info.absoluteFilePath();
        replays[i].time = info.fileTime(QFileDevice::FileModificationTime);
        QFile file(info.absoluteFilePath());
        file.open(QFile::ReadOnly);
        auto bytes = file.readAll();
        file.close();
        int color = bytes[0x0f];
        replays[i].color = static_cast<Replay::Color>(color);
        // little endian
        int steps = (quint8)bytes[0x1b] * 0x100 + (quint8)bytes[0x1a];
        replays[i].steps = steps;
        int pos = 0x20;
        int type = 0;
        for (int j = 0; j < 4; j++) {
            int color = (quint8)bytes[pos];
            if (color != 0xff) {
                type++;
                auto name = bytes.mid(pos + 8, 20);
                replays[i].names[color] = QTextCodec::codecForName("GBK")->toUnicode(name);
            }
            pos += 88;
        }
        int flag = (quint8)bytes[0x19c + 10 * (steps - 1) + 3];
        if (flag == 2) replays[i].result = Replay::Result::Tie;
        else {
            int lose_color = (quint8)bytes[0x19c + 10 * (steps - 2) + 2];
            if (type == 2) {
                if (color == lose_color) replays[i].result = Replay::Result::Lose;
                else replays[i].result = Replay::Result::Win;
            } else {
                if (color == lose_color || color == (lose_color ^ 2))
                    replays[i].result = Replay::Result::Lose;
                else replays[i].result = Replay::Result::Win;
            }
        }
    }
    if (notification) 
        emit dataChanged(index(0, 0),
                         index(rowCount() - 1, columnCount() - 1));
}
