#ifndef REPLAYTABLEMODEL_H
#define REPLAYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QQmlEngine>

class ReplayTableModel : public QAbstractTableModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit ReplayTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
};

#endif // REPLAYTABLEMODEL_H
