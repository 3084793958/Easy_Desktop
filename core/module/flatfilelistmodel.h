#ifndef FLATFILELISTMODEL_H
#define FLATFILELISTMODEL_H

#include <QAbstractTableModel>
#include "asynciconloader.h"

class FlatFileListModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    enum Column
    {
        NameColumn = 0,
        SizeColumn,
        TypeColumn,
        ModifiedColumn,
        ColumnCount//共4列
    };
    enum Roles
    {
        FilePathRole = Qt::UserRole + 1,
        IsDirRole = Qt::UserRole + 2
    };

    int *sort_type_ptr = nullptr;

    explicit FlatFileListModel(QObject *parent = nullptr);

    void setFileList(const QStringList &paths);
    void appendFiles(const QStringList &paths);
    void clear();
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QString filePath(const QModelIndex &index) const;
    QStringList file_list() const;
    QIcon getIcon(const QModelIndex &index);

    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void Rename_File(const QModelIndex &proxyIndex, const QString &new_name);

    void sort(int column, Qt::SortOrder order) override;
    static QString getFileType(const QFileInfo &info);

    void refreshAll();
    void refreshFile(const QString &path);
private:
    QString formatSize(qint64 bytes) const;
    QStringList m_paths;
    mutable QHash<QString, QFileInfo> m_fileInfoCache;
private slots:
    void onIconReady(const QFileInfo &info, const QIcon &icon);
private:
    mutable AsyncIconLoader m_loader;
    mutable QHash<QString, QIcon> m_finalCache;
private:
    int m_sortColumn = NameColumn;
    Qt::SortOrder m_sortOrder = Qt::AscendingOrder;
};

#endif // FLATFILELISTMODEL_H
