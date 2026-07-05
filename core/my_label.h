#ifndef MY_LABEL_H
#define MY_LABEL_H
#include "basic_widget.h"
#include <QDBusConnection>
#include <QDBusMessage>
#include<QDBusPendingReply>
#include<QDBusInterface>
#include<QDBusPendingCallWatcher>
class My_Label : public Basic_Widget
{
    Q_OBJECT
public:
    explicit My_Label(QWidget *parent = nullptr);
    ~My_Label();
    QList<My_Label *> *my_label_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
    virtual void set_icon(QString checked_icon_path);
private:
    Out_line_Label *main_label = new Out_line_Label(this->get_self());
    QMovie *movie = new QMovie;
    QMenu *menu = new QMenu(this);
    QMenu *set_dbus_service = new Trans_Menu(tr("设置dbus服务"), "设置dbus服务", this->metaObject()->className(), this);
    QAction *dbus_setup_action = new Trans_Action(tr("启用"), "启用", this->metaObject()->className(), this);
    QAction *set_dbus_info = new Trans_Action(tr("设置信息"), "设置信息", this->metaObject()->className(), this);
    QAction *set_image = new Trans_Action(tr("设置图像"), "设置图像", this->metaObject()->className(), this);
    QAction *set_text = new Trans_Action(tr("设置文字"), "设置文字", this->metaObject()->className(), this);
    QAction *set_text_color = new Trans_Action(tr("设置文字颜色"), "设置文字颜色", this->metaObject()->className(), this);
    QAction *set_font = new Trans_Action(tr("设置字体"), "设置字体", this->metaObject()->className(), this);
    QMenu *set_out_line_menu = new Trans_Menu(tr("设置描边"), "设置描边", this->metaObject()->className(), this);
    QAction *set_out_line_width = new Trans_Action(tr("设置描边大小"), "设置描边大小", this->metaObject()->className(), this);
    QAction *set_out_line_color = new Trans_Action(tr("设置描边颜色"), "设置描边颜色", this->metaObject()->className(), this);
    QAction *clear_label = new Trans_Action(tr("清空"), "清空", this->metaObject()->className(), this);
    QSize image_size;
    QDBusConnection dbus = QDBusConnection::sessionBus();
    QString dbus_service = "";
    QString dbus_path = "";
    QString dbus_interface = "";
    QString dbus_name = "";
    bool dbus_setup = false;
private slots:
    void DBusMessageReceived(QDBusMessage message);
private:
    void auto_set_font_size();
    void contextMenuEvent(QContextMenuEvent *event);
    QSize get_Image_Size(QString path);
};
#endif // MY_LABEL_H
