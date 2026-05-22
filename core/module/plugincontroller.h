#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include "interfaces/dde-dock/v1.2.3/pluginsiteminterface.h"

class Plugin_Root;
class PluginController : public QObject, public PluginProxyInterface
{
    Q_OBJECT
public:
    explicit PluginController(QObject *parent = nullptr, Plugin_Root *plugin_root = nullptr);
    virtual ~PluginController() override;
    virtual void itemAdded(PluginsItemInterface * const itemInter, const QString &itemKey) override;
    virtual void itemUpdate(PluginsItemInterface * const itemInter, const QString &itemKey) override;
    virtual void itemRemoved(PluginsItemInterface * const itemInter, const QString &itemKey) override;
    virtual void requestWindowAutoHide(PluginsItemInterface * const itemInter, const QString &itemKey, const bool autoHide) override;
    virtual void requestRefreshWindowVisible(PluginsItemInterface * const itemInter, const QString &itemKey) override;
    virtual void requestSetAppletVisible(PluginsItemInterface * const itemInter, const QString &itemKey, const bool visible) override;
    virtual void saveValue(PluginsItemInterface * const itemInter, const QString &key, const QVariant &value) override;
    virtual const QVariant getValue(PluginsItemInterface *const itemInter, const QString &key, const QVariant& fallback = QVariant()) override;
    virtual void removeValue(PluginsItemInterface *const itemInter, const QStringList &keyList) override;
private:
    Plugin_Root *root = nullptr;
    QSettings *m_settings = nullptr;
    QString buildKey(PluginsItemInterface *itemInter, const QString &key) const;
};

#endif // PLUGINCONTROLLER_H
