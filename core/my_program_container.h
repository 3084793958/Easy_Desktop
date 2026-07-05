#ifndef MY_PROGRAM_CONTAINER_H
#define MY_PROGRAM_CONTAINER_H
#include "basic_widget.h"
#include "X11/Xlib.h"
#include <QX11Info>
#include <X11/Xatom.h>
#undef CursorShape
class My_Program_Container : public Basic_Widget
{
    Q_OBJECT
public:
    explicit My_Program_Container(QWidget *parent);
    ~My_Program_Container();
    void break_out_con();
    QWidget *Container_Widget = new QWidget(this->get_self());
    Window targetWindow = 0;
    QWidget *targetWidget;
    Display *display;
    QList<My_Program_Container *> *my_program_container_list;
    int distance_width = 20;
    int distance_height = 20;
    int delta_x = 0;
    int delta_y = 0;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
private:
    QMenu *menu = new QMenu(this);
    QAction *set_program = new Trans_Action(tr("设置程序"), "设置程序", this->metaObject()->className(), this);
    QAction *break_program = new Trans_Action(tr("脱离"), "脱离", this->metaObject()->className(), this);
    QAction *focus_action = new Trans_Action(tr("聚焦"), "聚焦", this->metaObject()->className(), this);
    QAction *set_distance_action = new Trans_Action(tr("设置间距"), "设置间距", this->metaObject()->className(), this);
    QAction *set_delta_action = new Trans_Action(tr("设置偏移"), "设置偏移", this->metaObject()->className(), this);
    void update_container();
    QString get_window_class(Display *display, Window window);
    pid_t get_window_pid(Display *display, Window window);
private:
    void contextMenuEvent(QContextMenuEvent *event);
    virtual void focusInEvent(QFocusEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
};

#endif // MY_PROGRAM_CONTAINER_H
