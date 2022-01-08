#pragma once

#include <QAbstractTableModel>
#include <QFileInfoList>
#include <QBrush>
#include <QDateTime>

#include "replay.h"

class Model : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit Model(QString path, QObject *parent = nullptr);
    void openReplay(const QModelIndex &index);
    void reload(QString path);
    
protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

private:
    QVector<Replay> replays;
};

