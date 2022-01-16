#ifndef REPLAYTABLEMODEL_H
#define REPLAYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QQmlEngine>

#include "src/replay.h"

class ReplayTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString dirName READ dirName WRITE setDirName NOTIFY dirNameChanged)
    QML_ELEMENT

public:
    explicit ReplayTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QString dirName() const;
    void setDirName(const QString &dirName);

signals:
    void dirNameChanged();

private:
    QList<Replay> m_replays;
    QString m_dirName;
};

#endif // REPLAYTABLEMODEL_H
