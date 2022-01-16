#ifndef REPLAYTABLEMODEL_H
#define REPLAYTABLEMODEL_H

#include <QAbstractTableModel>
#include <QQmlEngine>

#include "src/replay.h"

class ReplayTableModel : public QAbstractTableModel
{
    Q_OBJECT
    Q_PROPERTY(QString jgsFileList MEMBER m_jgsFileList NOTIFY jgsFileListChanged)
    Q_PROPERTY(QQmlListProperty<QString> jgsFiles READ jgsFiles)
    QML_ELEMENT

public:
    explicit ReplayTableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    void append(const Replay &replay);

    QQmlListProperty<QString> jgsFiles();
    void appendJgsFile(QString *);
    qsizetype jgsFileCount() const;
    QString *jgsFile(qsizetype) const;
    void clearJgsFiles();
    void replaceJgsFile(qsizetype, QString *);
    void removeLastJgsFile();

signals:
    void jgsFileListChanged(QString newJgsFileList);

private:
    static void appendJgsFile(QQmlListProperty<QString> *, QString *);
    static qsizetype jgsFileCount(QQmlListProperty<QString> *);
    static QString* jgsFile(QQmlListProperty<QString> *, qsizetype);
    static void clearJgsFiles(QQmlListProperty<QString> *);
    static void replaceJgsFile(QQmlListProperty<QString> *, qsizetype, QString *);
    static void removeLastJgsFile(QQmlListProperty<QString> *);

    QVector<Replay> m_replays;
    QString m_jgsFileList;
    QVector<QString *> m_jgsFiles;
};

#endif // REPLAYTABLEMODEL_H
