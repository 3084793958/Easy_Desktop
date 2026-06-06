#ifndef WALLPAPER_INFO_DEF_H
#define WALLPAPER_INFO_DEF_H
#include <QString>
enum class Scale_Type
{
    No,
    Each,
    Width,
    Height,
    Short,//短基准
    Long,//长基准
    Full,
    User//用户定义
};
enum class Mouse_Control_Type
{
    Follow_Desktop,
    Follow_Wallpaper
};
class Path_Info
{
public:
    explicit Path_Info(uint m_id);
    explicit Path_Info(uint m_id, QString m_name, bool m_is_image, QString m_path, Scale_Type m_scale_type, bool m_center, bool m_mouse_effect,
                       qreal m_k_mouse_move_width, qreal m_k_mouse_move_height, int m_delta_x, int m_delta_y,
                       bool m_on_Antialiasing, Mouse_Control_Type m_mouse_control_type = Mouse_Control_Type::Follow_Desktop, int m_wallpaper_width = 0, int m_wallpaper_height = 0);
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
    Mouse_Control_Type mouse_control_type = Mouse_Control_Type::Follow_Desktop;
    int wallpaper_width = 0;
    int wallpaper_height = 0;
};
#endif // WALLPAPER_INFO_DEF_H
