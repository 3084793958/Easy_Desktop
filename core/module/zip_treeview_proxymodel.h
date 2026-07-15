#ifndef ZIP_TREEVIEW_PROXYMODEL_H
#define ZIP_TREEVIEW_PROXYMODEL_H

#include <QSortFilterProxyModel>

class Zip_TreeView_ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit Zip_TreeView_ProxyModel(QObject *parent = nullptr);
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

#endif // ZIP_TREEVIEW_PROXYMODEL_H
