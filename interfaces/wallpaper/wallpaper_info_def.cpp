#include "wallpaper_info_def.h"
Path_Info::Path_Info(uint m_id)
{
    id = m_id;
    Empty = true;
}
Path_Info::Path_Info(uint m_id, QString m_name, bool m_is_image, QString m_path, Scale_Type m_scale_type, bool m_center, bool m_mouse_effect,
                     qreal m_k_mouse_move_width, qreal m_k_mouse_move_height, int m_delta_x, int m_delta_y,
                     bool m_on_Antialiasing, Mouse_Control_Type m_mouse_control_type, int m_wallpaper_width, int m_wallpaper_height)
{
    Empty = false;
    id = m_id;
    name = m_name;
    is_image = m_is_image;
    path = m_path;
    scale_type = m_scale_type;
    center = m_center;
    mouse_effect = m_mouse_effect;
    k_mouse_move_width = m_k_mouse_move_width;
    k_mouse_move_height = m_k_mouse_move_height;
    delta_x = m_delta_x;
    delta_y = m_delta_y;
    on_Antialiasing = m_on_Antialiasing;
    mouse_control_type = m_mouse_control_type;
    wallpaper_width = m_wallpaper_width;
    wallpaper_height = m_wallpaper_height;
}
