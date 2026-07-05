#ifndef DESKTOP_MAIN_H
#define DESKTOP_MAIN_H
#include <QtWidgets>
#include "setting_widget.h"
#include "experimental_settings.h"
#include "media_widgetaction.h"
#include "core/tools/widget_control.hpp"
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
    void Update_Color_style();
    void set_towards_ptr(bool *m_is_towards_up_and_down);
private:
    int Button_Number;
    int *now_page = nullptr;
    bool *locking_desktop = nullptr;
    bool *is_towards_up_and_down = nullptr;
};
class Desktop_Control_Dock : public QWidget
{
    Q_OBJECT
public:
    explicit Desktop_Control_Dock(QWidget *parent);
    void set_Desktop_Size(int d_width, int d_height);
    void Update_Widget();
    void Update_Color_style();
    void First_Set();
    void Set_Desktop_Number(int *number);
    void Set_Now_page(int *m_now_page);
    void Changed_Signals();
    void Set_Dock_Show_Bool(bool *m_allow_dock_show);
    void set_locking_desktop(bool *m_locking_desktop);
    void set_towards_ptr(bool *m_is_towards_up_and_down);
    QPoint *basic_pos = nullptr;

    void load(QSettings *settings);
    void save(QSettings *settings);
private:
    int desktop_width;
    int desktop_height;
    int *Desktop_NUmber = nullptr;
    int *now_page = nullptr;
    bool *allow_dock_show = nullptr;
    bool *locking_desktop = nullptr;
    bool *is_towards_up_and_down = nullptr;
    QList<Dock_Button *> Dock_Button_List;
    QWidget *background = new QWidget(this);
    QPoint press_point;
    bool on_press = false;
    char m_padding[7];
    QMenu *menu = new QMenu(this);
    QAction *to_center = new Trans_Action(tr("居中"), "居中", this->metaObject()->className(), this);
    QAction *call_update = new Trans_Action(tr("更新"), "更新", this->metaObject()->className(), this);
    QAction *hide_update = new Trans_Action(tr("隐藏"), "隐藏", this->metaObject()->className(), this);
    QMenu *control_towards_menu = new Trans_Menu(tr("窗口朝向"), "窗口朝向", this->metaObject()->className(), this);
    QAction *towards_up_and_down_action = new Trans_Action(tr("上下"), "上下", this->metaObject()->className(), this);
    QAction *towards_left_and_right_action = new Trans_Action(tr("左右"), "左右", this->metaObject()->className(), this);

    QMenu *set_color_menu = new Trans_Menu(tr("设置颜色"), "设置颜色", this->metaObject()->className(), this);
    QAction *set_background_color_action = new Trans_Action(tr("背景颜色"), "背景颜色", this->metaObject()->className(), this);
    QAction *set_button_color_action = new Trans_Action(tr("按钮颜色"), "按钮颜色", this->metaObject()->className(), this);
    QAction *set_button_pressed_color_action = new Trans_Action(tr("按钮颜色pressed"), "按钮颜色pressed", this->metaObject()->className(), this);
    QAction *set_button_hover_color_action = new Trans_Action(tr("按钮颜色hover"), "按钮颜色hover", this->metaObject()->className(), this);

    QMenu *set_size_menu = new Trans_Menu(tr("设置大小"), "设置大小", this->metaObject()->className(), this);
    QAction *set_window_broder_action = new Trans_Action(tr("窗口圆角大小"), "窗口圆角大小", this->metaObject()->className(), this);
    QAction *set_button_broder_action = new Trans_Action(tr("按钮圆角大小"), "按钮圆角大小", this->metaObject()->className(), this);
    QAction *set_button_length_action = new Trans_Action(tr("当前页按钮长度"), "当前页按钮长度", this->metaObject()->className(), this);
    QAction *set_button_space_action = new Trans_Action(tr("按钮间距"), "按钮间距", this->metaObject()->className(), this);
public:
    QColor bg_color{0, 0, 0, 75};
    QColor button_color{255, 255, 255, 150};
    QColor button_pressed_color{255, 255, 255, 200};
    QColor button_hover_color{255, 255, 255, 150};

