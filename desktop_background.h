#ifndef DESKTOP_BACKGROUND_H
#define DESKTOP_BACKGROUND_H
#include <QtWidgets>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <QMediaObject>
#include "keyscan.h"
#include "interfaces/wallpaper/wallpaper_plugin.h"
class Path_List : public QList<Path_Info>
{
public:
    void Sort();
};
class Desktop_Background : public QWidget
{
    Q_OBJECT
public:
    explicit Desktop_Background(QWidget *parent = nullptr);
    void set_Desktop_Size(int d_width, int d_height);
    void Update_Widget();
    void Pause();
    void Play();
    void Mouse_Pause();
    void Mouse_Play();
    bool Get_Playing_State();
    bool Get_Mouse_Playing_State();
    void Set_Position(int value);
    void Set_Volume(int value);
    void Set_Speed(int value);
    void save(QSettings *settings);
    void load(QSettings *settings);
    void geometry_change();
public:
    void plugin_settings_event();
signals:
    void send_position(int value, QString text);
public:
    Path_List path_list;
    int Path_List_Index = 0;
    bool sending_info = false;
    bool use_plugin = false;
    QPoint Desktop_point = QPoint(0, 0);
    QSize final_size_backup = QApplication::desktop()->size();
private:
    Ext_Wallpaper_PluginInterface *plugin_interface = nullptr;
    QWidget *wallpaper_plugin_item = nullptr;
    QPluginLoader *plugin_loader = new QPluginLoader(this);
    P_Sender *send_position_sender = new P_Sender(this, true, false);
    void load_plugin(QString filepath);
    void unload_plugin();
    static bool Contains_Ext_Plugin(QString Ext_name, QString plugin_controller_name);
    bool is_Ext_plugin();
public:
    QColor *theme_color = nullptr;
    QColor *theme_background_color = nullptr;
    QColor *theme_text_color = nullptr;
    QColor *select_text_color = nullptr;
    QColor *disabled_text_color = nullptr;
    QString *checked_icon_path = nullptr;
    void style_update();
public:
    int volume;
    qreal rate;
private:
    QSize get_Image_Size(QString path);
    void Second_Update_Widget();
    void mouse_move_event(int mouse_x, int mouse_y);
public:
    keyScan *btnCheck = new keyScan;
private:
    int desktop_width;
    int desktop_height;
    bool call_video_update = false;
    bool updating = false;
    QPoint base_point;
    QLabel *image_background = new QLabel(this);
    QMovie *image_movie = new QMovie;
    QGraphicsScene *scene = new QGraphicsScene(this);
    QGraphicsView *graphicsView = new QGraphicsView(scene, this);
    QMediaPlayer *media_player = new QMediaPlayer(this);
    QGraphicsVideoItem *video_item = new QGraphicsVideoItem;
    QTimer *holding_pos_timer = new QTimer(this);
    int holding_time = 0;
    int holding_value = 0;
    int holding_max_time = 10;
    QSize save_final_size = QSize(0, 0);
};
#endif // DESKTOP_BACKGROUND_H
