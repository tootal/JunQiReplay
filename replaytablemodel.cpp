#include "replaytablemodel.h"

#include "src/junqi.h"

ReplayTableModel::ReplayTableModel(QObject *parent)
    : QAbstractTableModel{parent}
{

}

int ReplayTableModel::rowCount(const QModelIndex &) const
{
    return 3;
}

int ReplayTableModel::columnCount(const QModelIndex &) const
{
    return 8;
}

QVariant ReplayTableModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::BackgroundRole) {
        if (index.column() >= 2 && index.column() <= 5) {
            QVector<QColor> colorList{JunQi::orange, JunQi::green, JunQi::blue, JunQi::purple};
            return colorList.at(index.column() - 2);
        } else {
            return QVariant();
        }
    } else if (role == Qt::DisplayRole) {
        QVector<QStringList> dataList{
            {"1", "2021/11.19 22:00:49", "游 戏 玩 家", ".", "觅思方向", "游戏玩家", "302", "Tie"},
            {"2", "2021/11.01 23:00:58", "游 戏 玩 家", ".", "醉美德阳", "欧阳亦儒", "408", "Win"},
            {"3", "2021/10.30 00:19:42", ".", "沐尘", "专属回忆", "钢化", "263", "Lose"}
        };
        return dataList.at(index.row()).at(index.column());
    } else {
        return QVariant();
    }
}

QVariant ReplayTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) {
        return QVariant();
    }
    QStringList dataList{"#", "Time", "This", "That", "Prev", "Next", "Step", "Result"};
    return dataList.at(section);
}

QHash<int, QByteArray> ReplayTableModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "display" },
        { Qt::BackgroundRole, "background" }
    };
}
