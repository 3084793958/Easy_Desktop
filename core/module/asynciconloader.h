#ifndef ASYNCICONLOADER_H
#define ASYNCICONLOADER_H

#include <QObject>
#include <QFileInfo>
#include <QIcon>
#include <QHash>
#include <QMutex>

#include <QtConcurrent/QtConcurrentRun>

class AsyncIconLoader : public QObject
{
    Q_OBJECT
public:
    explicit AsyncIconLoader(QObject *parent = nullptr);
    QIcon requestIcon(const QFileInfo &info);
signals:
    void iconReady(const QFileInfo &info, const QIcon &icon);
private slots:
    void onIconGenerated(const QFileInfo &info, const QIcon &icon);
private:
    QMutex m_mutex;
    QHash<QString, QIcon> m_cache;
    QSet<QString> m_pending;
};

#endif // ASYNCICONLOADER_H
