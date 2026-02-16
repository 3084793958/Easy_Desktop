#ifndef PLUGIN_WIDGET_H
#define PLUGIN_WIDGET_H
#include <QPluginLoader>
#include <QCoreApplication>
#include <interfaces/pluginsiteminterface.h>
#include "basic_widget.h"
class Plugin_Root;
class PluginController : public QObject, public PluginProxyInterface
{
    Q_OBJECT
public:
    explicit PluginController(QObject *parent = nullptr, Plugin_Root *plugin_root = nullptr);
    ~PluginController() override;
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
class Plugin_Widget : public Basic_Widget
{
    Q_OBJECT
public:
    explicit Plugin_Widget(QWidget *parent);
    virtual void P_save(QSettings *settings, QString Token);
    virtual void p_load(QSettings *settings, QString Token);
    bool is_item = false;
    char m_padding[6];//配位
    QWidget *carrier = new QWidget(this->get_self());
    QWidget *save_ptr = nullptr;
    int distance_width = 10;
    int distance_height = 10;
    int delta_x = 0;
    int delta_y = 0;
    void set_widget(QWidget *ptr);
    void remove_widget();
    void plugin_set_size();
    void call_to_show();
protected:
    QMenu *menu = new QMenu(this);
    QAction *set_distance_action = new QAction(tr("设置距离"), this);
    QAction *set_delta_action = new QAction(tr("设置偏移"), this);
    virtual void contextMenuEvent(QContextMenuEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
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
signals:
    void Released();
    void Hover();
    void Hover_end();
    void Call_X11_Raise();
    void extra_menu_call(QString menuId, bool checked);
    void real_close_event();
private:
    Plugin_Root *root = nullptr;
    QAction *set_plugin_path_action = new QAction(tr("设置插件路径"), this);
    QAction *tips_always_show_action = new QAction(tr("tips窗口永久显示"), this);
    QAction *popup_always_show_action = new QAction(tr("popup窗口永久显示"), this);
    QAction *disable_plugin_action = new QAction(tr("禁用插件"), this);
    QAction *follow_plugin_show_action = new QAction(tr("插件指导窗口大小"), this);
    QAction *update_plugin_action = new QAction(tr("强制更新插件"), this);
    QAction *only_hide_widget_action = new QAction(tr("移除插件"), this);
    QMenu *set_plugin_position = new QMenu(tr("设置插件位置"), this);
    QAction *set_top_position = new QAction(tr("Top"), this);
    QAction *set_right_position = new QAction(tr("Right"), this);
    QAction *set_bottom_position = new QAction(tr("Bottom"), this);
    QAction *set_left_position = new QAction(tr("Left"), this);
    QMenu *plugin_extra_context_menu = new QMenu(this);
    QTimer *hover_timer = new QTimer;
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
class Plugin_Root : public QObject
{
    Q_OBJECT
public:
    explicit Plugin_Root(QWidget *parent);
    ~Plugin_Root();
    void only_hide();
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
    void update_plugin_position();
    WId WinId;
    void X11_Raise();
    QList<Plugin_Root *> *plugin_root_list;
public:
    QString plugin_path = "";
    bool tips_always_show = false;
    bool popup_always_show = false;
    bool follow_plugin_show_after_load = true;
    bool plugin_disabled = false;
    QString style_sheet = "";
    QString plugin_itemKey= "";
    bool has_been_closed = false;
    Dock::Position plugin_position = Dock::Position::Top;
private:
    QWidget *desktop_parent = nullptr;
    int *now_page;
    int *desktop_number;
    QList<QWidget *> *basic_list;
    QPoint root_pos;
    QPluginLoader *plugin_loader = new QPluginLoader(this);
    PluginController *plugin_controller = new PluginController(this, this);
public:
    Plugin_Item_Widget *item_carrier = new Plugin_Item_Widget(nullptr, this);
    Plugin_Widget *tips_carrier = new Plugin_Widget(nullptr);
    Plugin_Widget *popup_carrier = new Plugin_Widget(nullptr);
    QWidget *item_widget = nullptr;
    QWidget *tips_widget = nullptr;
    QWidget *popup_widget = nullptr;
    void update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, QString checked_icon_path);
};
#endif // PLUGIN_WIDGET_H
