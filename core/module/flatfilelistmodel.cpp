#include "flatfilelistmodel.h"
#include <QIcon>
#include <QFileIconProvider>
#include <QDateTime>
#include <QMimeDatabase>
#include <math.h>
#include <QDir>
#include <QCollator>
FlatFileListModel::FlatFileListModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    connect(&m_loader, &AsyncIconLoader::iconReady, this, &FlatFileListModel::onIconReady);
}
void FlatFileListModel::setFileList(const QStringList &paths)
{
    beginResetModel();
    m_paths = paths;
    m_fileInfoCache.clear();
    endResetModel();

    sort(m_sortColumn, m_sortOrder);
}
void FlatFileListModel::appendFiles(const QStringList &paths)
{
    if (paths.isEmpty())
    {
        return;
    }
    int start = m_paths.size();
    beginInsertRows(QModelIndex(), start, start + paths.size() - 1);
    m_paths.append(paths);
    endInsertRows();

    sort(m_sortColumn, m_sortOrder);
}
void FlatFileListModel::clear()
{
    beginResetModel();
    m_paths.clear();
    m_fileInfoCache.clear();
    endResetModel();
}
int FlatFileListModel::rowCount(const QModelIndex &parent) const
{
    (void) parent;
    return m_paths.size();
}
int FlatFileListModel::columnCount(const QModelIndex &parent) const
{
    (void)parent;
    return ColumnCount;
}
QVariant FlatFileListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
    {
        return QVariant();
    }
    switch (section)
    {
    case NameColumn:     return tr("名称");
    case SizeColumn:     return tr("大小");
    case TypeColumn:     return tr("类型");
    case ModifiedColumn: return tr("修改时间");
    default:             return QVariant();
    }
}
void FlatFileListModel::Rename_File(const QModelIndex &proxyIndex, const QString &new_name)
{
    if (!proxyIndex.isValid() || proxyIndex.row() >= m_paths.size())
    {
        return;
    }
    int row = proxyIndex.row();
    QString oldPath = m_paths.at(row);
    QFileInfo oldInfo(oldPath);

    QString newPath = oldInfo.absolutePath() + QDir::separator() + new_name;

    m_paths[row] = newPath;

    m_fileInfoCache.remove(oldPath);
    m_finalCache.remove(oldPath);//不知道改名后图标是否会改变

    QFileInfo newInfo(newPath);
    if (newInfo.exists())
    {
        m_fileInfoCache[newPath] = newInfo;
    }

    QModelIndex topLeft = index(row, 0);
    QModelIndex bottomRight = index(row, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole, Qt::DecorationRole});
    //这里就先不移除了,可能改错名嘛
}
QVariant FlatFileListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_paths.size())
    {
        return QVariant();
    }
    QString path = m_paths.at(index.row());
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

    int col = index.column();

    if (col == NameColumn)
    {
        if (role == Qt::DisplayRole)
        {
            return info.fileName();
        }
        else if (role == Qt::DecorationRole)
        {
            if (m_finalCache.contains(path))
                return m_finalCache[path];
            return m_loader.requestIcon(info);
        }
    }

    if (role == Qt::DisplayRole)
    {
        switch (col)
        {
        case SizeColumn:
        {
            if (info.isDir())
            {
                return QString();
            }
            return formatSize(info.size());
        }
        case TypeColumn:
        {
            QString typeStr;
            if (info.isDir())
            {
                typeStr = tr("文件夹");
            }
            else if (info.isSymLink())
            {
                typeStr = tr("符号链接");
            }
            else if (info.isFile())
            {
                QString suffix = info.suffix().toUpper();
                if (suffix.isEmpty())
                {
                    typeStr = tr("文件");
                }
                else
                {
                    typeStr = tr("%1 文件").arg(suffix);
                }
            }
            else
            {
                typeStr = tr("未知");
            }
            return typeStr;
        }
        case ModifiedColumn:
        {
            return info.lastModified().toString(Qt::LocalDate);
        }
        default:
            return QVariant();
        }
    }
    if (role == Qt::ToolTipRole)
    {
        switch (index.column())
        {
        case 0:
        {
            return info.filePath();
        }
        case 3:
        {
            return info.lastModified().toString(Qt::LocalDate);//只留个时间
        }
        default:
        {
            return info.filePath();
        }
        }
    }
    if (role == FilePathRole)
    {
        return path;
    }
    if (role == IsDirRole)
    {
        return info.isDir();
    }

    return QVariant();
}
QString FlatFileListModel::formatSize(qint64 bytes) const
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

