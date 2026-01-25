#ifndef MY_PROCESS_CARRIER_H
#define MY_PROCESS_CARRIER_H
#include "basic_widget.h"
#include "file_widget.h"
class Page_Dock_Button : public QPushButton
{
    Q_OBJECT
public:
    explicit Page_Dock_Button(QWidget *parent);
    void set_Number(int number);
    void set_Now_Page(int *m_now_page);
    void Update_Button();
private:
    int Button_Number;
    int *now_page;
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
private:
    int *Desktop_NUmber;
    int *now_page;
    QList<Page_Dock_Button *> Dock_Button_List;
};
class My_Process_Carrier :public Basic_Widget
{
    Q_OBJECT
public:
    explicit My_Process_Carrier(QWidget *parent);
    ~My_Process_Carrier();
    void Update_Basic_Desktop();
    void desktop_Move_Update(int delta_move);
    QList<My_Process_Carrier *> *my_process_carrier_list;
    QList<File_Widget *> *file_widget_list;
    QList<Process_Widget *> *process_widget_list;
    WId m_WinId;
    Process_Widget **process_widget_p;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
    int save_index_number = 0;
    bool *m_allow_drop;
    QString *file_open_way_process;
    QString *file_open_path_process;
    QString *file_open_info_process;
private:
    QWidget *Basic_Carrier = new QWidget(this);
public:
    QList<QWidget *> carrier_widget_list;
    int carrier_now_page = 0;
    int carrier_page_number = 1;
private:
    int desktop_move_x = 0;
    QTimer *move_Timer = new QTimer;
    int run_time = 0;
    int timer_move_x = 0;
    Page_Control_Dock *control_Dock = new Page_Control_Dock(this->Basic_Carrier);
protected:
    QMenu *menu = new QMenu(this);
    QAction *create_process_widget_action = new QAction(tr("新建进程按钮"), this);
    QAction *create_file_widget_action = new QAction(tr("新建文件按钮"), this);
    QAction *get_process_widget_action = new QAction(tr("载入按钮"), this);
    QMenu *load_file_action = new QMenu(tr("载入文件(夹)"), this);
    QAction *load_files = new QAction(tr("载入文件"), this);
    QAction *load_dir = new QAction(tr("载入文件夹"), this);
    QAction *create_carrier_action = new QAction(tr("新建页"), this);
    QAction *delete_carrier_action = new QAction(tr("删除页"), this);
    void context_solution(QAction *know_what, QPoint pos);
    void X11_Raise();
private:
    void Call_Timer_Move();
    void Timer_End();
    void contextMenuEvent(QContextMenuEvent *event);
    void wheelEvent(QWheelEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
};

#endif // MY_PROCESS_CARRIER_H
