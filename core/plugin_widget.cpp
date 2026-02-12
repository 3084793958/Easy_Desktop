#include "plugin_widget.h"
#include <QX11Info>
#include <X11/Xlib.h>
void Plugin_Root::X11_Raise()
{
    Window win_Id = static_cast<Window>(WinId);
    Display *display = QX11Info::display();
    XRaiseWindow(display, win_Id);
    XFlush(display);
}
#undef CursorShape
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
    root->item_widget = itemInter->itemWidget(itemKey);
    root->tips_widget = itemInter->itemTipsWidget(itemKey);
    root->popup_widget = itemInter->itemPopupApplet(itemKey);
    root->item_carrier->remove_widget();
    root->item_carrier->set_widget(root->item_widget);
    if (root->item_widget)
    {
        root->item_widget->setStyleSheet(root->style_sheet);
        root->item_widget->update();
        root->item_widget->show();
    }
    if (root->follow_plugin_show_after_load) root->item_carrier->plugin_set_size();
    root->item_carrier->call_to_show();

    root->tips_carrier->remove_widget();
    root->tips_carrier->set_widget(root->tips_widget);
    if (root->tips_widget)
    {
        root->tips_widget->setStyleSheet(root->style_sheet);
        root->tips_widget->update();
        root->tips_widget->show();
    }
    if (root->follow_plugin_show_after_load) root->tips_carrier->plugin_set_size();
    if (root->tips_always_show) root->tips_carrier->call_to_show();

    root->popup_carrier->remove_widget();
    root->popup_carrier->set_widget(root->popup_widget);
    if (root->popup_widget)
    {
        root->popup_widget->setStyleSheet(root->style_sheet);
        root->popup_widget->update();
        root->popup_widget->show();
    }
    if (root->follow_plugin_show_after_load) root->popup_carrier->plugin_set_size();
    if (root->popup_always_show) root->popup_carrier->call_to_show();

    if (root->follow_plugin_show_after_load)
    {
        root->follow_plugin_show_after_load = false;
    }
    root->plugin_itemKey = itemKey;
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
    root->item_widget = itemInter->itemWidget(itemKey);
    root->tips_widget = itemInter->itemTipsWidget(itemKey);
    root->popup_widget = itemInter->itemPopupApplet(itemKey);
    root->item_carrier->remove_widget();
    root->item_carrier->set_widget(root->item_widget);
    if (root->item_widget)
    {
        root->item_widget->setStyleSheet(root->style_sheet);
        root->item_widget->update();
        root->item_widget->show();
    }
    if (root->follow_plugin_show_after_load) root->item_carrier->plugin_set_size();
    root->item_carrier->call_to_show();

    root->tips_carrier->remove_widget();
    root->tips_carrier->set_widget(root->tips_widget);
    if (root->tips_widget)
    {
        root->tips_widget->setStyleSheet(root->style_sheet);
        root->tips_widget->update();
        root->tips_widget->show();
    }
    if (root->follow_plugin_show_after_load) root->tips_carrier->plugin_set_size();
    if (root->tips_always_show) root->tips_carrier->call_to_show();

    root->popup_carrier->remove_widget();
    root->popup_carrier->set_widget(root->popup_widget);
    if (root->popup_widget)
    {
        root->popup_widget->setStyleSheet(root->style_sheet);
        root->popup_widget->update();
        root->popup_widget->show();
    }
    if (root->follow_plugin_show_after_load) root->popup_carrier->plugin_set_size();
    if (root->popup_always_show) root->popup_carrier->call_to_show();
    root->plugin_itemKey = itemKey;
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
    (void)autoHide;
}
void PluginController::requestRefreshWindowVisible(PluginsItemInterface * const itemInter, const QString &itemKey)
{
    PluginController::itemUpdate(itemInter, itemKey);
}
void PluginController::requestSetAppletVisible(PluginsItemInterface * const itemInter, const QString &itemKey, const bool visible)
{
    PluginController::itemUpdate(itemInter, itemKey);
    (void)visible;
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
Plugin_Root::Plugin_Root(QWidget *parent)
    :QObject(parent)
{
    desktop_parent = parent;
    item_carrier->is_item = true;
    //load_plugin("/home/deepin/Desktop/QT/music-island-B-QT-P-main/build-music-island-unknown-Debug/libMusic-Island.so");
    connect(item_carrier, &Plugin_Item_Widget::real_close_event, this, [=]
    {
        unload_plugin();
        if (plugin_root_list)
        {
            plugin_root_list->removeOne(this);
        }
        has_been_closed = true;
        this->deleteLater();
    });
    connect(item_carrier, &Plugin_Item_Widget::Released, this, [=]
    {
        if (popup_always_show || popup_carrier->isHidden())
        {
            if (plugin_disabled) return;
            QObject *pluginInstance = plugin_loader->instance();
            if (pluginInstance)
            {
                PluginsItemInterface *plugin_interface = qobject_cast<PluginsItemInterface *>(pluginInstance);
                if (plugin_interface)
                {
                    plugin_controller->itemUpdate(plugin_interface, plugin_itemKey);
                }
            }
            popup_carrier->call_to_show();
        }
        else
        {
            popup_carrier->hide();
        }
    });
    connect(item_carrier, &Plugin_Item_Widget::Hover, this, [=]
    {
        if (plugin_disabled) return;
        QObject *pluginInstance = plugin_loader->instance();
        if (pluginInstance)
        {
            PluginsItemInterface *plugin_interface = qobject_cast<PluginsItemInterface *>(pluginInstance);
            if (plugin_interface)
            {
                plugin_controller->itemUpdate(plugin_interface, plugin_itemKey);
            }
        }
        tips_carrier->call_to_show();
    });
    connect(item_carrier, &Plugin_Item_Widget::Hover_end, this, [=]
    {
        if (!tips_always_show)
        {
            tips_carrier->hide();
        }
    });
    connect(item_carrier, &Plugin_Item_Widget::Call_X11_Raise, this, &Plugin_Root::X11_Raise);
    connect(item_carrier, &Plugin_Item_Widget::extra_menu_call, this, [=](QString menuId, bool checked)
    {
        QObject *pluginInstance = plugin_loader->instance();
        if (pluginInstance)
        {
            PluginsItemInterface *plugin_interface = qobject_cast<PluginsItemInterface *>(pluginInstance);
            plugin_interface->invokedMenuItem(plugin_itemKey, menuId, checked);
        }
    });
    connect(item_carrier, &Basic_Widget::close_signals, this, [=]
    {
        only_hide();
    });
}
void Plugin_Root::click_call()
{
    QObject *pluginInstance = plugin_loader->instance();
    if (pluginInstance)
    {
        PluginsItemInterface *plugin_interface = qobject_cast<PluginsItemInterface *>(pluginInstance);
        if (plugin_interface)
        {
            QString command = plugin_interface->itemCommand(plugin_itemKey);
            if (command.isNull() || command.isEmpty()) return;
            QProcess process;
            process.setProgram("/bin/bash");
            process.setArguments(QStringList() << "-c" << command);
            process.setStandardOutputFile("/dev/null");
            process.setStandardErrorFile("/dev/null");
            process.startDetached();
        }
    }
}
void Plugin_Root::update_plugin()
{
    QObject *pluginInstance = plugin_loader->instance();
    if (pluginInstance)
    {
        PluginsItemInterface *plugin_interface = qobject_cast<PluginsItemInterface *>(pluginInstance);
        if (plugin_interface)
        {
            plugin_controller->itemUpdate(plugin_interface, plugin_itemKey);
        }
    }
}
Plugin_Root::~Plugin_Root()
{
    //this->setParent(nullptr);
    only_hide();
    return;//偷天还日
}
void Plugin_Root::only_hide()
{
    this->setParent(nullptr);
    if (!plugin_loader->isLoaded())
    {
        unload_plugin();
        if (plugin_root_list)
        {
            plugin_root_list->removeOne(this);
        }
        has_been_closed = true;
        this->deleteLater();
        return;
    }
    if (has_been_closed) return;
    has_been_closed = true;
    QObject *pluginInstance = plugin_loader->instance();
    if (pluginInstance)
    {
        PluginsItemInterface *plugin_interface = qobject_cast<PluginsItemInterface *>(pluginInstance);
        if (plugin_interface)
        {
            if (plugin_interface->pluginIsAllowDisable())
            {
                if (!plugin_interface->pluginIsDisable())
                {
                    plugin_interface->pluginStateSwitched();
                }
            }
        }
    }
    if (plugin_root_list)
    {
        plugin_root_list->removeOne(this);
    }
    item_carrier->is_item = false;
    plugin_path.clear();
    item_carrier->remove_widget();
    if (item_widget) item_widget->hide();
    tips_carrier->remove_widget();
    if (tips_widget) tips_widget->hide();
    popup_carrier->remove_widget();
    if (popup_widget) popup_widget->hide();
    item_carrier->hide();
    tips_carrier->hide();
    popup_carrier->hide();
    item_carrier->deleteLater();
    tips_carrier->deleteLater();
    popup_carrier->deleteLater();
}
void Plugin_Root::set_now_page(int *m_now_page)
{
    now_page = m_now_page;
}
void Plugin_Root::set_desktop_number(int *m_desktop_number)
{
    desktop_number = m_desktop_number;
}
void Plugin_Root::set_basic_list(QList<QWidget *> *m_basic_list)
{
    basic_list = m_basic_list;
}
void Plugin_Root::set_root_pos(QPoint pos)
{
    root_pos = pos;
}
void Plugin_Root::call_update_plugin_carrier()
{
    item_carrier->setParent(desktop_parent);
    item_carrier->set_now_page(now_page);
    item_carrier->set_desktop_number(desktop_number);
    item_carrier->set_basic_list(basic_list);
    item_carrier->move(root_pos);
    tips_carrier->setParent(desktop_parent);
    tips_carrier->set_now_page(now_page);
    tips_carrier->set_desktop_number(desktop_number);
    tips_carrier->set_basic_list(basic_list);
    tips_carrier->move(root_pos);
    popup_carrier->setParent(desktop_parent);
    popup_carrier->set_now_page(now_page);
    popup_carrier->set_desktop_number(desktop_number);
    popup_carrier->set_basic_list(basic_list);
    popup_carrier->move(root_pos);
    item_carrier->call_to_show();
}
void Plugin_Root::load_plugin(QString filepath)
{
    unload_plugin();
    if (filepath.isEmpty()) return;
    plugin_loader->setFileName(filepath);
    if (!plugin_loader->load())
    {
        qDebug() << "插件导入失败:" << plugin_loader->errorString();
        return;
    }
    QObject *pluginInstance = plugin_loader->instance();
    if (!pluginInstance)
    {
        plugin_loader->unload();
        return;
    }
    PluginsItemInterface *plugin_interface = qobject_cast<PluginsItemInterface *>(pluginInstance);
    if (plugin_interface)
    {
        plugin_interface->init(plugin_controller);
        plugin_controller->itemUpdate(plugin_interface, "");
    }
}
void Plugin_Root::unload_plugin()
{
    if (!plugin_loader->isLoaded()) return;
    QObject *pluginInstance = plugin_loader->instance();
    if (pluginInstance)
    {
        PluginsItemInterface *plugin_interface = qobject_cast<PluginsItemInterface *>(pluginInstance);
        if (plugin_interface)
        {
            if (plugin_interface->pluginIsAllowDisable() && !plugin_interface->pluginIsDisable())
            {
                plugin_interface->pluginStateSwitched();
            }
        }
    }
    item_carrier->set_extra_menu(QString());
    item_carrier->remove_widget();
    tips_carrier->remove_widget();
    popup_carrier->remove_widget();
    if (item_widget) item_widget->hide();
    if (tips_widget) tips_widget->hide();
    if (popup_widget) popup_widget->hide();
    item_widget = nullptr;
    tips_widget = nullptr;
    popup_widget = nullptr;
    plugin_loader->unload();//奇怪点:手动禁用有效,自动无效
    plugin_path.clear();
    follow_plugin_show_after_load = true;
}
void Plugin_Root::save(QSettings *settings)
{
    settings->setValue("plugin_path", plugin_path);
    settings->setValue("tips_always_show", tips_always_show);
    settings->setValue("plugin_disabled", plugin_disabled);
    item_carrier->P_save(settings, "item_");
    tips_carrier->P_save(settings, "tips_");
    popup_carrier->P_save(settings, "popup_");
}
void Plugin_Root::load(QSettings *settings)
{
    follow_plugin_show_after_load = false;
    plugin_path = settings->value("plugin_path", "").toString();
    tips_always_show = settings->value("tips_always_show", false).toBool();
    plugin_disabled = settings->value("plugin_disabled", false).toBool();
    item_carrier->p_load(settings, "item_");
    tips_carrier->p_load(settings, "tips_");
    popup_carrier->p_load(settings, "popup_");
    load_plugin(plugin_path);
    update_plugin();
    emit item_carrier->size_changed(item_carrier->get_self()->size());
    emit tips_carrier->size_changed(tips_carrier->get_self()->size());
    emit popup_carrier->size_changed(popup_carrier->get_self()->size());
}
void Plugin_Root::set_icon(QString checked_icon_path)
{
    item_carrier->set_icon(checked_icon_path);
    tips_carrier->set_icon(checked_icon_path);
    popup_carrier->set_icon(checked_icon_path);
}
void Plugin_Root::disable_plugin_update()
{
    QObject *pluginInstance = plugin_loader->instance();
    if (pluginInstance)
    {
        PluginsItemInterface *plugin_interface = qobject_cast<PluginsItemInterface *>(pluginInstance);
        if (plugin_interface)
        {
            if (plugin_interface->pluginIsAllowDisable())
            {
                if (plugin_interface->pluginIsDisable() && !plugin_disabled)
                {
                    plugin_interface->pluginStateSwitched();
                }
                else if (!plugin_interface->pluginIsDisable() && plugin_disabled)
                {
                    plugin_interface->pluginStateSwitched();
                }
            }
        }
    }
    if (plugin_disabled)
    {
        if (item_widget) item_widget->hide();
        item_carrier->show();
        tips_carrier->hide();
        popup_carrier->hide();
    }
    else
    {
        if (item_widget) item_widget->show();
        item_carrier->call_to_show();
    }
}
void Plugin_Root::update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, QString checked_icon_path)
{
    style_sheet = QString("QWidget{background:rgba(0,0,0,0);color:rgba(0,0,0,255)}"
                          "QMenu{border-radius:10px 10px;background:rgba(%1,%2,%3,%4);margin:0px -1px 0px -1px;padding-top:8px;padding-bottom:8px;icon-size:20px;border-radius:10px 10px}"
                          "QMenu::item{color:rgba(%5,%6,%7,%8);background:rgba(0,0,0,0);}"
                          "QMenu::item:disabled{color:rgba(%9,%10,%11,%12)}"
                          "QMenu::item:selected{color:rgba(%13,%14,%15,%16);background:rgba(%17,%18,%19,%20)}"
                          "QMenu::indicator{width: 20px; height 20px; background: transparent;}"
                          "QMenu::indicator:checked{image: url(%21);}"
                          "QMenu::separator{background:rgba(150,150,150,125)}"
                          "QScrollBar:vertical{border:none;background:rgba(0,0,0,0);width:8px;margin:0px0px0px0px;}"
                          "QScrollBar::handle:vertical{background:rgba(0,0,0,75);border-radius:4px;min-height:20px;}"
                          "QScrollBar::handle:vertical:hover{background:rgba(0,0,0,125);}"
                          "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0px;}"
                          "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}"
                          "QScrollBar:horizontal{border:none;background:rgba(0,0,0,0);height:8px;margin:0px0px0px0px;}"
                          "QScrollBar::handle:horizontal{background:rgba(0,0,0,75);border-radius:4px;min-width:20px;}"
                          "QScrollBar::handle:horizontal:hover{background:rgba(0,0,0,125);}"
                          "QScrollBar::add-line:horizontal,QScrollBar::sub-line:horizontal{width:0px;}"
                          "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:none;}")
                  .arg(theme_background_color.red()).arg(theme_background_color.green()).arg(theme_background_color.blue()).arg(theme_background_color.alpha())
                  .arg(theme_text_color.red()).arg(theme_text_color.green()).arg(theme_text_color.blue()).arg(theme_text_color.alpha())
                  .arg(disabled_text_color.red()).arg(disabled_text_color.green()).arg(disabled_text_color.blue()).arg(disabled_text_color.alpha())
                  .arg(select_text_color.red()).arg(select_text_color.green()).arg(select_text_color.blue()).arg(select_text_color.alpha())
                  .arg(theme_color.red()).arg(theme_color.green()).arg(theme_color.blue()).arg(theme_color.alpha())
                  .arg(checked_icon_path);
    if (item_widget) item_widget->setStyleSheet(style_sheet);
    if (tips_widget) tips_widget->setStyleSheet(style_sheet);
    if (popup_widget) popup_widget->setStyleSheet(style_sheet);
}
Plugin_Widget::Plugin_Widget(QWidget *parent)
    :Basic_Widget(parent)
{
    auto_close = false;
    menu->addAction(set_distance_action);
    menu->addAction(set_delta_action);
    Basic_Widget::basic_context(menu);
    carrier->setMouseTracking(true);
    carrier->setStyleSheet("background:rgba(255,255,255,0)");
    connect(this, &Basic_Widget::size_changed, this, [=](QSize size)
    {
        carrier->move(distance_width + delta_x, distance_height + delta_y);
        carrier->resize(size - QSize(2 * distance_width, 2 * distance_height));;
        if (save_ptr)
        {
            save_ptr->resize(carrier->size());
        }
    });
    resize(300, 200);
    hide();
}
void Plugin_Widget::P_save(QSettings *settings, QString Token)
{
    settings->setValue(Token + "pos", this->pos());
    settings->setValue(Token + "size", this->get_self()->size());
    settings->setValue(Token + "show_close", show_close_button->isIconVisibleInMenu());
    settings->setValue(Token + "background_color", background_color.rgba());
    settings->setValue(Token + "background_radius", background_radius);
    int res = -1;
    for (int i = 0; i < basic_list->count(); i++)
    {
        if (this->parentWidget() != nullptr && this->parentWidget() == basic_list->at(i))
        {
            res = i;
            break;
        }
    }
    settings->setValue(Token + "in_page", res);
    settings->setValue(Token + "distance_width", distance_width);
    settings->setValue(Token + "distance_height", distance_height);
    settings->setValue(Token + "delta_x", delta_x);
    settings->setValue(Token + "delta_y", delta_y);
}
void Plugin_Widget::p_load(QSettings *settings, QString Token)
{
    this->move(settings->value(Token + "pos", QPoint(0, 0)).toPoint());
    this->resize(settings->value(Token + "size", QSize(200, 100)).toSize());
    bool show_close_button_bool = settings->value(Token + "show_close", true).toBool();
    show_close_button->setIconVisibleInMenu(show_close_button_bool);
    close_button->setVisible(show_close_button_bool);
    background_color = QColor::fromRgba(settings->value(Token + "background_color", QColor(0,0,0,50).rgba()).toUInt());
    background_radius = settings->value(Token + "background_radius", 10).toInt();
    this->get_self()->setStyleSheet(QString("border-radius: %1px %1px;background:rgba(%2,%3,%4,%5)").arg(background_radius).arg(background_color.red()).arg(background_color.green()).arg(background_color.blue()).arg(background_color.alpha()));
    int res = settings->value(Token + "in_page", -1).toInt();
    if (res >= 0)
    {
        if (res >= basic_list->count())
        {
            res = basic_list->count() - 1;
        }
        this->setParent(basic_list->at(res));
    }
    int new_width = settings->value(Token + "distance_width", 20).toInt();
    int new_height = settings->value(Token + "distance_height", 20).toInt();
    distance_width = new_width;
    distance_height = new_height;
    int new_delta_x = settings->value(Token + "delta_x", 0).toInt();
    int new_delta_y = settings->value(Token + "delta_y", 0).toInt();
    delta_x = new_delta_x;
    delta_y = new_delta_y;
    emit Basic_Widget::size_changed(this->get_self()->size());
}
void Plugin_Widget::set_widget(QWidget *ptr)
{
    save_ptr = ptr;
    if (save_ptr)
    {
        save_ptr->setParent(carrier);
        save_ptr->setMouseTracking(true);
        save_ptr->show();
    }
}
void Plugin_Widget::remove_widget()
{
    if (save_ptr)
    {
        save_ptr->disconnect();
        save_ptr->setParent(nullptr);
        save_ptr->hide();
        save_ptr = nullptr;
    }
}
void Plugin_Widget::plugin_set_size()
{
    if (!save_ptr) return;
    save_ptr->adjustSize();
    QSize res = save_ptr->sizeHint();
    if (!res.isValid())
    {
        res = save_ptr->size();
    }
    this->resize(res + QSize(2 * distance_width, 2 * distance_height));
}
void Plugin_Widget::call_to_show()
{
    if (is_item)
    {
        this->show();
        return;
    }
    if (save_ptr)
    {
        save_ptr->show();
        this->show();
        if (!this->isVisible())
        {
            this->showNormal();//逆天操作
            this->lower();
        }
    }
    else
    {
        this->hide();
    }
}
void Plugin_Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == set_distance_action)
    {
        bool ok = false;
        int new_width = QInputDialog::getInt(nullptr, "获取数值", "宽度距离:", distance_width, 0, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int new_height = QInputDialog::getInt(nullptr, "获取数值", "高度距离:", distance_height, 0, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        distance_width = new_width;
        distance_height = new_height;
        emit Basic_Widget::size_changed(this->get_self()->size());
    }
    else if (know_what == set_delta_action)
    {
        bool ok = false;
        int new_x = QInputDialog::getInt(nullptr, "获取数值", "X偏移:", delta_x, -2147483647, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int new_y = QInputDialog::getInt(nullptr, "获取数值", "Y偏移:", delta_y, -2147483647, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        delta_x = new_x;
        delta_y = new_y;
        emit Basic_Widget::size_changed(this->get_self()->size());
    }
    else
    {
        basic_action_func(know_what);
    }
}
void Plugin_Widget::wheelEvent(QWheelEvent *event)
{
    if (save_ptr)
    {
        event->accept();
        return;
    }
}
Plugin_Item_Widget::Plugin_Item_Widget(QWidget *parent, Plugin_Root *plugin_root)
    :Plugin_Widget(parent)
    ,root(plugin_root)
{
    setMouseTracking(true);
    this->activateWindow();
    tips_always_show_action->setIconVisibleInMenu(false);
    tips_always_show_action->setIcon(QIcon(":/base/this.svg"));
    popup_always_show_action->setIconVisibleInMenu(false);
    popup_always_show_action->setIcon(QIcon(":/base/this.svg"));
    disable_plugin_action->setIconVisibleInMenu(false);
    disable_plugin_action->setIcon(QIcon(":/base/this.svg"));
    menu->insertAction(set_distance_action, set_plugin_path_action);
    menu->insertAction(set_distance_action, tips_always_show_action);
    menu->insertAction(set_distance_action, popup_always_show_action);
    menu->insertAction(set_distance_action, follow_plugin_show_action);
    menu->insertAction(set_distance_action, disable_plugin_action);
    menu->insertAction(set_distance_action, only_hide_widget_action);
    menu->insertAction(set_distance_action, update_plugin_action);
    hover_timer->setSingleShot(true);
    hover_timer->setInterval(1000);
    connect(hover_timer, &QTimer::timeout, this, [=]
    {
        if (mouse_in && !mouse_pressed)
        {
            emit Hover();
        }
        if (mouse_pressed)
        {
            mouse_pressed = false;
        }
    });
    hover_timer->start();
}
void Plugin_Item_Widget::set_icon(QString checked_icon_path)
{
    tips_always_show_action->setIcon(QIcon(checked_icon_path));
    popup_always_show_action->setIcon(QIcon(checked_icon_path));
    disable_plugin_action->setIcon(QIcon(checked_icon_path));
    Plugin_Widget::set_icon(checked_icon_path);
}
void Plugin_Item_Widget::set_extra_menu(QString data)
{
    for (auto child : plugin_extra_context_menu->children())
    {
        child->disconnect();
        child->deleteLater();
    }
    if (data.isEmpty())
    {
        use_plugin_context = false;
        return;
    }
    else
    {
        use_plugin_context = true;
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data.toUtf8(), &error);
        if (error.error != QJsonParseError::NoError)
        {
            return;
        }
        if (!doc.isObject())
        {
            return;
        }
        //(O_O)!好像没有详细定义
        QJsonObject menuObj = doc.object();
        bool checkableMenu = menuObj["checkableMenu"].toBool(false);
        bool singleCheck = menuObj["singleCheck"].toBool(false);
        if (singleCheck && checkableMenu)
        {
            QActionGroup *exclusiveGroup = new QActionGroup(plugin_extra_context_menu);
            exclusiveGroup->setExclusive(true);
        }
        if (!menuObj.contains("items") || !menuObj["items"].isArray())
        {
            return;
        }
        QJsonArray itemsArray = menuObj["items"].toArray();
        parseMenuItemsArray(itemsArray, checkableMenu, singleCheck);
    }
}
void Plugin_Item_Widget::parseMenuItemsArray(const QJsonArray &itemsArray, bool checkableMenu, bool singleCheck)
{
    QActionGroup *exclusiveGroup = nullptr;
    if (singleCheck && checkableMenu)
    {
        exclusiveGroup = new QActionGroup(plugin_extra_context_menu);
        exclusiveGroup->setExclusive(true);
    }
    for (const QJsonValue &itemValue : itemsArray)
    {
        if (!itemValue.isObject())
        {
            continue;
        }
        QJsonObject itemObj = itemValue.toObject();
        QString itemId = itemObj["itemId"].toString();
        QString itemText = itemObj["itemText"].toString();
        bool isActive = itemObj["isActive"].toBool(true);
        if (itemText.isEmpty())
        {
            continue;
        }
        QAction *action = plugin_extra_context_menu->addAction(itemText);
        if (!itemId.isEmpty())
        {
            action->setData(itemId);
        }
        action->setEnabled(isActive);
        if (checkableMenu)
        {
            action->setCheckable(true);
            if (itemObj.contains("checked"))
            {
                bool checked = itemObj["checked"].toBool(false);
                action->setChecked(checked);
            }
            if (singleCheck && exclusiveGroup)
            {
                exclusiveGroup->addAction(action);
            }
        }
        if (itemObj.contains("items") && itemObj["items"].isArray())
        {
            QMenu *subMenu = new QMenu(itemText, plugin_extra_context_menu);
            action->setMenu(subMenu);
            QJsonArray subItemsArray = itemObj["items"].toArray();
            parseMenuItemsArray(subMenu, subItemsArray, checkableMenu, singleCheck);
        }
        connect(action, &QAction::triggered, this, [this, action, itemId]
        {
            emit this->extra_menu_call(itemId, action->isChecked());
        });
    }
}
void Plugin_Item_Widget::parseMenuItemsArray(QMenu* parentMenu, const QJsonArray& itemsArray, bool checkableMenu, bool singleCheck)
{
    QActionGroup *exclusiveGroup = nullptr;

    if (singleCheck && checkableMenu)
    {
        exclusiveGroup = new QActionGroup(parentMenu);
        exclusiveGroup->setExclusive(true);
    }
    for (const QJsonValue &itemValue : itemsArray)
    {
        if (!itemValue.isObject())
        {
            continue;
        }
        QJsonObject itemObj = itemValue.toObject();
        QString itemId = itemObj["itemId"].toString();
        QString itemText = itemObj["itemText"].toString();
        bool isActive = itemObj["isActive"].toBool(true);
        if (itemText.isEmpty())
        {
            continue;
        }
        QAction *action = parentMenu->addAction(itemText);
        if (!itemId.isEmpty())
        {
            action->setData(itemId);
        }
        action->setEnabled(isActive);
        if (checkableMenu)
        {
            action->setCheckable(true);
            if (itemObj.contains("checked"))
            {
                bool checked = itemObj["checked"].toBool(false);
                action->setChecked(checked);
            }
            if (singleCheck && exclusiveGroup)
            {
                exclusiveGroup->addAction(action);
            }
        }
        if (itemObj.contains("items") && itemObj["items"].isArray())
        {
            QMenu *subMenu = new QMenu(itemText, parentMenu);
            action->setMenu(subMenu);
            QJsonArray subItemsArray = itemObj["items"].toArray();
            parseMenuItemsArray(subMenu, subItemsArray, checkableMenu, singleCheck);
        }
        connect(action, &QAction::triggered, this, [this, action, itemId]
        {
            emit this->extra_menu_call(itemId, action->isChecked());
        });
    }
}
void Plugin_Item_Widget::P_save(QSettings *settings, QString Token)
{
    Plugin_Widget::P_save(settings, Token);
}
void Plugin_Item_Widget::p_load(QSettings *settings, QString Token)
{
    Plugin_Widget::p_load(settings, Token);
    tips_always_show_action->setIconVisibleInMenu(root->tips_always_show);
    popup_always_show_action->setIconVisibleInMenu(root->popup_always_show);
    disable_plugin_action->setIconVisibleInMenu(root->plugin_disabled);
}
void Plugin_Item_Widget::contextMenuEvent(QContextMenuEvent *event)
{
    if (save_ptr)
    {
        if (carrier->geometry().contains(event->pos() - this->get_self()->pos()) && use_plugin_context)
        {
            plugin_extra_context_menu->exec(mapToGlobal(event->pos()));
            return;
        }
    }
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == set_plugin_path_action)
    {
        QString plugin_filename = QFileDialog::getOpenFileName(nullptr, "获取插件", QDir::homePath(), "插件(*.so);;所有文件(*.*)");
        emit Call_X11_Raise();
        if (!plugin_filename.isEmpty())
        {
            QFileInfo fileinfo(plugin_filename);
            if (fileinfo.isReadable())
            {
                root->plugin_path = plugin_filename;
                root->load_plugin(plugin_filename);
            }
        }
    }
    else if (know_what == tips_always_show_action)
    {
        tips_always_show_action->setIconVisibleInMenu(!tips_always_show_action->isIconVisibleInMenu());
        root->tips_always_show = tips_always_show_action->isIconVisibleInMenu();
        if (root->tips_always_show)
        {
            root->tips_carrier->call_to_show();
        }
        else
        {
            root->tips_carrier->hide();
        }
    }
    else if (know_what == popup_always_show_action)
    {
        popup_always_show_action->setIconVisibleInMenu(!popup_always_show_action->isIconVisibleInMenu());
        root->popup_always_show = popup_always_show_action->isIconVisibleInMenu();
        if (root->popup_always_show)
        {
            root->popup_carrier->call_to_show();
        }
        else
        {
            root->popup_carrier->hide();
        }
    }
    else if (know_what == disable_plugin_action)
    {
        disable_plugin_action->setIconVisibleInMenu(!disable_plugin_action->isIconVisibleInMenu());
        root->plugin_disabled = disable_plugin_action->isIconVisibleInMenu();
        root->disable_plugin_update();
    }
    else if (know_what == only_hide_widget_action)//改制
    {
        emit real_close_event();
    }
    else if (know_what == update_plugin_action)
    {
        root->update_plugin();
    }
    else if (know_what == follow_plugin_show_action)
    {
        if (root->item_widget)
        {
            root->item_carrier->plugin_set_size();
            root->item_carrier->call_to_show();
        }
        if (root->tips_widget)
        {
            root->tips_carrier->plugin_set_size();
            if (root->tips_always_show) root->tips_carrier->call_to_show();
        }
        if (root->popup_widget)
        {
            root->popup_carrier->plugin_set_size();
            if (root->popup_always_show) root->popup_carrier->call_to_show();
        }
    }
    else if (know_what == set_distance_action)
    {
        bool ok = false;
        int new_width = QInputDialog::getInt(nullptr, "获取数值", "宽度距离:", distance_width, 0, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int new_height = QInputDialog::getInt(nullptr, "获取数值", "高度距离:", distance_height, 0, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        distance_width = new_width;
        distance_height = new_height;
        emit Basic_Widget::size_changed(this->get_self()->size());
    }
    else if (know_what == set_delta_action)
    {
        bool ok = false;
        int new_x = QInputDialog::getInt(nullptr, "获取数值", "X偏移:", delta_x, -2147483647, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int new_y = QInputDialog::getInt(nullptr, "获取数值", "Y偏移:", delta_y, -2147483647, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        delta_x = new_x;
        delta_y = new_y;
        emit Basic_Widget::size_changed(this->get_self()->size());
    }
    else
    {
        basic_action_func(know_what);
    }
}
void Plugin_Item_Widget::mousePressEvent(QMouseEvent *event)
{
    press_event_point = event->globalPos();
    mouse_pressed = true;
    Plugin_Widget::mousePressEvent(event);
}
void Plugin_Item_Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (press_event_point == event->globalPos())
        {
            emit Released();
        }
        if (carrier->geometry().contains(event->pos() - this->get_self()->pos()) && press_event_point == event->globalPos())
        {
            root->click_call();
        }
    }
    mouse_pressed = false;
    mouse_in = true;
    if (save_ptr) save_ptr->update();
    hover_timer->start();
    Plugin_Widget::mouseReleaseEvent(event);
}
void Plugin_Item_Widget::enterEvent(QEvent *event)
{
    mouse_in = true;
    if (save_ptr) save_ptr->update();
    hover_timer->start();
    Plugin_Widget::enterEvent(event);
}
void Plugin_Item_Widget::leaveEvent(QEvent *event)
{
    mouse_in = false;
    Plugin_Widget::leaveEvent(event);
    emit Hover_end();
}
