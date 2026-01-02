#include "all_control.h"
#include <QX11Info>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <QDBusConnection>
#include <QDBusMessage>
#include<QDBusPendingReply>
#include<QDBusInterface>
#include<QDBusPendingCallWatcher>
void All_Control::X11_Rasie()
{
    Window win_Id = static_cast<Window>(winId());
    Display *display = QX11Info::display();
    XRaiseWindow(display, win_Id);
    XFlush(display);
}
void All_Control::Move_To_Workspace(int human_index)
{
    int workspace_count = 0;
    Display *display = QX11Info::display();
    Atom desktop_atom = XInternAtom(display, "_NET_CURRENT_DESKTOP", False);
    Atom type;
    int format;
    unsigned long nitems, bytes;
    unsigned char *data = nullptr;
    if (XGetWindowProperty(display, DefaultRootWindow(display), desktop_atom, 0, 1, False, XA_CARDINAL, &type, &format, &nitems, &bytes, &data) == Success)
    {
        if (data)
        {
            workspace_count = static_cast<int>(*(reinterpret_cast<long *>(data)));
            XFree(data);
        }
    }
    if (human_index == 0 || human_index - 1 == workspace_count)
    {
        show();
    }
    else
    {
        hide();
    }
}
#undef CursorShape
All_Control::All_Control(QWidget *parent, QString m_load_path, int m_workspace, int m_dbus_id, bool m_always_refresh_screen_size, QRect m_screen_geometry)
    :QWidget(parent)
    ,load_path(m_load_path)
    ,workspace(m_workspace)
    ,dbus_id(m_dbus_id)
    ,always_refresh_screen_size(m_always_refresh_screen_size)
    ,screen_geometry(m_screen_geometry)
{
    setAttribute(Qt::WA_X11NetWmWindowTypeDesktop, true);
    setting_widget->background_path = background;
    main_desktop->setting_widget = setting_widget;
    main_desktop->experimental_settings = experimental_settings;
    main_desktop->desktop_background = background;
    main_desktop->m_WinId = this->winId();
    setting_widget->winId = this->winId();
    experimental_settings->workspace = &workspace;
    experimental_settings->dbus_id = &dbus_id;
    experimental_settings->load_path = &load_path;
    experimental_settings->stay_on_top = &stay_on_top;
    experimental_settings->on_top_time = &on_top_time;
    experimental_settings->keyscan_timer = &keyscan_timer;
    experimental_settings->winId = this->winId();
    experimental_settings->has_been_set = true;
    experimental_settings->geometry = &screen_geometry;
    experimental_settings->always_refresh_geometry = &always_refresh_screen_size;
    Update_Widget();
    main_desktop->load_path = load_path;
    main_desktop->stay_on_top = &stay_on_top;
    main_desktop->on_top_time = &on_top_time;
    main_desktop->keyscan_timer = &keyscan_timer;
    main_desktop->load();
    setting_widget->Table_Update();
    stay_on_top_timer->setInterval(on_top_time);
    if (stay_on_top)
    {
        stay_on_top_timer->start();
    }
    else
    {
        stay_on_top_timer->stop();
    }
    background->btnCheck->signal_delta_time = keyscan_timer;
    experimental_settings->update_data();
    Move_To_Workspace(workspace);
    show();
    connect(stay_on_top_timer, &QTimer::timeout, this, [=]
    {
        this->X11_Rasie();//将错就错吧
    });
    connect(experimental_settings, &Experimental_Settings::has_sended, this , [=]
    {
        main_desktop->load_path = load_path;
        stay_on_top_timer->setInterval(on_top_time);
        if (stay_on_top)
        {
            stay_on_top_timer->start();
        }
        else
        {
            stay_on_top_timer->stop();
        }
        background->btnCheck->signal_delta_time = keyscan_timer;
        Move_To_Workspace(workspace);
        Refresh_geometry(screen_geometry);
    });
    connect(main_desktop, &Desktop_Main::keyscan_loaded, this, [=]
    {
        stay_on_top_timer->setInterval(on_top_time);
        if (stay_on_top)
        {
            stay_on_top_timer->start();
        }
        else
        {
            stay_on_top_timer->stop();
        }
        background->btnCheck->signal_delta_time = keyscan_timer;
        experimental_settings->update_data();
    });
    workspace_timer->setInterval(1000);
    connect(workspace_timer, &QTimer::timeout, this, [=]
    {
        Move_To_Workspace(workspace);
        if (always_refresh_screen_size)
        {
            screen_geometry = QApplication::desktop()->geometry();
            Refresh_geometry(QApplication::desktop()->geometry());
        }
    });
    workspace_timer->start();
    Refresh_geometry(screen_geometry);
    QDBusConnection::sessionBus().connect(
                "",
                "/my/Easy_Desktop",
                "my.Easy_Desktop",
                "Easy_Desktop",
                this,
                SLOT(dbus_slot(int, QString, QString)));//怎么那么奇怪,又能跑了?
}
void All_Control::dbus_slot(int m_dbus_id, QString m_method, QString m_argument)
{
    if (m_dbus_id != dbus_id)
    {
        return;
    }
    if (m_method == "save")
    {
        main_desktop->save();
    }
    else if (m_method == "load")
    {
        main_desktop->load();
    }
    else if (m_method == "exit")
    {
        QApplication::exit();
    }
    else if (m_method == "config")
    {
        if (m_argument.isEmpty())
        {
            return;
        }
        load_path = m_argument;
        main_desktop->load_path = load_path;
        experimental_settings->update_data();
    }
    else if(m_method == "workspace")
    {
        if (m_argument.isEmpty())
        {
            return;
        }
        bool ok = false;
        int res = m_argument.toInt(&ok);
        if (ok)
        {
            workspace = res;
            Move_To_Workspace(res);
            experimental_settings->update_data();
        }
    }
    else if (m_method == "geometry")
    {
        if (m_argument.isEmpty())
        {
            return;
        }
        if (m_argument == "true")
        {
            always_refresh_screen_size = true;
            experimental_settings->update_data();
            return;
        }
        else if (m_argument == "false")
        {
            always_refresh_screen_size = false;
            experimental_settings->update_data();
            return;
        }
        else
        {
            always_refresh_screen_size = false;
        }
        QStringList list = m_argument.split(",");
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
            experimental_settings->update_data();
            Refresh_geometry(screen_geometry);
        }
    }
}
void All_Control::Refresh_geometry(QRect geometry)
{
    if (this->geometry() == geometry)
    {
        return;
    }
    if (geometry.width() <= 0 || geometry.height() <= 0)
    {
        return;
    }
    desktop_width = geometry.width();
    desktop_height = geometry.height();
    setGeometry(geometry);
    main_desktop->raise();
    main_desktop->basic_pos = geometry.topLeft();
    main_desktop->set_Desktop_Size(desktop_width, desktop_height);
    background->set_Desktop_Size(desktop_width, desktop_height);
    //
    main_desktop->geometry_change();
    background->geometry_change();
    //
}
void All_Control::Update_Widget()
{
    desktop_width = QApplication::desktop()->width();
    desktop_height = QApplication::desktop()->height();
    resize(desktop_width, desktop_height);
    move(0, 0);
    main_desktop->raise();
    main_desktop->set_Desktop_Size(desktop_width, desktop_height);
    main_desktop->Connection_Update();
    main_desktop->Update_Widget();
    background->set_Desktop_Size(desktop_width, desktop_height);
    //background->path_list << Path_Info(0, "image", true, "/home/deepin/Pictures/2.png", Scale_Type::Full, true, true, 0.1, 0.1, 0, 0, true);
    //background->path_list << Path_Info(1, "video", false, "/usr/share/dde-introduction/demo.mp4", Scale_Type::Full, true, true, 0.1, 0.1, 0, 0, true);
    background->Path_List_Index = 0;
    background->Update_Widget();
}
