#ifndef PLUGIN_WIDGET_H
#define PLUGIN_WIDGET_H
#include <QPluginLoader>
#include <QCoreApplication>
#include "basic_widget.h"

#include "interfaces/easy_desktop_class.h"

class PluginsItemInterface;
class PluginsItemInterface_V_2_0_0;
class PluginController;
class PluginController_V_2_0_0;

class Plugin_Root : public QObject
{
    Q_OBJECT
public:
    explicit Plugin_Root(QWidget *parent);
    ~Plugin_Root();
    void close_plugin(bool force_remove = false);
    void set_now_page(int *m_now_page);
    void set_desktop_number(int *m_desktop_number);
    void set_basic_list(QList<QWidget *> *m_basic_list);
    void set_root_pos(QPoint pos);
    void call_update_plugin_carrier();
    void load_plugin(QString filepath);
    void unload_plugin();
    void save(QSettings *settings);
    void load(QSettings *settings);
    void set_icon(QString checked_icon_path);
    void disable_plugin_update();
    void click_call();
    void update_plugin();
    void update_plugin(PluginsItemInterface * const itemInter, const QString &itemKey);
    PluginsItemInterface *get_interface();
    bool isV_2_0_0_Plugin();
#ifdef USE_DTK
    void update_plugin(PluginsItemInterface_V_2_0_0 * const itemInter, const QString &itemKey);
    PluginsItemInterface_V_2_0_0 *get_interface_V_2_0_0();
#endif
    bool is_Ext_plugin();
    QList<Plugin_Root *> *plugin_root_list;
    static bool Contains_Ext_Plugin(QString Ext_name, QString plugin_controller_name);
public:
    QString plugin_path = "";
    bool tips_always_show = false;
    bool popup_always_show = false;
    bool plugin_disabled = false;
    bool will_fully_remove = false;
    QString style_sheet = "";
    QString plugin_itemKey= "";
    bool has_been_closed = false;
    int plugin_position = 0;
    P_Sender *update_sender = new P_Sender(this, false, false);
    P_Sender *send_data_sender = new P_Sender(this, false, false);
private:
    QWidget *desktop_parent = nullptr;
    int *now_page = nullptr;
    int *desktop_number = nullptr;
    QList<QWidget *> *basic_list = nullptr;
    QPoint root_pos;
    QPluginLoader *plugin_loader = new QPluginLoader(this);
    PluginController *plugin_controller = nullptr;
#ifdef USE_DTK
    PluginController_V_2_0_0 *plugin_controller_V_2_0_0 = nullptr;
#endif
public:
    class Plugin_Item_Widget *item_carrier = nullptr;
    class Plugin_Widget *tips_carrier = nullptr;
    class Plugin_Widget *popup_carrier = nullptr;
    QWidget *item_widget = nullptr;
    QWidget *tips_widget = nullptr;
    QWidget *popup_widget = nullptr;
    void update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, QString checked_icon_path);
};

