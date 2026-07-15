#include "basic_my_proxymodel.h"
#include <QtConcurrent/QtConcurrent>
Basic_My_ProxyModel::Basic_My_ProxyModel(QObject *parent, Tree_View_Root_Interface *m_root, QFileSystemModel *m_fsModel)
    :QSortFilterProxyModel(parent)
    ,root(m_root)
    ,fsModel(m_fsModel)
{
    m_updateTimer->setInterval(1000);
    connect(m_updateTimer, &QTimer::timeout, this, &My_ProxyModel::updateFlatView);
    if (fsModel)
    {
        connect(fsModel, &QAbstractItemModel::dataChanged, this, [=]()
        {

        });
    }
}
FlatFileListModel *Basic_My_ProxyModel::flatModel() const
{
    return m_flatModel;
}
void Basic_My_ProxyModel::setSearchPattern(const QString &pattern, bool deeply_search)
{
    setSearchPattern(pattern, deeply_search ? 1 : 0);
}
void Basic_My_ProxyModel::setSearchPattern(const QString &pattern, int type)
{
    if (m_flatWatcher->isRunning())
    {
        m_cancelFlat.ref();
        m_flatWatcher->cancel();
        m_flatWatcher->waitForFinished();
    }
    m_updateTimer->stop();
    m_pendingAddPaths.clear();

    m_pattern = pattern;
    m_searchType = type;

    if (type == 2)
    {
        m_flatModel->clear();
        startFlatSearch(pattern);
        m_updateTimer->start();

    }
    else
    {
        m_flatModel->clear();
        invalidateFilter();
    }
}
void Basic_My_ProxyModel::setShowHidden(bool show)
{
    if (m_showHidden != show)
    {
        m_showHidden = show;
        invalidateFilter();
    }
}
void Basic_My_ProxyModel::onMatchCheckFinished(const QString &dirPath, bool hasMatch) const
{
    if (hasMatch)
    {
        QMutexLocker locker(&m_cacheMutex);
        m_matchedDirsCache.insert(dirPath);
    }

    //新作用域
    {
        QMutexLocker locker(&m_pendingMutex);
        m_pendingDirs.remove(dirPath);
    }
    const_cast<Basic_My_ProxyModel *>(this)->invalidateFilter();//刷新
    root->backToPath();
}
void Basic_My_ProxyModel::addFlatMatches(const QStringList &paths)
{
    if (m_searchType != 2)
    {
        return;
    }
    QMutexLocker locker(&m_pendingMutex);
    m_pendingAddPaths.append(paths);
}
void Basic_My_ProxyModel::finishFlatSearch()
{
    if (m_searchType != 2)
    {
        return;
    }
    m_updateTimer->stop();
    updateFlatView();
}
void Basic_My_ProxyModel::updateFlatView()
{
    if (m_searchType != 2)
    {
        m_updateTimer->stop();
        return;
    }
    QStringList paths;
    {
        QMutexLocker locker(&m_pendingMutex);
        if (m_pendingAddPaths.isEmpty())
        {
            return;
        }
        paths = m_pendingAddPaths;
        m_pendingAddPaths.clear();
    }
    m_flatModel->appendFiles(paths);
}
void Basic_My_ProxyModel::startFlatSearch(const QString &pattern)
{
    if (!root || root->root_path_ptr->isEmpty())
    {
        return;
    }
    QString rootDir = *root->root_path_ptr;
    bool showHidden = m_showHidden;
    m_cancelFlat = 0;
    QFuture<void> future = QtConcurrent::run([this, pattern, rootDir, showHidden]()
    {
        QDirIterator it(rootDir, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | (showHidden ? QDir::Hidden : QDir::NoDot), QDirIterator::Subdirectories);
        QStringList batch;
        while (it.hasNext())
        {
            if (m_cancelFlat.loadRelaxed())
            {
                return;
            }
            it.next();
            if (it.fileName().contains(pattern, Qt::CaseInsensitive))
            {
                batch << it.filePath();
                if (batch.size() >= 50)
                {
                    QMetaObject::invokeMethod(const_cast<Basic_My_ProxyModel *>(this), "addFlatMatches", Qt::QueuedConnection, Q_ARG(QStringList, batch));
                    batch.clear();
                }
            }
        }
        if (!batch.isEmpty())
        {
            QMetaObject::invokeMethod(const_cast<Basic_My_ProxyModel *>(this), "addFlatMatches", Qt::QueuedConnection, Q_ARG(QStringList, batch));
        }
        QMetaObject::invokeMethod(const_cast<Basic_My_ProxyModel *>(this), "finishFlatSearch", Qt::QueuedConnection);
    });
    m_flatWatcher->setFuture(future);
}
bool Basic_My_ProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    if (m_searchType == 2)
    {
        return true;
    }
    QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!sourceIndex.isValid())
    {
        return false;
    }
    QFileInfo fileInfo = fsModel->fileInfo(sourceIndex);
    if (fileInfo.isHidden() && !m_showHidden)
    {
        return false;
    }
    if (m_pattern.isEmpty())
    {
        return true;
    }
    if (m_searchType == 1)
    {
        if (!fileInfo.isDir())
        {
            return fileInfo.fileName().contains(m_pattern, Qt::CaseInsensitive);
        }
        else
        {
            QString dirPath = fileInfo.absoluteFilePath();
            {
                QMutexLocker locker(&m_cacheMutex);
                if (m_matchedDirsCache.contains(dirPath))
                {
                    return true;
                }
            }
            bool shouldStartTask = false;
            {
                QMutexLocker locker(&m_pendingMutex);
                if (!m_pendingDirs.contains(dirPath))
                {
                    m_pendingDirs.insert(dirPath);
                    shouldStartTask = true;
                }
            }
            if (!dirPath.startsWith(*root->root_path_ptr))//怎么还有钉子户啊
            {
                return true;
            }
            if (shouldStartTask)
            {
                QFuture<bool> future = hasMatchInSubtreeAsync(sourceIndex);
                QFutureWatcher<bool> *watcher = new QFutureWatcher<bool>();

                connect(watcher, &QFutureWatcher<bool>::finished, this, [=]()
                {
                    bool result = watcher->result();
                    watcher->deleteLater();

                    //需要在主进程中运行,不然会出现许多奇怪的行为
                    QMetaObject::invokeMethod(const_cast<Basic_My_ProxyModel *>(this),[this, dirPath, result]()
                    {
                        this->onMatchCheckFinished(dirPath, result);
                    });
                });
                watcher->setFuture(future);
            }
            return false;//先不显示,其他算出来在说
        }
    }
    else
    {
        if (fileInfo.isDir())
        {
            return true;
        }
        else
        {
            return fileInfo.fileName().contains(m_pattern, Qt::CaseInsensitive);
        }
    }
    return QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}
