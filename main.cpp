/*************************************************************************/
/* main.cpp                                                              */
/*                              GPL v3.0                                 */
/*                  This file is part of: Easy_Desktop                   */
/*          <https://github.com/3084793958/Easy_Desktop.git>             */
/*************************************************************************/
/* Copyright (C) 2025-present github.com:3084793958                      */
/*                                                                       */
/* This program is free software: you can redistribute it and/or modify  */
/* it under the terms of the GNU General Public License as published by  */
/* the Free Software Foundation, either version 3 of the License, or     */
/* (at your option) any later version.                                   */
/*                                                                       */
/* This program is distributed in the hope that it will be useful,       */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of        */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         */
/* GNU General Public License for more details.                          */
/*                                                                       */
/* You should have received a copy of the GNU General Public License     */
/* along with this program.  If not, see <https://www.gnu.org/licenses/>.*/
/*************************************************************************/
#include <QApplication>
#include "all_control.h"
#include <stdio.h>
#include <unistd.h>
#include <QDBusConnection>
#include <QDBusMessage>
#include <DApplication>
int main(int argc, char* argv[])
{
    Dtk::Widget::DApplication app(argc, argv);
    QString load_path = QDir::homePath() + "/.local/lib/easy_desktop/config.ini";
    int workspace = 0;
    int dbus_id = 0;
    bool send_dbus = false;
    QString method = "";
    QStringList argument;
    bool always_refresh_screen_size = true;
    QRect screen_geometry = QApplication::desktop()->geometry();
    bool has_set_geometry = false;
    for (int i = 1; i <argc; i++)
    {
        if ((strcmp(argv[i], "-config") == 0 || strcmp(argv[i], "-C") == 0) && i + 1 < argc)
        {
            load_path = argv[++i];
        }
        else if ((strcmp(argv[i], "-workspace") == 0 || strcmp(argv[i], "-WS") == 0) && i + 1 < argc)
        {
            workspace = atoi(argv[++i]);
        }
        else if ((strcmp(argv[i], "-dbus_id") == 0 || strcmp(argv[i], "-D_I") == 0) && i + 1 < argc)
        {
            dbus_id = atoi(argv[++i]);
        }
        else if ((strcmp(argv[i], "-always_refresh") == 0 || strcmp(argv[i], "-A_R") == 0) && i + 1 < argc)
        {
            if (has_set_geometry)
            {
                continue;
            }
            always_refresh_screen_size = strcmp(argv[++i], "false") != 0;
        }
        else if ((strcmp(argv[i], "-G") == 0 || strcmp(argv[i], "-Geometry")/*很奇怪-geometry过不了*/ == 0) && i + 4 < argc)
        {
            has_set_geometry = true;
            always_refresh_screen_size = false;
            int x = atoi(argv[++i]);
            int y = atoi(argv[++i]);
            int width = atoi(argv[++i]);
            int height = atoi(argv[++i]);
            if (width == 0 || height == 0)
            {
                screen_geometry = QRect(x, y, screen_geometry.width(), screen_geometry.height());
            }
            else
            {
                screen_geometry = QRect(x, y, width, height);
            }
        }
        else if ((strcmp(argv[i], "-send_dbus") == 0 || strcmp(argv[i], "-S_D") == 0) && i + 2 < argc)
        {
            send_dbus = true;
            dbus_id = atoi(argv[++i]);
            method = argv[++i];
            for (i++; i < argc; i++)
            {
                argument << argv[i];
            }
            break;
        }
        else
        {
            std::cout << "用法: /path/to/Easy_Desktop [选项]" << std::endl << std::endl;
            std::cout << "示例: ./Easy_Desktop -G 0 0 1440 900" << std::endl;
            std::cout << "选项:" << std::endl;
            std::cout << "  -config, -C <路径>                          指定配置文件路径" << std::endl;
            std::cout << "  -workspace, -WS <索引号>                    设置工作空间索引 (0 表示任意空间)" << std::endl;
            std::cout << "  -dbus_id, -D_I <ID>                        设置 dbus_id 号" << std::endl;
            std::cout << "  -always_refresh, -A_R <布尔值>              是否持续刷新空间结构 (true/false)" << std::endl;
            std::cout << "  -Geometry, -G <x y width height>           设置空间结构 (设置后 -always_refresh 自动为 false)" << std::endl;
            std::cout << "  -send_dbus, -S_D <dbus_id> <方法> [参数]    发送 DBus 消息" << std::endl << std::endl;
            std::cout << "示例: ./Easy_Desktop -S_D 0 add_wallpaper 0 deepin true /usr/share/wallpapers/deepin/Deepin-Technology-Brand-Logo.jpg Scale_Type::Full true true 0.1 0.1 0 0 true" << std::endl;
            std::cout << "DBus 方法列表:" << std::endl;
            std::cout << "  save                             储存" << std::endl;
            std::cout << "  load                             读取" << std::endl;
            std::cout << "  exit                             退出" << std::endl;
            std::cout << "  config <路径>                     设置配置文件路径 (不自动加载)" << std::endl;
            std::cout << "  workspace <索引>                  切换工作空间" << std::endl;
            std::cout << "  geometry <值>                    设置空间结构或刷新模式" << std::endl;
            std::cout << "                                   格式1: x y width height (禁用自动刷新)" << std::endl;
            std::cout << "                                   格式2: true/false (设置自动刷新开关)" << std::endl;
            std::cout << "  volume_slider <数值>              设置音量进度条 (取值范围: 0-100 的整数)" << std::endl;
            std::cout << "  position_slider <数值>            设置位置进度条 (取值范围: 0-100 的整数)" << std::endl;
            std::cout << "  speed_slider <数值>               设置速度进度条 (取值范围: 10-300 的整数)" << std::endl;
            std::cout << "  set_wallpaper <wallpaper_id>      设置壁纸" << std::endl;
            std::cout << "  remove_wallpaper <wallpaper_id>   移除壁纸" << std::endl;
            std::cout << "  add_wallpaper [参数]               添加壁纸" << std::endl;
            std::cout << "                ID(UInt32)" << std::endl;
            std::cout << "                名称(String)" << std::endl;
            std::cout << "                显示方式(String)     [图像:true/视频:false](gif属于图像)" << std::endl;
            std::cout << "                路径(String)" << std::endl;
            std::cout << "                缩放方式(String)     [Scale_Type::No/Scale_Type::Each/Scale_Type::Width/Scale_Type::Height/Scale_Type::Short/Scale_Type::Long/Scale_Type::Full]" << std::endl;
            std::cout << "                居中(String)        [true/false]" << std::endl;
            std::cout << "                鼠标效果(String)     [true/false]" << std::endl;
            std::cout << "                鼠标效果宽度系数(Double)" << std::endl;
            std::cout << "                鼠标效果高度系数(Double)" << std::endl;
            std::cout << "                X轴偏移量(Int32)" << std::endl;
            std::cout << "                Y轴偏移量(Int32)" << std::endl;
            std::cout << "                抗锯齿(String)       [true/false]" << std::endl;
            app.exit();
            return 0;
        }
    }
    if (send_dbus)
    {
        QDBusMessage signal = QDBusMessage::createSignal(
                    "/my/Easy_Desktop",
                    "my.Easy_Desktop",
                    "Easy_Desktop");
        signal << dbus_id << method;
        if (argument.count() == 0)
        {
            argument << "";
        }
        for (int i = 0; i < argument.count(); i++)
        {
            signal << argument[i];//直接加他是真直接加啊
        }
        QDBusConnection::sessionBus().send(signal);
        app.exit();
        return 0;
    }
    app.setQuitOnLastWindowClosed(false);
    app.setApplicationName("Easy_Desktop");
    QTranslator qtTranslator;
    if (qtTranslator.load("qtbase_zh_CN.qm",":/base"))
    {
        app.installTranslator(&qtTranslator);
    }
    All_Control *all_control = new All_Control(nullptr, load_path, workspace, dbus_id, always_refresh_screen_size, screen_geometry);
    std::cout<<"Easy_Desktop: "<<">(O^<)<~GET!"<<std::endl;
    all_control->show();
    return app.exec();
}
