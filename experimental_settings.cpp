#include "experimental_settings.h"
#include <QX11Info>
#include <X11/Xlib.h>
void Experimental_Settings::X11_Raise()
{
    if (!has_been_set)
    {
        return;
    }
    Window win_Id = static_cast<Window>(winId);
    Display *display = QX11Info::display();
    XRaiseWindow(display, win_Id);
    XFlush(display);
}
#undef CursorShape
Experimental_Settings::Experimental_Settings(QWidget *parent)
    :QWidget(parent)
{
    this->setMinimumSize(450, 235);
    this->setWindowTitle("实验设置");
    load_path_label->move(5, 5);
    load_path_label->resize(70, 40);
    load_path_edit->move(75, 5);
    load_path_edit->resize(230, 40);
    load_path_edit->setPlaceholderText(QDir::homePath() + "/.local/lib/easy_desktop/config.ini");
    set_load_path->move(310, 5);
    set_load_path->resize(85, 40);
    workspace_label->move(5, 50);
    workspace_label->resize(50, 40);
    workspace_box->move(60, 50);
    workspace_box->resize(165, 40);
    workspace_box->setRange(0, 2147483647);
    workspace_box->setValue(0);
    stay_on_top_box->move(5, 95);
    stay_on_top_box->resize(100, 40);
    stay_on_top_timer_label->move(110, 95);
    stay_on_top_timer_label->resize(100, 40);
    stay_on_top_timer_edit->setRange(10, 2147483647);
    stay_on_top_timer_edit->setValue(5000);
    stay_on_top_timer_edit->move(215, 95);
    stay_on_top_timer_edit->resize(180, 40);
    keyscan_label->move(5, 140);
    keyscan_label->resize(100, 40);
    keyscan_timer_edit->setRange(10, 2147483647);
    keyscan_timer_edit->setValue(20);
    keyscan_timer_edit->move(110, 140);
    keyscan_timer_edit->resize(180, 40);
    dbus_id_label->move(230, 50);
    dbus_id_label->resize(60, 40);
    dbus_id_edit->move(290, 50);
    dbus_id_edit->resize(165, 40);
    dbus_id_edit->setRange(0, 2147483647);
    dbus_id_edit->setValue(0);
    geometry_label->move(5, 190);
    geometry_label->resize(70, 40);
    always_refresh->move(80, 190);
    always_refresh->resize(90, 40);
    geometry_edit->move(175, 190);
    geometry_edit->resize(170, 40);
    update_button->move(310, 190);
    update_button->resize(85, 40);
    resize(450, 235);
    connect(update_button, &QPushButton::clicked, this, &Experimental_Settings::send_data);
    connect(set_load_path, &QPushButton::clicked, this, [=]
    {
        QString filename = QFileDialog::getOpenFileName(nullptr, "获取配置文件", QDir::homePath(), "配置文件(*.ini);;所有文件(*.*)");
        X11_Raise();
        if (!filename.isEmpty())
        {
            load_path_edit->setText(filename);
        }
    });
}
void Experimental_Settings::resizeEvent(QResizeEvent *event)
{
    set_load_path->move(event->size().width() - 90, 5);
    load_path_edit->resize(event->size().width() - 170, 40);
    update_button->move(event->size().width() - 90, 190);
    dbus_id_label->move(event->size().width() / 2 + 5, 50);
    dbus_id_edit->move(event->size().width() / 2 + 65, 50);
    dbus_id_edit->resize(event->size().width() / 2 - 70, 40);
    stay_on_top_timer_edit->resize(event->size().width() - 220, 40);
    keyscan_timer_edit->resize(event->size().width() - 220, 40);
    workspace_box->resize(event->size().width() / 2 - 60, 40);
    geometry_edit->resize(event->size().width() - 280, 40);
}
void Experimental_Settings::update_data()
{
    load_path_edit->setText(*load_path);
    dbus_id_edit->setValue(*dbus_id);
    keyscan_timer_edit->setValue(*keyscan_timer);
    stay_on_top_box->setCheckState(*stay_on_top ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    stay_on_top_timer_edit->setValue(*on_top_time);
    workspace_box->setValue(*workspace);
    always_refresh->setCheckState(*always_refresh_geometry ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    geometry_edit->setText(QString("%1,%2,%3,%4").arg(geometry->x()).arg(geometry->y()).arg(geometry->width()).arg(geometry->height()));
}
void Experimental_Settings::send_data()
{
    *load_path = load_path_edit->text();
    *dbus_id = dbus_id_edit->value();
    *keyscan_timer = keyscan_timer_edit->value();
    *stay_on_top = stay_on_top_box->checkState() == Qt::CheckState::Checked;
    *on_top_time = stay_on_top_timer_edit->value();
    *workspace = workspace_box->value();
    *always_refresh_geometry = always_refresh->checkState() == Qt::CheckState::Checked;
    if (*always_refresh_geometry)
    {
        *geometry = QApplication::desktop()->geometry();
        geometry_edit->setText(QString("%1,%2,%3,%4").arg(geometry->x()).arg(geometry->y()).arg(geometry->width()).arg(geometry->height()));
    }
    else
    {
        QStringList list = geometry_edit->text().split(",");
        QRect screen_geometry = QApplication::desktop()->geometry();
        if (list.count() == 4)
        {
            int x = list[0].toInt();
            int y = list[1].toInt();
            int width = list[2].toInt();
            int height = list[3].toInt();
            if (width == 0 || height == 0)
            {
                screen_geometry = QRect(x, y, screen_geometry.width(), screen_geometry.height());
            }
            else
            {
                screen_geometry = QRect(x, y, width, height);
            }
        }
        *geometry = screen_geometry;
    }
    emit has_sended();
}
