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
    if (path.isEmpty()) return ;
    QDir dir(path);
    dir.setNameFilters({"*.jgs"});
    dir.setSorting(QDir::Time);
    infoList = dir.entryInfoList();
    infos.resize(infoList.size());
    for (int i = 0; i < infoList.size(); i++) {
        auto info = infoList[i];
        infos[i].filePath = info.absoluteFilePath();
        infos[i].time = info.fileTime(QFileDevice::FileModificationTime);
        QFile file(info.absoluteFilePath());
        file.open(QFile::ReadOnly);
        auto bytes = file.readAll();
        file.close();
        int color = bytes[0x0f];
        infos[i].color = color;
        // little endian
        int steps = (quint8)bytes[0x1b] * 0x100 + (quint8)bytes[0x1a];
        infos[i].steps = steps;
        int pos = 0x20;
        int type = 0;
        for (int j = 0; j < 4; j++) {
            int color = (quint8)bytes[pos];
            if (color != 0xff) {
                type++;
                auto name = bytes.mid(pos + 8, 20);
                infos[i].names[color] = QTextCodec::codecForName("GBK")->toUnicode(name);
            }
            pos += 88;
        }
        int flag = (quint8)bytes[0x19c + 10 * (steps - 1) + 3];
        if (flag == 2) infos[i].result = Info::Tie;
        else {
            int lose_color = (quint8)bytes[0x19c + 10 * (steps - 2) + 2];
            if (type == 2) {
                if (color == lose_color) infos[i].result = Info::Lose;
                else infos[i].result = Info::Win;
            } else {
                if (color == lose_color || color == (lose_color ^ 2))
                    infos[i].result = Info::Lose;
                else infos[i].result = Info::Win;
            }
        }
    }
}

int Model::rowCount(const QModelIndex &) const
{
    return infoList.size();
}

int Model::columnCount(const QModelIndex &) const
{
    return 7;
}

QVariant Model::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    int col = index.column();
    Info infor = infos[row];
    if (role == Qt::DisplayRole) {
        switch (col) {
        case 0: // Time
            return infor.time.toString("yyyy/MM.dd hh:mm:ss");
        case 1: // This
            return infor.names[infos[row].color];
        case 2: // That
            return infor.names[infos[row].color ^ 2];
        case 3: // Previous
            return infor.names[(infos[row].color + 1) % 4];
        case 4: // Next
            return infor.names[(infos[row].color + 3) % 4];
        case 5: // Steps
            return infor.steps;
        case 6: // Result
            if (infor.result == Info::Win) return tr("Win");
            else if (infor.result == Info::Tie) return tr("Tie");
            else if (infor.result == Info::Lose) return tr("Lose");
        default:
            return QVariant();
        }
    } else if (role == Qt::BackgroundRole) {
        switch (col) {
        case 1:
            if (infor.names[infos[row].color].isEmpty()) return QBrush(QColor(Qt::gray));
            else return infos[row].brush(infos[row].color);
        case 2:
            if (infor.names[infos[row].color ^ 2].isEmpty()) return QBrush(QColor(Qt::gray));
            else return infos[row].brush(infos[row].color ^ 2);
        case 3:
            if (infor.names[(infos[row].color + 1) % 4].isEmpty()) return QBrush(QColor(Qt::gray));
            else return infos[row].brush((infos[row].color + 1) % 4);
        case 4:
            if (infor.names[(infos[row].color + 3) % 4].isEmpty()) return QBrush(QColor(Qt::gray));
            else return infos[row].brush((infos[row].color + 3) % 4);
        }
    } else if (role == Qt::ForegroundRole) {
        if (col >= 1 && col <= 4) {
            return QBrush(QColor(Qt::white));
        } else if (col == 6) {
            if (infor.result == Info::Win) return QBrush(QColor(Qt::darkGreen));
            else if (infor.result == Info::Tie) return QBrush(QColor(Qt::darkGray));
            else if (infor.result == Info::Lose) return QBrush(QColor(Qt::red));
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
        infos[index.row()].filePath));
}
