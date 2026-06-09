#include "asyncfilesystemmodel.h"

AsyncFileSystemModel::AsyncFileSystemModel(QObject *parent)
    : QFileSystemModel(parent)
{
    connect(&m_loader, &AsyncIconLoader::iconReady, this, &AsyncFileSystemModel::onIconReady);
}
QVariant AsyncFileSystemModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (index.column() != 0)
    {
        return QFileSystemModel::data(index, role);
    }

    if (role == Qt::DecorationRole)
    {
        QString path = filePath(index);
        if (m_finalCache.contains(path))
            return m_finalCache[path];

        QFileInfo info = fileInfo(index);
        QIcon placeholder = m_loader.requestIcon(info);
        return placeholder;
    }

    return QFileSystemModel::data(index, role);
}
void AsyncFileSystemModel::onIconReady(const QFileInfo &info, const QIcon &icon)
{
    QString path = info.absoluteFilePath();
    m_finalCache[path] = icon;
    QModelIndex idx = index(path);
    if (idx.isValid())
    {
        emit dataChanged(idx, idx, {Qt::DecorationRole});
    }
}
