#include <QApplication>
#include "all_control.h"
#include <stdio.h>
#include <unistd.h>
#include <QDBusConnection>
#include <QDBusMessage>
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QString load_path = QDir::homePath() + "/.local/lib/easy_desktop/config.ini";
    int workspace = 0;
    int dbus_id = 0;
    bool send_dbus = false;
    QString method = "";
    QString argument = "";
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
            if (++i < argc)
            {
                if (method == "geometry")
                {
                    if (strcmp(argv[i], "true") == 0 || strcmp(argv[i], "false") == 0)
                    {
                        argument = argv[i];
                    }
                    else
                    {
                        int x = atoi(argv[i]);
                        int y = atoi(argv[++i]);
                        int width = atoi(argv[++i]);
                        int height = atoi(argv[++i]);
                        argument = QString("%1,%2,%3,%4").arg(x).arg(y).arg(width).arg(height);
                    }
                }
                else
                {
                    argument = argv[i];
                }
            }
            break;
        }
        else
        {
            std::cout << "用法:" << std::endl;
            std::cout << "[-config/-C 配置文件路径]" << std::endl;
            std::cout << "[-workspace/-WS 工作空间索引(0为任意空间)]" << std::endl;
            std::cout << "[-dbus_id/-D_I dbus id号]" << std::endl;
            std::cout << "[-always_refresh/-A_R 是否持续刷新空间结构(geometry)(true/false)]" << std::endl;
            std::cout << "[-Geometry/-G 空间结构(geometry)(x y width height)(若存在,则-always_refresh为false)]" << std::endl;
            std::cout << "[-send_dbus/-S_D dbus id号 方法 (方法参数)]" << std::endl;
            std::cout << "方法列表:" << std::endl;
            std::cout << "[储存: save]" << std::endl;
            std::cout << "[读取: load]" << std::endl;
            std::cout << "[退出: exit]" << std::endl;
            std::cout << "[配置文件: config 路径(并不会load)]" << std::endl;
            std::cout << "[设置工作区: workspace 工作空间索引(0为任意空间)]" << std::endl;
            std::cout << "[设置空间结构: geometry 空间结构(geometry)(x y width height)(若存在,则-always_refresh为false)/(true/false)]" << std::endl;
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
        signal << dbus_id << method << argument;
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
