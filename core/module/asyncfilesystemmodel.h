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
private slots:
    void onIconReady(const QFileInfo &info, const QIcon &icon);
private:
    mutable AsyncIconLoader m_loader;
    mutable QHash<QString, QIcon> m_finalCache;
};

#endif // ASYNCFILESYSTEMMODEL_H
