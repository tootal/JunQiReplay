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
    int color;
    QBrush brush() const {
        return brush(color);
    }
    QBrush brush(int x) const {
        switch (x) {
        case Yellow:
            return QBrush(QColor(Qt::yellow));
        case Blue:
            return QBrush(QColor(Qt::blue));
        case Green:
            return QBrush(QColor(Qt::green));
        case Purple:
            return QBrush(QColor(Qt::magenta));
        }   
    }
    int steps;
    QString names[4];
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
signals:
    
private:
    QFileInfoList infoList;
    QVector<Info> infos;
};

