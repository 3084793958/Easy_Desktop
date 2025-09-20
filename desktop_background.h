#ifndef DESKTOP_BACKGROUND_H
#define DESKTOP_BACKGROUND_H
#include <QtWidgets>
#include <QMediaPlayer>
#include <QGraphicsVideoItem>
#include <QMediaObject>
#include "keyscan.h"
enum class Scale_Type
{
    No,
    Each,
    Width,
    Height,
    Short,//短基准
    Long,//长基准
    Full
};
class Path_Info
{
public:
    explicit Path_Info(uint m_id);
    explicit Path_Info(uint m_id, QString m_name, bool m_is_image, QString m_path, Scale_Type m_scale_type, bool m_center, bool m_mouse_effect,
                       qreal m_k_mouse_move_width, qreal m_k_mouse_move_height, int m_delta_x, int m_delta_y,
                       bool m_on_Antialiasing);
public:
    bool Empty;
    uint id;
    QString name;
    bool is_image;
    QString path;
    Scale_Type scale_type;
    bool center;
    bool mouse_effect;
    qreal k_mouse_move_width;
    qreal k_mouse_move_height;
    int delta_x;
    int delta_y;
    bool on_Antialiasing;
};
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
signals:
    void send_position(int value, QString text);
public:
    Path_List path_list;
    int Path_List_Index = 0;
    bool sending_info = false;
public:
    int volume;
    qreal rate;
private:
    QSize get_Image_Size(QString path);
    void Second_Update_Widget();
    void mouse_move_event(int mouse_x, int mouse_y);
private:
    keyScan *btnCheck = new keyScan;
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
};
#endif // DESKTOP_BACKGROUND_H
