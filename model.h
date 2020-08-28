#pragma once

#include <QAbstractTableModel>
#include <QFileInfoList>
#include <QBrush>

struct Info {
    enum Color {
        Yellow,
        Blue,
        Green,
        Purple
    };
    enum Result {
        Win,
        Lose,
        Tie
    };
    int color;
    QBrush brush() const {
        return brush(color);
    }
    QBrush brush(int x) const {
        switch (x) {
        case Yellow:
            return QBrush(QColor(216, 108, 0));
        case Blue:
            return QBrush(QColor(40, 104, 160));
        case Green:
            return QBrush(QColor(115, 158, 5));
        case Purple:
            return QBrush(QColor(144, 68, 160));
        }   
    }
    int steps;
    QString names[4];
    int result;
};

class Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Model(QString path, QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void openReplay(const QModelIndex &index);
private:
    QFileInfoList infoList;
    QVector<Info> infos;
};

