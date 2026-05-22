#ifndef PLUGINCONTROLLER_V_2_0_0_H
#define PLUGINCONTROLLER_V_2_0_0_H
#include "interfaces/dde-dock/v2.0.0/pluginsiteminterface_v2.0.0.h"

class Plugin_Root;
class PluginController_V_2_0_0 : public QObject, public PluginProxyInterface_V_2_0_0
{
    Q_OBJECT
public:
    explicit PluginController_V_2_0_0(QObject *parent = nullptr, Plugin_Root *plugin_root = nullptr);
    virtual ~PluginController_V_2_0_0() override;
    virtual void itemAdded(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey) override;
    virtual void itemUpdate(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey) override;
    virtual void itemRemoved(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey) override;
    virtual void requestWindowAutoHide(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey, const bool autoHide) override;
    virtual void requestRefreshWindowVisible(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey) override;
    virtual void requestSetAppletVisible(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey, const bool visible) override;
    virtual void saveValue(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &key, const QVariant &value) override;
    virtual const QVariant getValue(PluginsItemInterface_V_2_0_0 *const itemInter, const QString &key, const QVariant& fallback = QVariant()) override;
    virtual void removeValue(PluginsItemInterface_V_2_0_0 *const itemInter, const QStringList &keyList) override;
    virtual void updateDockInfo(PluginsItemInterface_V_2_0_0 *const, const DockPart &) override;
private:
    Plugin_Root *root = nullptr;
    QSettings *m_settings = nullptr;
    QString buildKey(PluginsItemInterface_V_2_0_0 *itemInter, const QString &key) const;
};

#endif // PLUGINCONTROLLER_V_2_0_0_H
