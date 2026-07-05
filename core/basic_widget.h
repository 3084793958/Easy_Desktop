#ifndef BASIC_WIDGET_H
#define BASIC_WIDGET_H
#include <QtWidgets>
#include "tools/my_x11_libs.h"
#include "core/tools/trans_action.h"
enum class Towards
{
    No,
    Top,
    Bottom,
    Left,
    Right,
    Top_Left,
    Top_Right,
    Bottom_Left,
    Bottom_Right
};
class Basic_Widget;
class Desktop_Main_MouseSig_Event : public QWidget
{
    Q_OBJECT
public:
    explicit Desktop_Main_MouseSig_Event(QWidget *parent) : QWidget(parent) {}
    virtual ~Desktop_Main_MouseSig_Event() {}
    virtual void select_mousePressEvent(QMouseEvent *event, Basic_Widget *sender)
    {
        (void) event;
        (void) sender;
    }
    virtual void select_mouseReleaseEvent(QMouseEvent *event, Basic_Widget *sender)
    {
        (void) event;
        (void) sender;
    }
    virtual void select_mouseMoveEvent(QMouseEvent *event, Basic_Widget *sender)
    {
        (void) event;
        (void) sender;
    }
};
class Out_line_Label : public QLabel
{
    Q_OBJECT
public:
    explicit Out_line_Label(QWidget *parent = nullptr);
    QColor outlineColor;
    int outlineWidth;
    QColor text_color;
private:
    void paintEvent(QPaintEvent *event);
};
class Basic_Widget : public Desktop_Main_MouseSig_Event
{
    Q_OBJECT
public:
    explicit Basic_Widget(QWidget *parent);
    ~Basic_Widget();
    QWidget* get_self();
    void resize(int w, int h);
    void resize(QSize size);
    void setParent(QWidget *parent);
    void setGeometry(QRect rect);
    void set_now_page(int *m_now_page);
    void set_desktop_number(int *m_desktop_number);
    void set_basic_list(QList<QWidget *> *m_basic_list);
    void Update_Background();
    bool set_select(bool select);
    void sig_mousePressEvent(QMouseEvent *event);
    void sig_mouseReleaseEvent(QMouseEvent *event);
    void sig_mouseMoveEvent(QMouseEvent *event);
signals:
    void sig_select_mousePressEvent(QMouseEvent *event, Basic_Widget *sender);
    void sig_select_mouseReleaseEvent(QMouseEvent *event, Basic_Widget *sender);
    void sig_select_mouseMoveEvent(QMouseEvent *event, Basic_Widget *sender);
public:
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
    virtual void save(QSettings *settings, QString Token);//由于很多派生类已经写死了virtual void save(QSettings *settings);,不可能加个QString Token.
    virtual void load(QSettings *settings, QString Token);
    virtual void set_icon(QString checked_icon_path);
    enum class Button_Pos
    {
        Top_Left,
        Top_Right,
        Bottom_Left,
        Bottom_Right
    };
signals:
    void close_signals();
    void size_changed(QSize size);
public:
    QMenu *basic_control = new Trans_Menu(tr("基础操作"), "基础操作", this->metaObject()->className(), this);
    QAction *move_to_page_action = new Trans_Action(tr("移动到页"), "移动到页", this->metaObject()->className(), this);
    QAction *set_background_radius = new Trans_Action(tr("设置圆角大小"), "设置圆角大小", this->metaObject()->className(), this);
    QAction *set_background_color = new Trans_Action(tr("设置背景颜色"), "设置背景颜色", this->metaObject()->className(), this);
    QAction *show_close_button = new Trans_Action(tr("显示关闭窗口按钮"), "显示关闭窗口按钮", this->metaObject()->className(), this);
    QMenu *close_button_pos_menu = new Trans_Menu(tr("关闭窗口按钮位置"), "关闭窗口按钮位置", this->metaObject()->className(), this);
    QAction *close_button_pos_top_left = new Trans_Action(tr("左上"), "左上", this->metaObject()->className(), this);
    QAction *close_button_pos_top_right = new Trans_Action(tr("右上"), "右上", this->metaObject()->className(), this);
    QAction *close_button_pos_bottom_left = new Trans_Action(tr("左下"), "左下", this->metaObject()->className(), this);
    QAction *close_button_pos_bottom_right = new Trans_Action(tr("右下"), "右下", this->metaObject()->className(), this);
    Button_Pos close_button_pos = Button_Pos::Top_Right;
    QAction *show_select_button = new Trans_Action(tr("允许选择"), "允许选择", this->metaObject()->className(), this);
    QMenu *select_button_pos_menu = new Trans_Menu(tr("选择按钮位置"), "选择按钮位置", this->metaObject()->className(), this);
    QAction *select_button_pos_top_left = new Trans_Action(tr("左上"), "左上", this->metaObject()->className(), this);
    QAction *select_button_pos_top_right = new Trans_Action(tr("右上"), "右上", this->metaObject()->className(), this);
    QAction *select_button_pos_bottom_left = new Trans_Action(tr("左下"), "左下", this->metaObject()->className(), this);
    QAction *select_button_pos_bottom_right = new Trans_Action(tr("右下"), "右下", this->metaObject()->className(), this);
    Button_Pos select_button_pos = Button_Pos::Top_Left;
    void update_close_button_pos();
    QAction *set_pos_action = new Trans_Action(tr("设置位置"), "设置位置", this->metaObject()->className(), this);
    QAction *set_size_action = new Trans_Action(tr("设置大小"), "设置大小", this->metaObject()->className(), this);
    QAction *close_action = new Trans_Action(tr("关闭窗口"), "关闭窗口", this->metaObject()->className(), this);
    QPushButton *close_button = new Trans_PushButton(tr("×"), "×", this->metaObject()->className(), this);
    QPushButton *select_button = new QPushButton(this);
protected:
    void moveToDesktop(int index);
    void basic_context(QMenu *menu);
    void basic_action_func(QAction *action);
    int *now_page;
    int *desktop_number;
    QList<QWidget *> *basic_list;
    bool auto_close = true;
    bool select_tags = false;
protected:
    int background_radius = 10;
    QColor background_color = QColor(0,0,0,50);
private:
    QWidget *background = new QWidget(this);
    QPoint press_point;
    bool on_press = false;
    Towards press_towards = Towards::No;
    QPoint resize_point;
    bool press_resize = false;
protected:
    Desktop_Main_MouseSig_Event *save_sig_ptr = nullptr;
private:
    Towards get_towards(QPoint point, QRect rect);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
};

#endif // BASIC_WIDGET_H