class Plugin_Widget : public Basic_Widget
{
    Q_OBJECT
public:
    explicit Plugin_Widget(QWidget *parent, Plugin_Root *plugin_root, int m_carrier_type);
    virtual void P_save(QSettings *settings, QString Token);
    virtual void p_load(QSettings *settings, QString Token);
    int plugin_carrier_type = 0;
    QWidget *carrier = new QWidget(this->get_self());
    QWidget *save_ptr = nullptr;
    int distance_width = 10;
    int distance_height = 10;
    int delta_x = 0;
    int delta_y = 0;
    void set_widget(QWidget *ptr);
    void remove_widget();
    void plugin_set_size(PluginsItemInterface *const itemInter);
    void plugin_carrier_update(PluginsItemInterface * const itemInter);
    void plugin_carrier_sending_data(PluginsItemInterface * const itemInter);
#ifdef USE_DTK
    void plugin_set_size(PluginsItemInterface_V_2_0_0 *const itemInter);
    void plugin_carrier_update(PluginsItemInterface_V_2_0_0 * const itemInter);
    void plugin_carrier_sending_data(PluginsItemInterface_V_2_0_0 * const itemInter);
#endif
    void plugin_set_size();
    void call_to_show();
    virtual void context_menu_event(QAction *know_what);
    P_Sender *carrier_action_sender = new P_Sender(this);
    P_Sender *plugin_action_sender = nullptr;
    QAction *plugin_menu_ptr = nullptr;
public:
    QMenu *menu = new Trans_Menu(tr("载体菜单"), "载体菜单", this->metaObject()->className(), this);
protected:
    QAction *set_distance_action = new Trans_Action(tr("设置间距"), "设置间距", this->metaObject()->className(), this);
    QAction *set_delta_action = new Trans_Action(tr("设置偏移"), "设置偏移", this->metaObject()->className(), this);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    //music-island补丁
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    //music-island补丁
    Plugin_Root *root = nullptr;
};
class Plugin_Item_Widget : public Plugin_Widget
{
    Q_OBJECT
public:
    explicit Plugin_Item_Widget(QWidget *parent, Plugin_Root *plugin_root);
    virtual void P_save(QSettings *settings, QString Token);
    virtual void p_load(QSettings *settings, QString Token);
    virtual void set_icon(QString checked_icon_path);
    void set_extra_menu(QString data);
    void parseMenuItemsArray(QMenu *parentMenu, const QJsonArray &itemsArray, bool checkableMenu, bool singleCheck);
    void parseMenuItemsArray(const QJsonArray &itemsArray, bool checkableMenu, bool singleCheck);
    void plugin_position_gui_update();
    virtual void context_menu_event(QAction *know_what);
signals:
    void Released();
    void Hover();
    void Hover_end();
    void Call_X11_Raise();
    void extra_menu_call(QString menuId, bool checked);
    void real_close_event();
private:
    QAction *set_plugin_path_action = new Trans_Action(tr("设置插件路径"), "设置插件路径", this->metaObject()->className(), this);
    QAction *tips_always_show_action = new Trans_Action(tr("tips窗口永久显示"), "tips窗口永久显示", this->metaObject()->className(), this);
    QAction *popup_always_show_action = new Trans_Action(tr("popup窗口永久显示"), "popup窗口永久显示", this->metaObject()->className(), this);
    QAction *disable_plugin_action = new Trans_Action(tr("禁用插件"), "禁用插件", this->metaObject()->className(), this);
    QAction *follow_plugin_show_action = new Trans_Action(tr("插件指导窗口大小"), "插件指导窗口大小", this->metaObject()->className(), this);
    QAction *update_plugin_action = new Trans_Action(tr("强制更新插件"), "强制更新插件", this->metaObject()->className(), this);
    QAction *only_hide_widget_action = new Trans_Action(tr("移除插件"), "移除插件", this->metaObject()->className(), this);
    QMenu *set_plugin_position = new Trans_Menu(tr("设置插件位置"), "设置插件位置", this->metaObject()->className(), this);
    QAction *set_top_position = new Trans_Action(tr("Top"), "Top", this->metaObject()->className(), this);
    QAction *set_right_position = new Trans_Action(tr("Right"), "Right", this->metaObject()->className(), this);
    QAction *set_bottom_position = new Trans_Action(tr("Bottom"), "Bottom", this->metaObject()->className(), this);
    QAction *set_left_position = new Trans_Action(tr("Left"), "Left", this->metaObject()->className(), this);
    QMenu *plugin_extra_context_menu = new QMenu(this);
    QTimer *hover_timer = new QTimer(this);
    QPoint press_event_point = QPoint(0, 0);
    bool mouse_in = false;
    bool use_plugin_context = false;
    bool mouse_pressed = true;
    char m_padding2[5];//配位
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
};
#endif // PLUGIN_WIDGET_H
