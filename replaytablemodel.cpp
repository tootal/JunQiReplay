#include "replaytablemodel.h"

#include <QColor>
#include <QFont>
#include <QDir>
#include <QGuiApplication>

ReplayTableModel::ReplayTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{
}

int ReplayTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return static_cast<int>(m_replays.size());
}

int ReplayTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid()) {
        return 0;
    }
    return 8;
}

QVariant ReplayTableModel::data(const QModelIndex &index, int role) const
{
    const int row = index.row(), col = index.column();
    const auto replay = m_replays[row];
    if (role == Qt::ForegroundRole) {
        if (col >= 2 && col <= 5) {
            return QColor(Qt::white);
        } else if (col == 7) {
            return replay.resultColor();
        }
    }
    if (role == Qt::BackgroundRole) {
        if (col >= 2 && col <= 5) {
            QVector<QColor> colorList{
                replay.thisColor(),
                replay.thatColor(),
                replay.prevColor(),
                replay.nextColor()
            };
            return colorList[col - 2];
        } else {
            return QVariant();
        }
    } else if (role == Qt::DisplayRole) {
        QStringList dataList{
            QString::number(row + 1),
            replay.time.toString("yyyy/MM/dd hh:mm:ss"),
            replay.thisName(),
            replay.thatName(),
            replay.prevName(),
            replay.nextName(),
            QString::number(replay.steps),
            replay.resultString()
        };
        return dataList[col];
    } else if (role == Qt::FontRole) {
//        return QGuiApplication::font();
        return QFont("Microsoft YaHei", 10);
    }

    return QVariant();
}

QVariant ReplayTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }
    if (role == Qt::DisplayRole) {
        QStringList dataList{"#", "Time", "This", "That", "Prev", "Next", "Step", "Result"};
        return dataList.at(section);
    } else if (role == Qt::FontRole) {
        return QFont("Microsoft YaHei", 14, QFont::DemiBold);
    }
    return QVariant();
}

QHash<int, QByteArray> ReplayTableModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "display" },
        { Qt::BackgroundRole, "background" },
        { Qt::ForegroundRole, "foreground" },
        { Qt::FontRole, "theFont" }
    };
}

QString ReplayTableModel::dirName() const
{
    return m_dirName;
}

void ReplayTableModel::setDirName(const QString &dirName)
{
    qDebug() << "setDirName: " << dirName;
    if (m_dirName == dirName) {
        return ;
    }
    beginResetModel();
    m_dirName = dirName;
    QDir dir(dirName);
    if (!dir.exists()) return ;
    dir.setNameFilters({"*.jgs"});
    dir.setSorting(QDir::Time);
    auto infoList = dir.entryInfoList();
    // TODO ProgressDialog
    for (int i = 0; i < infoList.size(); i++) {
        auto replay = Replay::fromJGSFile(infoList[i].absoluteFilePath());
        if (replay.hasError()) {
            qWarning() << "replay load failed: " << replay.errorString;
            continue;
        }
        m_replays.append(replay);
    }
    endResetModel();
}
