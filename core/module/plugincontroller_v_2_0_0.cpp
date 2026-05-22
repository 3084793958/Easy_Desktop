#include "plugincontroller_v_2_0_0.h"
#include "core/plugin_widget.h"
PluginController_V_2_0_0::PluginController_V_2_0_0(QObject *parent, Plugin_Root *plugin_root)
    :QObject(parent)
    ,root(plugin_root)
{
    m_settings = new QSettings("Easy_Desktop", "plugins", this);
}
PluginController_V_2_0_0::~PluginController_V_2_0_0()
{
    m_settings->sync();
}
QString PluginController_V_2_0_0::buildKey(PluginsItemInterface_V_2_0_0 *itemInter, const QString &key) const
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
void PluginController_V_2_0_0::itemAdded(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey)
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
void PluginController_V_2_0_0::itemUpdate(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey)
{
    if (!itemInter) return;
    if (root->has_been_closed) return;
    root->plugin_itemKey = itemKey;
    root->update_plugin(itemInter, itemKey);
    //itemInter->itemContextMenu(itemKey);不能在update中加载
    root->disable_plugin_update();
}
void PluginController_V_2_0_0::itemRemoved(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey)
{
    PluginController_V_2_0_0::itemUpdate(itemInter, itemKey);
}
void PluginController_V_2_0_0::requestWindowAutoHide(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey, const bool autoHide)
{
    PluginController_V_2_0_0::itemUpdate(itemInter, itemKey);
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
void PluginController_V_2_0_0::requestRefreshWindowVisible(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey)
{
    PluginController_V_2_0_0::itemUpdate(itemInter, itemKey);
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
void PluginController_V_2_0_0::requestSetAppletVisible(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey, const bool visible)
{
    PluginController_V_2_0_0::itemUpdate(itemInter, itemKey);
    if (visible)
    {
        root->popup_carrier->call_to_show();
    }
    else
    {
        root->popup_carrier->hide();
    }
}
void PluginController_V_2_0_0::saveValue(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &key, const QVariant &value)
{
    QString fullKey = buildKey(itemInter, key);
    m_settings->setValue(fullKey, value);
}
const QVariant PluginController_V_2_0_0::getValue(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &key, const QVariant &fallback)
{
    QString fullKey = buildKey(itemInter, key);
    QVariant value = m_settings->value(fullKey, fallback);
    return value;
}
void PluginController_V_2_0_0::removeValue(PluginsItemInterface_V_2_0_0 * const itemInter, const QStringList &keyList)
{
    for (const QString &key : keyList)
    {
        QString fullKey = buildKey(itemInter, key);
        m_settings->remove(fullKey);
    }
}
void PluginController_V_2_0_0::updateDockInfo(PluginsItemInterface_V_2_0_0 * const itemInter, const DockPart &)
{
    PluginController_V_2_0_0::itemUpdate(itemInter, root->plugin_itemKey);//DockPart对于Easy_Desktop没有意义
}
