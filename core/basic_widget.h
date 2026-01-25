#ifndef BASIC_WIDGET_H
#define BASIC_WIDGET_H
#include <QtWidgets>
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
class Basic_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Basic_Widget(QWidget *parent);
    QWidget* get_self();
    void resize(int w, int h);
    void resize(QSize size);
    void setGeometry(QRect rect);
    void set_now_page(int *m_now_page);
    void set_desktop_number(int *m_desktop_number);
    void set_basic_list(QList<QWidget *> *m_basic_list);
    void Update_Background();
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
signals:
    void close_signals();
    void size_changed(QSize size);
public:
    QMenu *basic_control = new QMenu(tr("基础操作"), this);
    QAction *move_to_page_action = new QAction(tr("移动到页"), this);
    QAction *set_background_radius = new QAction(tr("设置圆角大小"), this);
    QAction *set_background_color = new QAction(tr("设置背景颜色"), this);
    QAction *show_close_button = new QAction(tr("显示关闭窗口键"), this);
    QAction *set_pos_action = new QAction(tr("设置位置"), this);
    QAction *set_size_action = new QAction(tr("设置大小"), this);
    QAction *close_action = new QAction(tr("关闭窗口"), this);
    QPushButton *close_button = new QPushButton(tr("×"),this);
protected:
    void moveToDesktop(int index);
    void basic_context(QMenu *menu);
    void basic_action_func(QAction *action);
    int *now_page;
    int *desktop_number;
    QList<QWidget *> *basic_list;
    bool auto_close = true;
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
private:
    Towards get_towards(QPoint point, QRect rect);
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
};

#endif // BASIC_WIDGET_H