QModelIndex Basic_My_ProxyModel::mapToSource(const QModelIndex &sourceIndex) const
{
    if (m_searchType == 2)
    {
        QString filePath = m_flatModel->filePath(sourceIndex);
        return fsModel->index(filePath);
    }
    else
    {
        return QSortFilterProxyModel::mapToSource(sourceIndex);
    }
}
QIcon Basic_My_ProxyModel::getIcon(const QModelIndex &sourceIndex)
{
    if (m_searchType == 2)
    {
        return m_flatModel->getIcon(sourceIndex);
    }
    else
    {
        return fsModel->fileIcon(this->QSortFilterProxyModel::mapToSource(sourceIndex));
    }
}
void Basic_My_ProxyModel::Rename_File(const QModelIndex &proxyIndex, const QString &new_name)
{
    fsModel->setData(mapToSource(proxyIndex), new_name, Qt::EditRole);
    if (m_searchType == 2)
    {
        m_flatModel->Rename_File(proxyIndex, new_name);
    }
}
void Basic_My_ProxyModel::refreshAll()
{
    //这里没有QFSModel的fresh,不想include了
    if (m_searchType == 2)
    {
        m_flatModel->refreshAll();
    }
}
void Basic_My_ProxyModel::refreshFile(const QString &path)
{
    if (m_searchType == 2)
    {
        m_flatModel->refreshFile(path);
    }
}
QFuture<bool> Basic_My_ProxyModel::hasMatchInSubtreeAsync(const QModelIndex &sourceIndex) const
{
    if (!sourceIndex.isValid())
    {
        return QtConcurrent::run([]() -> bool
        {
            return false;
        });
    }

    QString rootPath = fsModel->filePath(sourceIndex);
    QFileInfo rootInfo = fsModel->fileInfo(sourceIndex);
    QString pattern = m_pattern;
    bool showHidden = m_showHidden;

    if (!rootInfo.isDir())
    {
        bool immediateMatch = rootInfo.fileName().contains(pattern, Qt::CaseInsensitive);
        return QtConcurrent::run([immediateMatch]() -> bool
        {
            return immediateMatch;
        });
    }

    char func_m_padding[7] = {};

    return QtConcurrent::run([rootPath, pattern, showHidden, func_m_padding]() -> bool
    {
        (void) func_m_padding;
        QFileInfo rootInfo(rootPath);
        if (rootInfo.fileName().contains(pattern, Qt::CaseInsensitive))
        {
            return true;
        }
        QDirIterator::IteratorFlags flags = QDirIterator::Subdirectories | QDirIterator::FollowSymlinks;
        QDirIterator it(rootPath, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | (showHidden ? QDir::Hidden : QDir::NoDot), flags);
        while (it.hasNext())
        {
            it.next();
            if (it.fileName().contains(pattern, Qt::CaseInsensitive))
            {
                return true;
            }
        }
        return false;
    });
}
My_ProxyModel::My_ProxyModel(QObject *parent, Tree_View_Root_Interface *m_root, QFileSystemModel *m_fsModel)
    :Basic_My_ProxyModel(parent, m_root, m_fsModel)
{}
void My_ProxyModel::sort(int column, Qt::SortOrder order)
{
    if (m_searchType == 2)
    {
        flatModel()->sort(column, order);
    }
    else
    {
        fsModel->sort(column, order);
    }
    invalidate();
}

