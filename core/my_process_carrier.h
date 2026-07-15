#ifndef MY_PROCESS_CARRIER_H
#define MY_PROCESS_CARRIER_H
#include "basic_widget.h"
#include "file_widget.h"
struct File_Widget_CreateData
{
    QString filePath;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
    bool showCloseBtn = true;
    File_Widget_CreateData(QString filePath, int x, int y, int w, int h, bool showCloseBtn);
};
enum class Control_Dock_Pos
{
    Top = 0,
    Bottom = 1,
    Left = 2,
    Right = 3
};
class Page_Dock_Button : public QPushButton
{
    Q_OBJECT
public:
    explicit Page_Dock_Button(QWidget *parent);
    void set_Number(int number);
    void set_Now_Page(int *m_now_page);
    void Update_Button();
    void Set_Dock_Pos(Control_Dock_Pos *m_control_dock_pos);
private:
    int Button_Number;
    int *now_page;
    Control_Dock_Pos *control_dock_pos = nullptr;
};
class Page_Control_Dock : public QWidget
{
    Q_OBJECT
public:
    explicit Page_Control_Dock(QWidget *parent);
    void Update_Widget();
    void Set_Desktop_Number(int *number);
    void Set_Now_page(int *m_now_page);
    void Changed_Signals();
    void Set_Dock_Pos(Control_Dock_Pos *m_control_dock_pos);
private:
    int *Desktop_NUmber;
    int *now_page;
    Control_Dock_Pos *control_dock_pos = nullptr;
    QList<Page_Dock_Button *> Dock_Button_List;
};
class My_Process_Carrier :public Basic_Widget
{
    Q_OBJECT
private:
    Control_Dock_Pos control_dock_pos = Control_Dock_Pos::Bottom;
public:
    explicit My_Process_Carrier(QWidget *parent);
    ~My_Process_Carrier();
    void Update_Basic_Desktop();
    void desktop_Move_Update(int delta_move);
    QList<My_Process_Carrier *> *my_process_carrier_list;
    QList<File_Widget *> *file_widget_list;
    QList<Process_Widget *> *process_widget_list;
    Process_Widget **process_widget_p;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
    int save_index_number = 0;
    bool *m_allow_drop;
    QString *file_open_way_process;
    QString *file_open_path_process;
    QString *file_open_info_process;
    QString *terminal_process = nullptr;
private:
    QWidget *Basic_Carrier = new QWidget(this);
public:
    QList<QWidget *> carrier_widget_list;
    int carrier_now_page = 0;
    int carrier_page_number = 1;
private:
    int desktop_move_x = 0;
    QTimer *move_Timer = new QTimer(this);
    int run_time = 0;
    int timer_move_x = 0;
    Page_Control_Dock *control_Dock = new Page_Control_Dock(this->Basic_Carrier);
protected:
    QMenu *menu = new QMenu(this);
    QAction *create_process_widget_action = new Trans_Action(tr("新建进程按钮"), "新建进程按钮", this->metaObject()->className(), this);
    QAction *create_file_widget_action = new Trans_Action(tr("新建文件按钮"), "新建文件按钮", this->metaObject()->className(), this);
    QAction *get_process_widget_action = new Trans_Action(tr("载入按钮"), "载入按钮", this->metaObject()->className(), this);
    QMenu *load_file_action = new Trans_Menu(tr("载入文件(夹)"), "载入文件(夹)", this->metaObject()->className(), this);
    QAction *load_files = new Trans_Action(tr("载入文件"), "载入文件", this->metaObject()->className(), this);
    QAction *load_dir = new Trans_Action(tr("载入文件夹"), "载入文件夹", this->metaObject()->className(), this);
    QAction *create_carrier_action = new Trans_Action(tr("新建页"), "新建页", this->metaObject()->className(), this);
    QAction *delete_carrier_action = new Trans_Action(tr("删除页"), "删除页", this->metaObject()->className(), this);
    QMenu *set_control_dock_pos_menu = new Trans_Menu(tr("设置控制窗口方位"), "设置控制窗口方位", this->metaObject()->className(), this);
    QAction *set_to_top_action = new Trans_Action(tr("顶部"), "顶部", this->metaObject()->className(), this);
    QAction *set_to_bottom_action = new Trans_Action(tr("底部"), "底部", this->metaObject()->className(), this);
    QAction *set_to_left_action = new Trans_Action(tr("左侧"), "左侧", this->metaObject()->className(), this);
    QAction *set_to_right_action = new Trans_Action(tr("右侧"), "右侧", this->metaObject()->className(), this);
    void context_solution(QAction *know_what, QPoint pos);
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
private:
    virtual void select_mousePressEvent(QMouseEvent *event, Basic_Widget *sender);
    virtual void select_mouseReleaseEvent(QMouseEvent *event, Basic_Widget *sender);
    virtual void select_mouseMoveEvent(QMouseEvent *event, Basic_Widget *sender);
    virtual void select_closeEvent(Basic_Widget *sender, bool &send);
    virtual void select_moveToPage(int &page, Basic_Widget *sender);
    virtual void select_setRadius(int &radius, Basic_Widget *sender);
    virtual void select_setBackgroundColor(QList<QColor> colors, Basic_Widget *sender);
    virtual void select_setShowCloseButton(bool &show, Basic_Widget *sender);
    virtual void select_setCloseButtonPos(Button_Pos &pos, Basic_Widget *sender);
    virtual void select_setAllowSelectButton(Basic_Widget *sender);
    virtual void select_setSelectButtonPos(Button_Pos &pos, Basic_Widget *sender);
    virtual void select_setPos(QPoint &pos, Basic_Widget *sender);
    virtual void select_setSize(QRect &old_geometry, QRect &new_geometry, Basic_Widget *sender);
};

#endif // MY_PROCESS_CARRIER_H
