#ifndef MY_LABEL_H
#define MY_LABEL_H
#include "basic_widget.h"
class My_Label : public Basic_Widget
{
    Q_OBJECT
public:
    explicit My_Label(QWidget *parent = nullptr);
    ~My_Label();
    void set_WinId(WId m_winId);
    QList<My_Label *> *my_label_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
private:
    Out_line_Label *main_label = new Out_line_Label(this->get_self());
    QMovie *movie = new QMovie;
    QMenu *menu = new QMenu(this);
    QAction *set_image = new QAction(tr("设置图像"), this);
    QAction *set_text = new QAction(tr("设置文字"), this);
    QAction *set_text_color = new QAction(tr("设置文字颜色"), this);
    QAction *set_font = new QAction(tr("设置字体"), this);
    QMenu *set_out_line_menu = new QMenu(tr("设置描边"), this);
    QAction *set_out_line_width = new QAction(tr("设置描边大小"), this);
    QAction *set_out_line_color = new QAction(tr("设置描边颜色"), this);
    QAction *clear_label = new QAction(tr("清空"), this);
    WId winId;
    QSize image_size;
private:
    void X11_Rasie();
    void auto_set_font_size();
    void contextMenuEvent(QContextMenuEvent *event);
    QSize get_Image_Size(QString path);
};
#endif // MY_LABEL_H