My_Table_ProxyModel::My_Table_ProxyModel(QObject *parent, Tree_View_Root_Interface *m_root, int *m_sort_type, QFileSystemModel *m_fsModel)
    :Basic_My_ProxyModel(parent, m_root, m_fsModel)
    ,sort_type_ptr(m_sort_type)
{
    flatModel()->sort_type_ptr = m_sort_type;
}
void My_Table_ProxyModel::sort(int column, Qt::SortOrder order)
{
    if (m_searchType == 2)
    {
        flatModel()->sort(column, order);
    }
    else
    {
        QSortFilterProxyModel::sort(0);
    }
}
bool My_Table_ProxyModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
    QFileSystemModel *fsModel = qobject_cast<QFileSystemModel *>(sourceModel());
    if (!fsModel)
    {
        return QSortFilterProxyModel::lessThan(left, right);
    }
    else
    {
        QFileInfo leftInfo = fsModel->fileInfo(left);
        QFileInfo rightInfo = fsModel->fileInfo(right);

        if (*sort_type_ptr == 0)//名称
        {
            if (leftInfo.isDir() && !rightInfo.isDir())
            {
                return true;
            }
            if (!leftInfo.isDir() && rightInfo.isDir())
            {
                return false;
            }
            return leftInfo.fileName() < rightInfo.fileName();
        }
        else if (*sort_type_ptr == 1)//大小
        {
            if (leftInfo.isDir() && !rightInfo.isDir())
            {
                return true;
            }
            if (!leftInfo.isDir() && rightInfo.isDir())
            {
                return false;
            }
            return leftInfo.size() < rightInfo.size();
        }
        else if (*sort_type_ptr == 2)//日期
        {
            if (leftInfo.isDir() && !rightInfo.isDir())
            {
                return true;
            }
            if (!leftInfo.isDir() && rightInfo.isDir())
            {
                return false;
            }
            return leftInfo.lastModified() < rightInfo.lastModified();
        }
        else if (*sort_type_ptr == 3)//名称Z
        {
            if (leftInfo.isDir() && !rightInfo.isDir())
            {
                return true;
            }
            if (!leftInfo.isDir() && rightInfo.isDir())
            {
                return false;
            }
            return leftInfo.fileName() > rightInfo.fileName();
        }
        else if (*sort_type_ptr == 4)//大小Z
        {
            if (leftInfo.isDir() && !rightInfo.isDir())
            {
                return true;
            }
            if (!leftInfo.isDir() && rightInfo.isDir())
            {
                return false;
            }
            return leftInfo.size() > rightInfo.size();
        }
        else if (*sort_type_ptr == 5)//日期Z
        {
            if (leftInfo.isDir() && !rightInfo.isDir())
            {
                return true;
            }
            if (!leftInfo.isDir() && rightInfo.isDir())
            {
                return false;
            }
            return leftInfo.lastModified() > rightInfo.lastModified();
        }
        else
        {
            if (leftInfo.isDir() && !rightInfo.isDir())
            {
                return true;
            }
            if (!leftInfo.isDir() && rightInfo.isDir())
            {
                return false;
            }
            return leftInfo.fileName() < rightInfo.fileName();
        }
    }
}

Tree_View_Root_Interface::Tree_View_Root_Interface()
{}
Tree_View_Root_Interface::~Tree_View_Root_Interface()
{}
void Tree_View_Root_Interface::backToPath()
{}
