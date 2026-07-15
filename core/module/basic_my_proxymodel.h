#ifndef BASIC_MY_PROXYMODEL_H
#define BASIC_MY_PROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QFutureWatcher>
#include <QFileSystemModel>
#include <QTimer>
#include "core/module/flatfilelistmodel.h"

class Tree_View_Root_Interface//C++ OOP的水太深,你把握不住
{
public:
    explicit Tree_View_Root_Interface();
    virtual ~Tree_View_Root_Interface();
    virtual void backToPath();
    QString *root_path_ptr = nullptr;
};

class Basic_My_ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit Basic_My_ProxyModel(QObject *parent = nullptr, Tree_View_Root_Interface *m_root = nullptr, QFileSystemModel *m_fsModel = nullptr);
    void setSearchPattern(const QString &pattern, bool deeply_search = false);
    void setSearchPattern(const QString &pattern, int type);
    void setShowHidden(bool show);
    FlatFileListModel *flatModel() const;
    QModelIndex mapToSource(const QModelIndex &sourceIndex) const override;
    QIcon getIcon(const QModelIndex &sourceIndex);
    void Rename_File(const QModelIndex &proxyIndex, const QString &new_name);
    void refreshAll();
    void refreshFile(const QString &path);
protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
private:
    Tree_View_Root_Interface *root = nullptr;
protected:
    QFileSystemModel *fsModel = nullptr;
private:
    QString m_pattern;
    bool m_showHidden = false;
protected:
    int m_searchType = 0;//0,1,2,顶替原m_deeply_search
private:
    QFuture<bool> hasMatchInSubtreeAsync(const QModelIndex &sourceIndex) const;
    mutable QSet<QString> m_matchedDirsCache;//给const函数用
    mutable QMutex m_cacheMutex;
    mutable QSet<QString> m_pendingDirs;
    mutable QMutex m_pendingMutex;
protected slots:
    void onMatchCheckFinished(const QString &dirPath, bool hasMatch) const;

private slots:
    void addFlatMatches(const QStringList &paths);
    void finishFlatSearch();
    void updateFlatView();//定时更新

private:
    void startFlatSearch(const QString &pattern);

    FlatFileListModel *m_flatModel = new FlatFileListModel(this);
    QFutureWatcher<void> *m_flatWatcher = new QFutureWatcher<void>(this);
    QAtomicInt m_cancelFlat;
    QTimer *m_updateTimer = new QTimer(this);
    QStringList m_pendingAddPaths;//待添加路径
};

class My_ProxyModel : public Basic_My_ProxyModel
{
    Q_OBJECT
public:
    explicit My_ProxyModel(QObject *parent = nullptr, Tree_View_Root_Interface *m_root = nullptr, QFileSystemModel *m_fsModel = nullptr);
protected:
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
};

class My_Table_ProxyModel : public Basic_My_ProxyModel
{
    Q_OBJECT
public:
    explicit My_Table_ProxyModel(QObject *parent = nullptr, Tree_View_Root_Interface *m_root = nullptr, int *m_sort_type = nullptr, QFileSystemModel *m_fsModel = nullptr);
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
protected:
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
private:
    int *sort_type_ptr = nullptr;
};

#endif // BASIC_MY_PROXYMODEL_H
