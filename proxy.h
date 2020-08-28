#pragma once

#include <QSortFilterProxyModel>

class Proxy : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit Proxy(QObject *parent = nullptr);
    
protected:
    bool filterAcceptsRow(int source_row, 
                          const QModelIndex &source_parent) const override;
    
};

