#ifndef PROCESS_WIDGET_H
#define PROCESS_WIDGET_H
#include "basic_widget.h"
#include "core/module/out_line_label.h"
class Process_Widget : public Basic_Widget
{
    Q_OBJECT
public:
    explicit Process_Widget(QWidget *parent);
    ~Process_Widget();
    QList<Process_Widget *> *process_widget_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
    virtual void set_icon(QString checked_icon_path);
    virtual void sig_setBackgroundColor(QList<QColor> colors);
signals:
    void Pressed();
    void move_To_Desktop_Sig(QPoint pos_);
    void set_to_Carrier_Sig();
public:
    QString process_string;
    QString running_path;
    bool in_carrier = false;
    int carrier_index = 0;
    void setParent(QWidget *parent);
    void updateSig();
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
    QColor press_color = QColor(255,255,255,75);
    QColor hover_color = QColor(255,255,255,75);
    QColor basic_color = QColor(255,255,255,50);
protected:
    QMenu *menu = new QMenu(this);
    QAction *run_action = new Trans_Action(tr("运行"), "运行", this->metaObject()->className(), this);
    QMenu *setting_menu = new Trans_Menu(tr("设置"), "设置", this->metaObject()->className(), this);
    QAction *single_press_mode_action = new Trans_Action(tr("单击模式"), "单击模式", this->metaObject()->className(), this);
    QAction *set_image = new Trans_Action(tr("设置图像"), "设置图像", this->metaObject()->className(), this);
    QAction *reset_image = new Trans_Action(tr("还原图像"), "还原图像", this->metaObject()->className(), this);
    QAction *set_name = new Trans_Action(tr("设置名称"), "设置名称", this->metaObject()->className(), this);
    QAction *set_font = new Trans_Action(tr("设置字体"), "设置字体", this->metaObject()->className(), this);
    QAction *set_auto_resize = new Trans_Action(tr("自动缩放字体"), "自动缩放字体", this->metaObject()->className(), this);
    QAction *set_text_color = new Trans_Action(tr("设置文本颜色"), "设置文本颜色", this->metaObject()->className(), this);
    QMenu *name_out_line_menu = new Trans_Menu(tr("设置描边"), "设置描边", this->metaObject()->className(), this);
    QAction *set_out_line_width = new Trans_Action(tr("设置描边大小"), "设置描边大小", this->metaObject()->className(), this);
    QAction *set_out_line_color = new Trans_Action(tr("设置描边颜色"), "设置描边颜色", this->metaObject()->className(), this);
    QAction *set_process = new Trans_Action(tr("设置进程信息"), "设置进程信息", this->metaObject()->className(), this);
    QAction *set_by_desktop = new Trans_Action(tr("通过.desktop设置"), "通过.desktop设置", this->metaObject()->className(), this);
    QAction *break_out = new Trans_Action(tr("移入/脱离"), "移入/脱离", this->metaObject()->className(), this);
    QTimer *double_click_timer = new QTimer(this);
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