    int bg_broder_radius = 10;
    int button_broder_radius = 5;
    int button_length = 50;
    int button_space = 20;
private:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
};
class Desktop_Main : public Desktop_Main_MouseSig_Event
{
    Q_OBJECT
public:
    explicit Desktop_Main(QWidget *parent = nullptr);
    ~Desktop_Main();
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
    Process_Widget *process_widget_p = nullptr;
    QString load_path = QDir::homePath() + "/.local/lib/easy_desktop/config.ini";
    bool *stay_on_top = nullptr;
    int *on_top_time = nullptr;
    int *keyscan_timer = nullptr;
    bool *allow_drop = nullptr;
    QString *file_open_way_process = nullptr;
    QString *file_open_path_process = nullptr;
    QString *file_open_info_process = nullptr;
    QString *terminal_process = nullptr;
    QString *compressor_process = nullptr;
    QString *compressor_zip_process = nullptr;
    QString *compressor_7z_process = nullptr;
    QColor *theme_color = nullptr;
    QColor *theme_background_color = nullptr;
    QColor *theme_text_color = nullptr;
    QColor *select_text_color = nullptr;
    QColor *disabled_text_color = nullptr;
    QString *checked_icon_path = nullptr;
    QPoint basic_pos = QPoint(0, 0);
private:
    int desktop_width;
    int desktop_height;
    bool is_towards_up_and_down = false;
    QList<File_Widget *> file_widget_list;
    QList<My_Clock *> my_clock_list;
    QList<My_Label *> my_label_list;
    QList<My_LineEdit *> my_lineedit_list;
    QList<My_Process_Carrier *> my_process_carrier_list;
    QList<My_Program_Container *> my_program_container_list;
    QList<Process_Widget *> process_widget_list;
    QList<File_Tree *> file_tree_list;
    QList<File_Table *> file_table_list;
#ifdef USE_CHART
    QList<CPU_Chart *> cpu_chart_list;
    QList<RAM_Chart *> ram_chart_list;
    QList<NET_Chart *> net_chart_list;
    QList<DISK_Chart *> disk_chart_list;
    QList<PulseAudio_Chart *> pulseaudio_chart_list;
#endif
    QList<Plugin_Root *> plugin_root_list;
    //ACTION
    QMenu *menu=new QMenu(this);
    QMenu *play_menu=new Trans_Menu(tr("播放菜单"), "播放菜单", this->metaObject()->className(), this);
    QAction *background_play_Action = new Trans_Action(tr("播放"), "播放", this->metaObject()->className(), this);
    QAction *background_pause_Action = new Trans_Action(tr("暂停"), "暂停", this->metaObject()->className(), this);
    QMenu *background_Add_Action = new Trans_Menu(tr("新建控件"), "新建控件", this->metaObject()->className(), this);
    QAction *my_clock_action = new Trans_Action(tr("时钟"), "时钟", this->metaObject()->className(), this);
    QAction *text_edit_action = new Trans_Action(tr("文本框"), "文本框", this->metaObject()->className(), this);
    QAction *my_label_action = new Trans_Action(tr("标签"), "标签", this->metaObject()->className(), this);
    QAction *my_process_action = new Trans_Action(tr("进程按钮"), "进程按钮", this->metaObject()->className(), this);
    QAction *my_file_action = new Trans_Action(tr("文件按钮"), "文件按钮", this->metaObject()->className(), this);
    QAction *file_tree_action = new Trans_Action(tr("树状视图文件夹"), "树状视图文件夹", this->metaObject()->className(), this);
    QAction *file_table_action = new Trans_Action(tr("图标视图文件夹"), "图标视图文件夹", this->metaObject()->className(), this);
    QAction *my_process_Carrier_action = new Trans_Action(tr("进程/文件按钮载体"), "进程/文件按钮载体", this->metaObject()->className(), this);
    QAction *my_program_INNER_action = new Trans_Action(tr("内嵌窗口"), "内嵌窗口", this->metaObject()->className(), this);
    QAction *plugin_widget_action = new Trans_Action(tr("插件窗口"), "插件窗口", this->metaObject()->className(), this);
#ifdef USE_CHART
    QMenu *my_chart_menu = new Trans_Menu(tr("可视化图表"), "可视化图表", this->metaObject()->className(), this);
    QAction *cpu_chart_action = new Trans_Action(tr("CPU"), "CPU", this->metaObject()->className(), this);
    QAction *ram_chart_action = new Trans_Action(tr("内存"), "内存", this->metaObject()->className(), this);
    QAction *net_chart_action = new Trans_Action(tr("网络"), "网络", this->metaObject()->className(), this);
    QAction *disk_chart_action = new Trans_Action(tr("磁盘"), "磁盘", this->metaObject()->className(), this);
    QAction *pulseaudio_chart_action = new Trans_Action(tr("声音服务"), "声音服务", this->metaObject()->className(), this);
#endif
    QMenu *desktop_control_action = new Trans_Menu(tr("桌面控制"), "桌面控制", this->metaObject()->className(), this);
    QAction *lock_desktop = new Trans_Action(tr("锚定桌面"), "锚定桌面", this->metaObject()->className(), this);
    QAction *unlock_desktop = new Trans_Action(tr("取消锚定"), "取消锚定", this->metaObject()->className(), this);
    QAction *show_dock_action = new Trans_Action(tr("显示控制窗口"), "显示控制窗口", this->metaObject()->className(), this);
    QAction *hide_dock_action = new Trans_Action(tr("隐藏控制窗口"), "隐藏控制窗口", this->metaObject()->className(), this);
    QAction *desktop_control_move_to_center_action = new Trans_Action(tr("控制窗口归中"), "控制窗口归中", this->metaObject()->className(), this);
    QAction *Add_new_desktop = new Trans_Action(tr("新建桌面"), "新建桌面", this->metaObject()->className(), this);
    QAction *delete_this_desktop = new Trans_Action(tr("移除桌面"), "移除桌面", this->metaObject()->className(), this);
    QMenu *mouse_menu = new Trans_Menu(tr("鼠标控制"), "鼠标控制", this->metaObject()->className(), this);
    QAction *mouse_play_Action = new Trans_Action(tr("应用"), "应用", this->metaObject()->className(), this);
    QAction *mouse_pause_Action = new Trans_Action(tr("禁用"), "禁用", this->metaObject()->className(), this);
    QMenu *Slider_menu=new Trans_Menu(tr("进度条菜单"), "进度条菜单", this->metaObject()->className(), this);
    Media_WidgetAction *slider_action = new Media_WidgetAction(Slider_menu);
    QAction *save_Action = new Trans_Action(tr("储存"), "储存", this->metaObject()->className(), this);
    QAction *load_Action = new Trans_Action(tr("读取"), "读取", this->metaObject()->className(), this);
    QAction *background_setting_Action = new Trans_Action(tr("壁纸设置"), "壁纸设置", this->metaObject()->className(), this);
    QAction *debugging_setting_Action = new Trans_Action(tr("高级设置"), "高级设置", this->metaObject()->className(), this);
    QAction *Quit_Action = new Trans_Action(tr("退出"), "退出", this->metaObject()->className(), this);
    //ACTION
    QList<Basic_Desktop *> desktop_core_dock_list;
    Desktop_Control_Dock *control_Dock = new Desktop_Control_Dock(this);
    int Desktop_NUmber = 1;
    int now_page = 0;//INDEX
    bool allow_dock_show = true;
    int desktop_move_x = 0;
    QTimer *move_Timer = new QTimer(this);
    int run_time = 0;
    int timer_move_x = 0;
    bool locking_desktop = false;
private:
    QList<File_Widget_CreateData> m_pendingItems;
    int m_currentIndex = 0;
    QTimer *m_createTimer = nullptr;
private slots:
    void createNextBatch();
private:
    void Call_Timer_Move();
    void Timer_End();
    void contextMenuEvent(QContextMenuEvent *event);
    void wheelEvent(QWheelEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
private:
    QRubberBand *m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    QPoint origin_pos = QPoint();
    bool setup_rubber = false;
    char my_padding[7];
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QList<Basic_Widget *> select_basic_widget_list = {};
    virtual void select_mousePressEvent(QMouseEvent *event, Basic_Widget *sender);
    virtual void select_mouseReleaseEvent(QMouseEvent *event, Basic_Widget *sender);
    virtual void select_mouseMoveEvent(QMouseEvent *event, Basic_Widget *sender);
};

#endif // DESKTOP_MAIN_H
