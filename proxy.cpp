#include "proxy.h"

Proxy::Proxy(QObject *parent) 
    : QSortFilterProxyModel(parent)
{
    
}

bool Proxy::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index1 = sourceModel()->index(source_row, 1, source_parent);
    QModelIndex index2 = sourceModel()->index(source_row, 2, source_parent);
    QModelIndex index3 = sourceModel()->index(source_row, 3, source_parent);
    QModelIndex index4 = sourceModel()->index(source_row, 4, source_parent);

    return sourceModel()->data(index1).toString().contains(filterRegExp())
        || sourceModel()->data(index2).toString().contains(filterRegExp())
        || sourceModel()->data(index3).toString().contains(filterRegExp())
        || sourceModel()->data(index4).toString().contains(filterRegExp());
}
