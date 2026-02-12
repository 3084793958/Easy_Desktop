#ifndef DESKTOP_MAIN_H
#define DESKTOP_MAIN_H
#include <QtWidgets>
#include "setting_widget.h"
#include "experimental_settings.h"
#include "media_widgetaction.h"
#include "core/widget_control.hpp"
class Basic_Desktop : public QWidget
{
    Q_OBJECT
public:
    explicit Basic_Desktop(QWidget *parent, int m_desktop_width, int m_desktop_height);
    void set_Desktop_Size(int d_width, int d_height);
    void Update_Widget();
private:
    int desktop_width;
    int desktop_height;
};
class Dock_Button : public QPushButton
{
    Q_OBJECT
public:
    explicit Dock_Button(QWidget *parent);
    void set_Number(int number);
    void set_Now_Page(int *m_now_page);
    void set_locking_desktop(bool *m_locking_desktop);
    void Update_Button();
private:
    int Button_Number;
    int *now_page;
    bool *locking_desktop;
};
class Desktop_Control_Dock : public QWidget
{
    Q_OBJECT
public:
    explicit Desktop_Control_Dock(QWidget *parent);
    void set_Desktop_Size(int d_width, int d_height);
    void Update_Widget();
    void First_Set();
    void Set_Desktop_Number(int *number);
    void Set_Now_page(int *m_now_page);
    void Changed_Signals();
    void Set_Dock_Show_Bool(bool *m_allow_dock_show);
    void set_locking_desktop(bool *m_locking_desktop);
    QPoint *basic_pos;
private:
    int desktop_width;
    int desktop_height;
    int *Desktop_NUmber;
    int *now_page;
    bool *allow_dock_show;
    bool *locking_desktop;
    QList<Dock_Button *> Dock_Button_List;
    QWidget *background = new QWidget(this);
    QPoint press_point;
    bool on_press = false;
    char m_padding[7];
    QMenu *menu = new QMenu(this);
    QAction *to_center = new QAction(tr("居中"), this);
    QAction *call_update = new QAction(tr("更新"), this);
    QAction *hide_update = new QAction(tr("隐藏"), this);
private:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
};
class Desktop_Main : public QWidget
{
    Q_OBJECT
public:
    explicit Desktop_Main(QWidget *parent = nullptr);
    void set_Desktop_Size(int d_width, int d_height);
    void Update_Widget();
    void Connection_Update();
    void Update_Basic_Desktop();
    void desktop_Move_Update(int delta_move);
    void save();
    void save(QString path);
    void load();
    void geometry_change();
    void update_for_lineedit(QColor m_theme_color, QColor m_theme_background_color, QColor m_theme_text_color, QColor m_select_text_color, QColor m_disabled_text_color, QString m_checked_icon_path);
    void slider_set_volume(int value);
    void slider_set_position(int value);
    void slider_set_speed(int value);
signals:
    void keyscan_loaded();
public:
    Setting_Widget *setting_widget;
    Experimental_Settings *experimental_settings;
    Desktop_Background *desktop_background;
    WId m_WinId;
    Process_Widget *process_widget_p = nullptr;
    QString load_path = QDir::homePath() + "/.local/lib/easy_desktop/config.ini";
    bool *stay_on_top;
    int *on_top_time;
    int *keyscan_timer;
    bool *allow_drop;
    QString *file_open_way_process;
    QString *file_open_path_process;
    QString *file_open_info_process;
    QColor *theme_color;
    QColor *theme_background_color;
    QColor *theme_text_color;
    QColor *select_text_color;
    QColor *disabled_text_color;
    QString *checked_icon_path;
    QPoint basic_pos = QPoint(0, 0);
private:
    int desktop_width;
    int desktop_height;
    QList<File_Widget *> file_widget_list;
    QList<My_Clock *> my_clock_list;
    QList<My_Label *> my_label_list;
    QList<My_LineEdit *> my_lineedit_list;
    QList<My_Process_Carrier *> my_process_carrier_list;
    QList<My_Program_Container *> my_program_container_list;
    QList<Process_Widget *> process_widget_list;
    QList<File_Tree *> file_tree_list;
    QList<CPU_Chart *> cpu_chart_list;
    QList<RAM_Chart *> ram_chart_list;
    QList<NET_Chart *> net_chart_list;
    QList<DISK_Chart *> disk_chart_list;
    QList<PulseAudio_Chart *> pulseaudio_chart_list;
    QList<Plugin_Root *> plugin_root_list;
    //ACTION
    QMenu *menu=new QMenu(this);
    QMenu *play_menu=new QMenu(tr("播放菜单"),this);
    QAction *background_play_Action = new QAction(tr("播放"),this);
    QAction *background_pause_Action = new QAction(tr("暂停"),this);
    QMenu *background_Add_Action = new QMenu(tr("新建控件"),this);
    QAction *my_clock_action = new QAction(tr("时钟"), this);
    QAction *text_edit_action = new QAction(tr("文本框"), this);
    QAction *my_label_action = new QAction(tr("标签"), this);
    QAction *my_process_action = new QAction(tr("进程按钮"), this);
    QAction *my_file_action = new QAction(tr("文件按钮"), this);
    QAction *file_tree_action = new QAction(tr("树状文件夹"), this);
    QAction *my_process_Carrier_action = new QAction(tr("进程/文件按钮载体"), this);
    QAction *my_program_INNER_action = new QAction(tr("内嵌窗口"), this);
    QAction *plugin_widget_action = new QAction(tr("插件窗口"), this);
    QMenu *my_chart_menu = new QMenu(tr("可视化图表"), this);
    QAction *cpu_chart_action = new QAction(tr("CPU"), this);
    QAction *ram_chart_action = new QAction(tr("内存"), this);
    QAction *net_chart_action = new QAction(tr("网络"), this);
    QAction *disk_chart_action = new QAction(tr("磁盘"), this);
    QAction *pulseaudio_chart_action = new QAction(tr("声音服务"), this);
    QMenu *desktop_control_action = new QMenu(tr("桌面控制"),this);
    QAction *lock_desktop = new QAction(tr("锚定桌面"),this);
    QAction *unlock_desktop = new QAction(tr("取消锚定"),this);
    QAction *show_dock_action = new QAction(tr("显示控制窗口"),this);
    QAction *hide_dock_action = new QAction(tr("隐藏控制窗口"),this);
    QAction *Add_new_desktop = new QAction(tr("新建桌面"),this);
    QAction *delete_this_desktop = new QAction(tr("移除桌面"),this);
    QMenu *mouse_menu = new QMenu(tr("鼠标控制"),this);
    QAction *mouse_play_Action = new QAction(tr("应用"),this);
    QAction *mouse_pause_Action = new QAction(tr("禁用"),this);
    QMenu *Slider_menu=new QMenu(tr("进度条菜单"),this);
    Media_WidgetAction *slider_action = new Media_WidgetAction(Slider_menu);
    QAction *save_Action = new QAction(tr("储存"),this);
    QAction *load_Action = new QAction(tr("读取"),this);
    QAction *background_setting_Action = new QAction(tr("壁纸设置"),this);
    QAction *debugging_setting_Action = new QAction(tr("实验设置"), this);
    QAction *Quit_Action = new QAction(tr("退出"),this);
    //ACTION
    QList<Basic_Desktop *> desktop_core_dock_list;
    Desktop_Control_Dock *control_Dock = new Desktop_Control_Dock(this);
    int Desktop_NUmber = 1;
    int now_page = 0;//INDEX
    bool allow_dock_show = true;
    int desktop_move_x = 0;
    QTimer *move_Timer = new QTimer;
    int run_time = 0;
    int timer_move_x = 0;
    bool locking_desktop = false;
private:
    void Call_Timer_Move();
    void Timer_End();
    void contextMenuEvent(QContextMenuEvent *event);
    void wheelEvent(QWheelEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
};

#endif // DESKTOP_MAIN_H
