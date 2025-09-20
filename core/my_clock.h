#ifndef MY_CLOCK_H
#define MY_CLOCK_H
#include "basic_widget.h"
#include <QDateTime>
class My_Clock : public Basic_Widget
{
    Q_OBJECT
public:
    explicit My_Clock(QWidget *parent = nullptr);
    ~My_Clock();
    QList<My_Clock *> *my_clock_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
private:
    Out_line_Label *time_label = new Out_line_Label(this->get_self());
    QTimer *clock_timer = new QTimer;
    QMenu *menu = new QMenu(this);
    QAction *set_outline = new QAction(tr("设置描边大小"), this);
    QAction *set_outline_color = new QAction(tr("设置描边颜色"), this);
    QAction *set_color = new QAction(tr("设置字体颜色"), this);
    QAction *set_font = new QAction(tr("设置字体"), this);
private:
    void timer_update();
    void auto_set_font_size();
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // MY_CLOCK_H