QString FlatFileListModel::filePath(const QModelIndex &index) const
{
    if (!index.isValid() || index.row() >= m_paths.size())
    {
        return QString();
    }
    return m_paths.at(index.row());
}
QStringList FlatFileListModel::file_list() const
{
    return m_paths;
}
void FlatFileListModel::onIconReady(const QFileInfo &info, const QIcon &icon)
{
    QString path = info.absoluteFilePath();
    m_finalCache[path] = icon;
    int row = m_paths.indexOf(path);
    if (row != -1)
    {
        QModelIndex idx = index(row, NameColumn);
        emit dataChanged(idx, idx, {Qt::DecorationRole});
    }
}
QIcon FlatFileListModel::getIcon(const QModelIndex &index)
{
    QString path = m_paths.at(index.row());
    QFileInfo info(path);
    if (m_finalCache.contains(path))
    {
        return m_finalCache[path];
    }
    return m_loader.requestIcon(info);
}

void FlatFileListModel::sort(int column, Qt::SortOrder order)
{
    beginResetModel();

    m_sortColumn = column;
    m_sortOrder = order;

    if (sort_type_ptr)
    {
        switch (*sort_type_ptr)
        {
        case 0:
        {
            column = 0;
            order = Qt::SortOrder::AscendingOrder;
            break;
        }
        case 1:
        {
            column = 1;
            order = Qt::SortOrder::AscendingOrder;
            break;
        }
        case 2:
        {
            column = 3;
            order = Qt::SortOrder::AscendingOrder;
            break;
        }
        case 3:
        {
            column = 0;
            order = Qt::SortOrder::DescendingOrder;
            break;
        }
        case 4:
        {
            column = 1;
            order = Qt::SortOrder::DescendingOrder;
            break;
        }
        case 5:
        {
            column = 3;
            order = Qt::SortOrder::DescendingOrder;
            break;
        }
        default:
        {
            break;
        }
        }
    }

    auto func = [column, order](const QString &a, const QString &b)
    {
        QFileInfo infoA(a);
        QFileInfo infoB(b);
        bool less = false;

        switch (column)
        {
        case NameColumn:
        {
            static QCollator collator;
            collator.setNumericMode(true);
            collator.setCaseSensitivity(Qt::CaseInsensitive);
            less = collator.compare(infoA.fileName(), infoB.fileName()) < 0;
            break;
        }
        case SizeColumn:
        {
            qint64 sizeA = infoA.isDir() ? -1 : infoA.size();
            qint64 sizeB = infoB.isDir() ? -1 : infoB.size();
            less = sizeA < sizeB;
            break;
        }
        case TypeColumn:
        {
            QString typeA = getFileType(infoA);
            QString typeB = getFileType(infoB);
            less = typeA.compare(typeB, Qt::CaseInsensitive) < 0;
            break;
        }
        case ModifiedColumn:
        {
            less = infoA.lastModified() < infoB.lastModified();
            break;
        }
        default:
        {
            less = false;
            break;
        }
        }
        return order == Qt::AscendingOrder ? less : !less;
    };

    std::stable_sort(m_paths.begin(), m_paths.end(), func);

    endResetModel();
}
QString FlatFileListModel::getFileType(const QFileInfo &info)
{
    QString typeStr;
    if (info.isDir())
    {
        typeStr = tr("文件夹");
    }
    else if (info.isSymLink())
    {
        typeStr = tr("符号链接");
    }
    else if (info.isFile())
    {
        QString suffix = info.suffix().toUpper();
        if (suffix.isEmpty())
        {
            typeStr = tr("文件");
        }
        else
        {
            typeStr = tr("%1 文件").arg(suffix);
        }
    }
    else
    {
        typeStr = tr("未知");
    }
    return typeStr;
}
void FlatFileListModel::refreshFile(const QString &path)
{
    if (m_fileInfoCache.contains(path))
    {
        m_fileInfoCache.remove(path);
    }
    if (m_finalCache.contains(path))
    {
        m_finalCache.remove(path);
    }

    int row = m_paths.indexOf(path);
    if (row == -1)
    {
        return;
    }
    QModelIndex topLeft = index(row, 0);
    QModelIndex bottomRight = index(row, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole, Qt::DecorationRole});
}
void FlatFileListModel::refreshAll()
{
    m_fileInfoCache.clear();
    m_finalCache.clear();
    if (m_paths.isEmpty())
    {
        return;
    }
    QModelIndex topLeft = index(0, 0);
    QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);
    emit dataChanged(topLeft, bottomRight, {Qt::DisplayRole, Qt::DecorationRole});
    emit headerDataChanged(Qt::Horizontal, 0, columnCount() - 1);
}
