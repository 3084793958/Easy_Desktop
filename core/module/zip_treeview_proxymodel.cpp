#include "zip_treeview_proxymodel.h"

Zip_TreeView_ProxyModel::Zip_TreeView_ProxyModel(QObject *parent)
    :QSortFilterProxyModel(parent)
{}
bool Zip_TreeView_ProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QVariant leftData = left.data(Qt::UserRole);
    QVariant rightData = right.data(Qt::UserRole);
    bool hasUserData = leftData.isValid() && rightData.isValid();
    if (!hasUserData)
    {
        return QSortFilterProxyModel::lessThan(left, right);
    }

    QList<QVariant> leftList = leftData.toList();
    QList<QVariant> rightList = rightData.toList();

    if (leftList.first().toString() != rightList.first().toString())
    {
        return QSortFilterProxyModel::lessThan(left, right);
    }

    if (leftList.first().toString() == "name")
    {
        if (leftList[1].toBool() != rightList[1].toBool())
        {
            return leftList[1].toBool();
        }
    }
    else if (leftList.first().toString() == "size")
    {
        return leftList[1].toLongLong() < rightList[1].toLongLong();//这里不用慌,文件夹是0
    }
    else if (leftList.first().toString() == "type")
    {
        if (leftList[1].toBool() != rightList[1].toBool())
        {
            return leftList[1].toBool();
        }
    }
    else if (leftList.first().toString() == "date")
    {
        return QSortFilterProxyModel::lessThan(left, right);
    }
    return QSortFilterProxyModel::lessThan(left, right);
}
bool Zip_TreeView_ProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const//这里没有懒加载,直接判断即可
{
    QModelIndex idx = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!idx.isValid())
    {
        return false;
    }
    QVariant data = idx.data(Qt::UserRole);
    if (!data.canConvert<QList<QVariant>>())
    {
        return false;
    }
    QList<QVariant> list = data.toList();
    if (list.size() < 3)
    {
        return false;
    }
    QString path = list[2].toString();

    QRegExp regExp = filterRegExp();
    if (regExp.isEmpty())
    {
        return true;
    }
    if (path.contains(regExp))
    {
        return true;
    }
    int childCount = sourceModel()->rowCount(idx);
    for (int i = 0; i < childCount; ++i)
    {
        if (filterAcceptsRow(i, idx))
        {
            return true;
        }
    }
    return false;
}
