#ifndef PROCESS_WIDGET_H
#define PROCESS_WIDGET_H
#include "basic_widget.h"
class Process_Widget : public Basic_Widget
{
    Q_OBJECT
public:
    explicit Process_Widget(QWidget *parent);
    ~Process_Widget();
    void set_WinId(WId m_winId);
    QList<Process_Widget *> *process_widget_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
    virtual void set_icon(QString checked_icon_path);
signals:
    void Pressed();
    void move_To_Desktop_Sig(QPoint pos_);
    void set_to_Carrier_Sig();
public:
    QString process_string;
    QString running_path;
    bool in_carrier = false;
    int carrier_index = 0;
protected:
    QSize movie_size;
    QWidget *Carrier = new QWidget(this->get_self());
    QMovie *movie = new QMovie;
    Out_line_Label *process_name_label = new Out_line_Label(Carrier);
    bool is_file_widget = false;
    bool theme_image = false;
    QLabel *image_label = new QLabel(Carrier);
    QPushButton *just_show_image = new QPushButton(image_label);//注意private变量
    QString theme_name;
private:
    bool left_mouse_on_press = false;
    bool moved = false;
    WId winId;
    QColor press_color = QColor(255,255,255,75);
    QColor hover_color = QColor(255,255,255,75);
    QColor basic_color = QColor(255,255,255,50);
protected:
    QMenu *menu = new QMenu(this);
    QAction *run_action = new QAction(tr("运行"), this);
    QMenu *setting_menu = new QMenu(tr("设置"), this);
    QAction *set_image = new QAction(tr("设置图像"), this);
    QAction *reset_image = new QAction(tr("还原图像"), this);
    QAction *set_name = new QAction(tr("设置名称"), this);
    QAction *set_font = new QAction(tr("设置字体"), this);
    QAction *set_auto_resize = new QAction(tr("自动缩放字体"), this);
    QAction *set_text_color = new QAction(tr("设置文本颜色"), this);
    QMenu *name_out_line_menu = new QMenu(tr("设置描边"), this);
    QAction *set_out_line_width = new QAction(tr("设置描边大小"), this);
    QAction *set_out_line_color = new QAction(tr("设置描边颜色"), this);
    QAction *set_process = new QAction(tr("设置进程信息"), this);
    QAction *set_by_desktop = new QAction(tr("通过.desktop设置"), this);
    QAction *break_out = new QAction(tr("移入/脱离"), this);
    void X11_Rasie();
private:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void enterEvent(QEvent *event);
    virtual void leaveEvent(QEvent *event);
    void contextMenuEvent(QContextMenuEvent *event);
protected:
    void auto_set_font_size();
    QSize get_Image_Size(QString path);
    void context_solution(QAction *know_what);
    virtual void Extra_Pressed_Do();
    virtual void Set_Base_Icon();
};
#endif // PROCESS_WIDGET_H
