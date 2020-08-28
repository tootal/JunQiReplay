#include "model.h"

#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QDataStream>
#include <QTextCodec>

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
        QFile file(info.absoluteFilePath());
        file.open(QFile::ReadOnly);
        auto bytes = file.readAll();
        file.close();
        infos[i].color = bytes[0x0f];
        // little endian
        infos[i].steps = (quint8)bytes[0x1b] * 0x100 + (quint8)bytes[0x1a];
        int pos = 0x20;
        for (int j = 0; j < 4; j++) {
            int color = (quint8)bytes[pos];
            if (color != 0xff) {
                auto name = bytes.mid(0x28, 20);
                infos[i].names[color] = QTextCodec::codecForName("GBK")->toUnicode(name);
            }
            pos += 88;
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
    if (role == Qt::DisplayRole) {
        switch (col) {
        case 0: // Time
            return infoList[row].fileTime(QFileDevice::FileModificationTime).toString("yyyy/MM.dd hh:mm:ss");
        case 1: // This
            return infos[row].names[infos[row].color];
        case 2: // That
            return infos[row].names[infos[row].color ^ 2];
        case 3: // Previous
            return infos[row].names[(infos[row].color + 1) % 4];
        case 4: // Next
            return infos[row].names[(infos[row].color + 3) % 4];
        case 5: // Steps
            return infos[row].steps;
        default:
            return QVariant();
        }
    } else if (role == Qt::BackgroundRole) {
        switch (col) {
        case 1:
            return infos[row].brush();
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
