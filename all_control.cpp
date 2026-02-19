#include "all_control.h"
#include <QX11Info>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <QDBusConnection>
#include <QDBusMessage>
#include <QDBusPendingReply>
#include <QDBusInterface>
#include <QDBusPendingCallWatcher>
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
    Window win_Id = static_cast<Window>(winId());
    if (human_index == 0 || human_index - 1 == workspace_count)
    {
        workspace_donotshow = false;
        this->setGeometry(screen_geometry);
        this->setEnabled(true);
        XRaiseWindow(display, win_Id);
    }
    else
    {
        workspace_donotshow = true;
        this->setGeometry(QRect(-101,-101,100,100));
        this->setEnabled(false);
        XLowerWindow(display, win_Id);
        //由于没有真正地hide,能耗可能会更高,当然这只是平均的,使用show会使瞬时能耗很高
    }
    XFlush(display);
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
    experimental_settings->allow_drop = &allow_drop;
    experimental_settings->on_top_time = &on_top_time;
    experimental_settings->keyscan_timer = &keyscan_timer;
    experimental_settings->winId = this->winId();
    experimental_settings->has_been_set = true;
    experimental_settings->geometry = &screen_geometry;
    experimental_settings->always_refresh_geometry = &always_refresh_screen_size;
    experimental_settings->file_open_way_process = &file_open_way_process;
    experimental_settings->file_open_info_process = &file_open_info_process;
    experimental_settings->file_open_path_process = &file_open_path_process;
    experimental_settings->theme_color = &theme_color;
    experimental_settings->theme_background_color = &theme_background_color;
    experimental_settings->theme_text_color = &theme_text_color;
    experimental_settings->select_text_color = &select_text_color;
    experimental_settings->disabled_text_color = &disabled_text_color;
    experimental_settings->checked_icon_path = &checked_icon_path;
    main_desktop->load_path = load_path;
    main_desktop->stay_on_top = &stay_on_top;
    main_desktop->allow_drop = &allow_drop;
    main_desktop->on_top_time = &on_top_time;
    main_desktop->keyscan_timer = &keyscan_timer;
    main_desktop->file_open_way_process = &file_open_way_process;
    main_desktop->file_open_info_process = &file_open_info_process;
    main_desktop->file_open_path_process = &file_open_path_process;
    main_desktop->theme_color = &theme_color;
    main_desktop->theme_background_color = &theme_background_color;
    main_desktop->theme_text_color = &theme_text_color;
    main_desktop->select_text_color = &select_text_color;
    main_desktop->disabled_text_color = &disabled_text_color;
    main_desktop->checked_icon_path = &checked_icon_path;
    Update_Widget();
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
        Refresh_geometry(screen_geometry);
        Move_To_Workspace(workspace);
        setStyleSheet(QString("QMenu{border-radius:10px 10px;background:rgba(%1,%2,%3,%4);margin:0px -1px 0px -1px;padding-top:8px;padding-bottom:8px;icon-size:20px;border-radius:10px 10px}"
                              "QMenu::item{color:rgba(%5,%6,%7,%8);background:rgba(0,0,0,0);}"
                              "QMenu::item:disabled{color:rgba(%9,%10,%11,%12)}"
                              "QMenu::item:selected{color:rgba(%13,%14,%15,%16);background:rgba(%17,%18,%19,%20)}"
                              "QMenu::indicator{width: 20px; height 20px; background: transparent;}"
                              "QMenu::indicator:checked{image: url(%21);}"
                              "QMenu::separator{background:rgba(150,150,150,125)}")
                      .arg(theme_background_color.red()).arg(theme_background_color.green()).arg(theme_background_color.blue()).arg(theme_background_color.alpha())
                      .arg(theme_text_color.red()).arg(theme_text_color.green()).arg(theme_text_color.blue()).arg(theme_text_color.alpha())
                      .arg(disabled_text_color.red()).arg(disabled_text_color.green()).arg(disabled_text_color.blue()).arg(disabled_text_color.alpha())
                      .arg(select_text_color.red()).arg(select_text_color.green()).arg(select_text_color.blue()).arg(select_text_color.alpha())
                      .arg(theme_color.red()).arg(theme_color.green()).arg(theme_color.blue()).arg(theme_color.alpha())
                      .arg(checked_icon_path));
        main_desktop->update_for_lineedit(theme_color, theme_background_color, theme_text_color, select_text_color, disabled_text_color, checked_icon_path);
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
        setStyleSheet(QString("QMenu{border-radius:10px 10px;background:rgba(%1,%2,%3,%4);margin:0px -1px 0px -1px;padding-top:8px;padding-bottom:8px;icon-size:20px;border-radius:10px 10px}"
                              "QMenu::item{color:rgba(%5,%6,%7,%8);background:rgba(0,0,0,0);}"
                              "QMenu::item:disabled{color:rgba(%9,%10,%11,%12)}"
                              "QMenu::item:selected{color:rgba(%13,%14,%15,%16);background:rgba(%17,%18,%19,%20)}"
                              "QMenu::indicator{width: 20px; height 20px; background: transparent;}"
                              "QMenu::indicator:checked{image: url(%21);}"
                              "QMenu::separator{background:rgba(150,150,150,125)}")
                      .arg(theme_background_color.red()).arg(theme_background_color.green()).arg(theme_background_color.blue()).arg(theme_background_color.alpha())
                      .arg(theme_text_color.red()).arg(theme_text_color.green()).arg(theme_text_color.blue()).arg(theme_text_color.alpha())
                      .arg(disabled_text_color.red()).arg(disabled_text_color.green()).arg(disabled_text_color.blue()).arg(disabled_text_color.alpha())
                      .arg(select_text_color.red()).arg(select_text_color.green()).arg(select_text_color.blue()).arg(select_text_color.alpha())
                      .arg(theme_color.red()).arg(theme_color.green()).arg(theme_color.blue()).arg(theme_color.alpha())
                      .arg(checked_icon_path));
        main_desktop->update_for_lineedit(theme_color, theme_background_color, theme_text_color, select_text_color, disabled_text_color, checked_icon_path);
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
                SLOT(dbus_slot(QDBusMessage)));//怎么那么奇怪,又能跑了?
    auto_save_timer->setInterval(30000);
    connect(auto_save_timer, &QTimer::timeout, this, [=]
    {
        main_desktop->save("/tmp/Easy_Desktop/backup_config.ini");
    });
    auto_save_timer->start();
    setStyleSheet(QString("QMenu{border-radius:10px 10px;background:rgba(%1,%2,%3,%4);margin:0px -1px 0px -1px;padding-top:8px;padding-bottom:8px;icon-size:20px;border-radius:10px 10px}"
                          "QMenu::item{color:rgba(%5,%6,%7,%8);background:rgba(0,0,0,0);}"
                          "QMenu::item:disabled{color:rgba(%9,%10,%11,%12)}"
                          "QMenu::item:selected{color:rgba(%13,%14,%15,%16);background:rgba(%17,%18,%19,%20)}"
                          "QMenu::indicator{width: 20px; height 20px; background: transparent;}"
                          "QMenu::indicator:checked{image: url(%21);}"
                          "QMenu::separator{background:rgba(150,150,150,125)}")
                  .arg(theme_background_color.red()).arg(theme_background_color.green()).arg(theme_background_color.blue()).arg(theme_background_color.alpha())
                  .arg(theme_text_color.red()).arg(theme_text_color.green()).arg(theme_text_color.blue()).arg(theme_text_color.alpha())
                  .arg(disabled_text_color.red()).arg(disabled_text_color.green()).arg(disabled_text_color.blue()).arg(disabled_text_color.alpha())
                  .arg(select_text_color.red()).arg(select_text_color.green()).arg(select_text_color.blue()).arg(select_text_color.alpha())
                  .arg(theme_color.red()).arg(theme_color.green()).arg(theme_color.blue()).arg(theme_color.alpha())
                  .arg(checked_icon_path));
    main_desktop->update_for_lineedit(theme_color, theme_background_color, theme_text_color, select_text_color, disabled_text_color, checked_icon_path);
}
void All_Control::dbus_slot(QDBusMessage message)
{
    int m_dbus_id;
    QString m_method;
    QStringList m_argument;
    QList<QVariant> args = message.arguments();
    m_dbus_id = args[0].toInt();
    m_method = args[1].toString();
    for (int i = 2; i < args.size(); i++)
    {
        m_argument << args[i].toString();
    }
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
        load_path = m_argument[0];
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
        int res = m_argument[0].toInt(&ok);
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
        if (m_argument[0] == "true")
        {
            always_refresh_screen_size = true;
            experimental_settings->update_data();
            return;
        }
        else if (m_argument[0] == "false")
        {
            always_refresh_screen_size = false;
            experimental_settings->update_data();
            return;
        }
        else
        {
            always_refresh_screen_size = false;
        }
        if (m_argument.count() >= 4)
        {
            int x = m_argument[0].toInt();
            int y = m_argument[1].toInt();
            int width = m_argument[2].toInt();
            int height = m_argument[3].toInt();
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
        else
        {
            QStringList pos_list = m_argument[0].split(",");
            if (pos_list.count() >= 4)
            {
                int x = pos_list[0].toInt();
                int y = pos_list[1].toInt();
                int width = pos_list[2].toInt();
                int height = pos_list[3].toInt();
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
    else if (m_method == "volume_slider")
    {
        if (m_argument.isEmpty())
        {
            return;
        }
        int value = m_argument[0].toInt();
        main_desktop->slider_set_volume(value);
    }
    else if (m_method == "position_slider")
    {
        if (m_argument.isEmpty())
        {
            return;
        }
        int value = m_argument[0].toInt();
        main_desktop->slider_set_position(value);
    }
    else if (m_method == "speed_slider")
    {
        if (m_argument.isEmpty())
        {
            return;
        }
        int value = m_argument[0].toInt();
        main_desktop->slider_set_speed(value);
    }
    else if (m_method == "set_wallpaper")
    {
        if (m_argument.isEmpty())
        {
            return;
        }
        int wallpaper_id = m_argument[0].toInt();
        setting_widget->set_wallpaper(wallpaper_id);
    }
    else if (m_method == "remove_wallpaper")
    {
        if (m_argument.isEmpty())
        {
            return;
        }
        int wallpaper_id = m_argument[0].toInt();
        setting_widget->remove_wallpaper(wallpaper_id);
    }
    else if (m_method == "add_wallpaper")
    {
        if (m_argument.count() < 12)
        {
            return;
        }
        uint m_id = m_argument[0].toUInt();
        QString m_name = m_argument[1];
        bool m_is_image = true;
        if (m_argument[2] == "false")
        {
            m_is_image = false;
        }
        QString m_path = m_argument[3];
        Scale_Type m_scale_type;
        if (m_argument[4] == "Scale_Type::No")
        {
            m_scale_type = Scale_Type::No;
        }
        else if (m_argument[4] == "Scale_Type::Each")
        {
            m_scale_type = Scale_Type::Each;
        }
        else if (m_argument[4] == "Scale_Type::Width")
        {
            m_scale_type = Scale_Type::Width;
        }
        else if (m_argument[4] == "Scale_Type::Height")
        {
            m_scale_type = Scale_Type::Height;
        }
        else if (m_argument[4] == "Scale_Type::Short")
        {
            m_scale_type = Scale_Type::Short;
        }
        else if (m_argument[4] == "Scale_Type::Long")
        {
            m_scale_type = Scale_Type::Long;
        }
        else
        {
            m_scale_type = Scale_Type::Full;
        }
        bool m_center = true;
        if (m_argument[5] == "false")
        {
            m_center = false;
        }
        bool m_mouse_effect = false;
        if (m_argument[6] == "true")
        {
            m_mouse_effect = true;
        }
        qreal m_k_mouse_move_width = m_argument[7].toDouble();
        qreal m_k_mouse_move_height = m_argument[8].toDouble();
        int m_delta_x = m_argument[9].toInt();
        int m_delta_y = m_argument[10].toInt();
        bool m_on_Antialiasing = true;
        if (m_argument[11] == "false")
        {
            m_on_Antialiasing = false;
        }
        setting_widget->add_wallpaper(m_id, m_name, m_is_image, m_path, m_scale_type, m_center, m_mouse_effect,
                                      m_k_mouse_move_width, m_k_mouse_move_height, m_delta_x, m_delta_y,
                                      m_on_Antialiasing);
    }
}
void All_Control::Refresh_geometry(QRect geometry)
{
    if (workspace_donotshow)
    {
        return;
    }
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
    background->Desktop_point = geometry.topLeft();
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
    background->Desktop_point = this->pos();
    //background->path_list << Path_Info(0, "image", true, "/home/deepin/Pictures/2.png", Scale_Type::Full, true, true, 0.1, 0.1, 0, 0, true);
    //background->path_list << Path_Info(1, "video", false, "/usr/share/dde-introduction/demo.mp4", Scale_Type::Full, true, true, 0.1, 0.1, 0, 0, true);
    background->Path_List_Index = 0;
    background->Update_Widget();
}
