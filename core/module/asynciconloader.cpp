#include "asynciconloader.h"
#include <QThreadPool>
#include <QDebug>
#include <QMetaMethod>
#include <QMimeDatabase>
#include <QSettings>
#include "my_icon_provider.h"

AsyncIconLoader::AsyncIconLoader(QObject *parent)
    :QObject(parent)
{
    qRegisterMetaType<QFileInfo>("QFileInfo");
}
QIcon AsyncIconLoader::requestIcon(const QFileInfo &info)
{
    QString path = info.absoluteFilePath();
    {
        QMutexLocker locker(&m_mutex);
        if (m_cache.contains(path))
        {
            return m_cache[path];
        }
        if (m_pending.contains(path))
        {
            return info.isDir() ? QIcon::fromTheme("folder") : QIcon::fromTheme("unknown");
        }
        m_pending.insert(path);
    }
    QtConcurrent::run([this, info]()
    {
        QIcon icon;
        QMimeDatabase db;
        QMimeType mime = db.mimeTypeForFile(info);
        if (mime.name() == "application/x-desktop")
        {
            icon = QIcon();//这要在主进程中生成
        }
        else
        {
            My_Icon_Provider provider;
            icon = provider.icon(info);
        }
        QMetaObject::invokeMethod(const_cast<AsyncIconLoader*>(this), "onIconGenerated", Qt::QueuedConnection, Q_ARG(QFileInfo, info), Q_ARG(QIcon, icon));
    });
    return info.isDir() ? QIcon::fromTheme("folder") : QIcon::fromTheme("unknown");
}
void AsyncIconLoader::onIconGenerated(const QFileInfo &info, const QIcon &icon)
{
    QString path = info.absoluteFilePath();
    QIcon finalIcon = icon;
    if (finalIcon.isNull())
    {
        My_Icon_Provider provider;
        finalIcon = provider.icon(info);
        if (finalIcon.isNull())
        {
            finalIcon = QIcon::fromTheme("unknown");
        }
    }
    {
        QMutexLocker locker(&m_mutex);
        m_cache[path] = finalIcon;
        m_pending.remove(path);
    }
    emit iconReady(info, finalIcon);
}
