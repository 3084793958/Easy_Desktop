#ifndef ASYNCFILESYSTEMMODEL_H
#define ASYNCFILESYSTEMMODEL_H
#include <QFileSystemModel>
#include "asynciconloader.h"

class AsyncFileSystemModel : public QFileSystemModel
{
    Q_OBJECT
public:
    explicit AsyncFileSystemModel(QObject *parent = nullptr);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    void refreshFile(const QString &path);
    void refreshAll();
private slots:
    void onIconReady(const QFileInfo &info, const QIcon &icon);
private:

    QString formatSize(qint64 bytes) const;

    mutable AsyncIconLoader m_loader;
    mutable QHash<QString, QIcon> m_finalCache;

    mutable QHash<QString, QFileInfo> m_fileInfoCache;
};

#endif // ASYNCFILESYSTEMMODEL_H
