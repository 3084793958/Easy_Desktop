#include "plugincontroller.h"
#include "core/plugin_widget.h"
PluginController::PluginController(QObject *parent, Plugin_Root *plugin_root)
    :QObject(parent)
    ,root(plugin_root)
{
    m_settings = new QSettings("Easy_Desktop", "plugins", this);
}
PluginController::~PluginController()
{
    m_settings->sync();
}
QString PluginController::buildKey(PluginsItemInterface *itemInter, const QString &key) const
{
    if (itemInter)
    {
        return QString("%1/%2").arg(itemInter->pluginName()).arg(key);
    }
    else
    {
        return QString("unknown_plugin/%1").arg(key);
    }
}
void PluginController::itemAdded(PluginsItemInterface * const itemInter, const QString &itemKey)
{
    if (!itemInter) return;
    if (root->has_been_closed) return;
    root->plugin_itemKey = itemKey;
    root->update_plugin(itemInter, itemKey);
    root->item_carrier->plugin_set_size(itemInter);
    root->popup_carrier->plugin_set_size(itemInter);
    root->tips_carrier->plugin_set_size(itemInter);
    QString extra_Data = itemInter->itemContextMenu(itemKey);
    if (extra_Data.isNull() || extra_Data.isEmpty())
    {
        root->item_carrier->set_extra_menu(QString());
    }
    else
    {
        root->item_carrier->set_extra_menu(extra_Data);
    }
    if (root->plugin_disabled)
    {
        root->disable_plugin_update();
    }
}
void PluginController::itemUpdate(PluginsItemInterface * const itemInter, const QString &itemKey)
{
    if (!itemInter) return;
    if (root->has_been_closed) return;
    root->plugin_itemKey = itemKey;
    root->update_plugin(itemInter, itemKey);
    //itemInter->itemContextMenu(itemKey);不能在update中加载
    root->disable_plugin_update();
}
void PluginController::itemRemoved(PluginsItemInterface * const itemInter, const QString &itemKey)
{
    PluginController::itemUpdate(itemInter, itemKey);
}
void PluginController::requestWindowAutoHide(PluginsItemInterface * const itemInter, const QString &itemKey, const bool autoHide)
{
    PluginController::itemUpdate(itemInter, itemKey);
    if (autoHide)
    {
        root->item_carrier->call_to_show();
        if (!root->tips_always_show)
        {
            root->tips_carrier->hide();
        }
        if (!root->popup_always_show)
        {
            root->popup_carrier->hide();
        }
    }
}
void PluginController::requestRefreshWindowVisible(PluginsItemInterface * const itemInter, const QString &itemKey)
{
    PluginController::itemUpdate(itemInter, itemKey);
    root->item_carrier->call_to_show();
    if (!root->tips_always_show)
    {
        root->tips_carrier->hide();
    }
    if (!root->popup_always_show)
    {
        root->popup_carrier->hide();
    }
}
void PluginController::requestSetAppletVisible(PluginsItemInterface * const itemInter, const QString &itemKey, const bool visible)
{
    PluginController::itemUpdate(itemInter, itemKey);
    if (visible)
    {
        root->popup_carrier->call_to_show();
    }
    else
    {
        root->popup_carrier->hide();
    }
}
void PluginController::saveValue(PluginsItemInterface * const itemInter, const QString &key, const QVariant &value)
{
    QString fullKey = buildKey(itemInter, key);
    m_settings->setValue(fullKey, value);
}
const QVariant PluginController::getValue(PluginsItemInterface * const itemInter, const QString &key, const QVariant &fallback)
{
    QString fullKey = buildKey(itemInter, key);
    QVariant value = m_settings->value(fullKey, fallback);
    return value;
}
void PluginController::removeValue(PluginsItemInterface * const itemInter, const QStringList &keyList)
{
    for (const QString &key : keyList)
    {
        QString fullKey = buildKey(itemInter, key);
        m_settings->remove(fullKey);
    }
}
