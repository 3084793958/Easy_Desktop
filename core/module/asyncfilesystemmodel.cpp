#include "asyncfilesystemmodel.h"
#include <math.h>
#include <QDateTime>
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

    if (role == Qt::ToolTipRole)
    {
        switch (index.column())
        {
        case 0:
        {
            return fileInfo(index).filePath();
        }
        case 3:
        {
            return fileInfo(index).lastModified().toString(Qt::LocalDate);//只留个时间
        }
        default:
        {
            return fileInfo(index).filePath();
        }
        }
    }

    if (index.column() == 0)
    {
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

    if (role == Qt::DisplayRole)
    {
        QString path = filePath(index);
        QFileInfo info;

        auto it = m_fileInfoCache.find(path);
        if (it != m_fileInfoCache.end())
        {
            info = it.value();
        }
        else
        {
            info = QFileInfo(path);
            if (info.exists())
            {
                m_fileInfoCache[path] = info;
            }
            else
            {
                return QVariant();
            }
        }
        if (index.column() == 1)
        {
            if (info.isDir())
            {
                return QString();//这里就不开QFuture去算文件夹大小了
            }
            return formatSize(info.size());
        }
        else if (index.column() == 3)
        {
            return info.lastModified().toString(Qt::LocalDate);
        }
    }
    return QFileSystemModel::data(index, role);
}
void AsyncFileSystemModel::refreshFile(const QString &path)
{
    if (m_fileInfoCache.contains(path))
    {
        m_fileInfoCache.remove(path);
    }
    QModelIndex idx = index(path);
    if (idx.isValid())
    {
        QModelIndex topLeft = idx.sibling(idx.row(), 0);
        QModelIndex bottomRight = idx.sibling(idx.row(), columnCount() - 1);
        emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole});
    }
}
void AsyncFileSystemModel::refreshAll()
{
    QStringList paths = m_fileInfoCache.keys();

    m_fileInfoCache.clear();

    for (const QString &path : paths)
    {
        QModelIndex idx = index(path);
        if (idx.isValid())
        {
            QModelIndex topLeft = idx.sibling(idx.row(), 0);
            QModelIndex bottomRight = idx.sibling(idx.row(), columnCount() - 1);
            emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole});
        }
    }
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
QString AsyncFileSystemModel::formatSize(qint64 bytes) const
{
    const char* units[] = {"B", "KiB", "MiB", "GiB"};
    double value = bytes;
    int unitIdx = 0;
    while (unitIdx < 3 && value > 2048.0)
    {
        value /= 1024.0;
        ++unitIdx;
    }
    return QString("%1 %2").arg(std::round(value * 100) / 100).arg(units[unitIdx]);
}
