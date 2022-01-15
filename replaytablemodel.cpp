#include "replaytablemodel.h"

#include <QColor>
#include <QFont>
#include <QGuiApplication>

ReplayTableModel::ReplayTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{
    QString jgsStr = "C:\\Users\\tootal\\workspace\\JunQiReplay\\test\\junqi2021_11_19_22_0.jgs;C:\\Users\\tootal\\workspace\\JunQiReplay\\test\\junqi2021_11_1_23_0最后决胜步.jgs;C:\\Users\\tootal\\workspace\\JunQiReplay\\test\\junqi2021_10_30_0_19.jgs";
    QStringList jgsFile = jgsStr.split(';');
    for (int i = 0; i < jgsFile.size(); i++) {
        m_replays.append(Replay::fromJGSFile(jgsFile[i]));
    }
}

int ReplayTableModel::rowCount(const QModelIndex &) const
{
    return static_cast<int>(m_replays.size());
}

int ReplayTableModel::columnCount(const QModelIndex &) const
{
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
        return QFont("Microsoft YaHei", 14);
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

void ReplayTableModel::append(const Replay &replay)
{
    m_replays.append(replay);
}
