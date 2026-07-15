#include "zip_view_model.h"
#include <QModelIndex>
Zip_View_Model::Zip_View_Model(QObject *parent)
    :QStandardItemModel(parent)
{}
QVariant Zip_View_Model::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }
    if (role == Qt::ToolTipRole)
    {

        switch (index.column())
        {
        case 3:
        {
            return index.data();
        }
        default:
        {
            auto ptr = QStandardItemModel::index(index.row(), 0, index.parent());
            return ptr.data();
        }
        }
    }
    return QStandardItemModel::data(index, role);
}
