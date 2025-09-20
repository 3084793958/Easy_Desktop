#include "all_control.h"
#include <QX11Info>
All_Control::All_Control(QWidget *parent)
    :QWidget(parent)
{
    setAttribute(Qt::WA_X11NetWmWindowTypeDesktop, true);
    setting_widget->background_path = background;
    main_desktop->setting_widget = setting_widget;
    main_desktop->desktop_background = background;
    main_desktop->m_WinId = this->winId();
    setting_widget->winId = this->winId();
    Update_Widget();
    main_desktop->load();
    setting_widget->Table_Update();
    show();
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
