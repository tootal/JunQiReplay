#include "replaytablemodel.h"

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
    return 2;
}

QVariant ReplayTableModel::data(const QModelIndex &index, int role) const
{
    QStringList dataList{"cat", "black", "dog", "brown", "bird", "white"};
    auto dataIndex = index.row() * columnCount(index) + index.column();
    switch (role) {
    case Qt::DisplayRole:
        return dataList.at(dataIndex);
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> ReplayTableModel::roleNames() const
{
    return {
        { Qt::DisplayRole, "display" }
    };
}
