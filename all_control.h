#ifndef ALL_CONTROL_H
#define ALL_CONTROL_H
#include <QtWidgets>
#include "desktop_background.h"
#include "desktop_main.h"
#include "setting_widget.h"
#include "experimental_settings.h"
class All_Control : public QWidget
{
    Q_OBJECT
public:
    explicit All_Control(QWidget *parent = nullptr, QString m_load_path = "", int m_workspace = 0, int m_dbus_id = 0, bool m_always_refresh_screen_size = true, QRect m_screen_geometry = QApplication::desktop()->geometry());
    void Update_Widget();
    QString load_path = QDir::homePath() + "/.local/lib/easy_desktop/config.ini";
    bool stay_on_top = true;
    int on_top_time = 5000;
    bool workspace_donotshow = false;
    int workspace = 0;
    int dbus_id = 0;
    int keyscan_timer = 20;
    bool always_refresh_screen_size = true;
    bool allow_drop = true;
    QColor theme_color = QColor(0,129,255,255);
    QColor theme_background_color = QColor(255,255,255,75);
    QColor theme_text_color = QColor(0,0,0,255);
    QColor select_text_color = QColor(255,255,255,255);
    QColor disabled_text_color = QColor(131,136,139,255);
    QString checked_icon_path = ":/base/this.svg";
    QRect screen_geometry = QApplication::desktop()->geometry();
    QString file_open_way_process = "dde-file-manager -d -o";
    QString file_open_path_process = "dde-file-manager --show-item";
    QString file_open_info_process = "dde-file-manager -p";
    void X11_Rasie();
    void Move_To_Workspace(int human_index);
    void Refresh_geometry(QRect geometry);
private:
    int desktop_width;
    int desktop_height;
    Desktop_Background *background = new Desktop_Background(this);
    Desktop_Main *main_desktop = new Desktop_Main(this);
    Setting_Widget *setting_widget = new Setting_Widget;
    Experimental_Settings *experimental_settings = new Experimental_Settings;
    QTimer *stay_on_top_timer = new QTimer;
    QTimer *workspace_timer = new QTimer;
    QTimer *auto_save_timer = new QTimer;
private slots:
    void dbus_slot(QDBusMessage message);
};

#endif // ALL_CONTROL_H
