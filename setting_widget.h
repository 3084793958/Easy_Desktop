#ifndef SETTING_WIDGET_H
#define SETTING_WIDGET_H
#include <QtWidgets>
#include "desktop_background.h"
#include "core/tools/trans_action.h"
class Setting_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Setting_Widget(QWidget *parent = nullptr);
    void Trans_update();
    void Table_Update();
    void private_update();
    void List_Update();
    void set_wallpaper(int wallpaper_id);
    void remove_wallpaper(int wallpaper_id);
    void add_wallpaper(uint m_id, QString m_name, bool m_is_image, QString m_path, Scale_Type m_scale_type, bool m_center, bool m_mouse_effect,
                       qreal m_k_mouse_move_width, qreal m_k_mouse_move_height, int m_delta_x, int m_delta_y,
                       bool m_on_Antialiasing, Mouse_Control_Type m_mouse_control_type = Mouse_Control_Type::Follow_Desktop, int m_wallpaper_width = 0, int m_wallpaper_height = 0);
public:
    Desktop_Background *background_path;
private:
    Path_List path_list;
    int path_list_index;
    int choose_id;
    QTableWidget *table_widget = new QTableWidget(this);
    QList<QSpinBox*> id_box_list;
    QList<QLineEdit*> name_box_list;
    QList<QComboBox*> image_box_list;
    QList<QLineEdit*> path_box_list;
    QList<QComboBox*> scale_box_list;
    QList<QComboBox*> center_box_list;
    QList<QComboBox*> mouse_effect_box_list;
    QList<QDoubleSpinBox*> mouse_width_box_list;
    QList<QDoubleSpinBox*> mouse_height_box_list;
    QList<QSpinBox*> delta_x_box_list;
    QList<QSpinBox*> delta_y_box_list;
    QList<QComboBox*> on_Antialiasing_box_list;
    QList<QComboBox*> mouse_control_type_box_list;
    QList<QSpinBox*> wallpaper_width_box_list;
    QList<QSpinBox*> wallpaper_height_box_list;
    QPushButton *update_button = new Trans_PushButton(tr("更新"), "更新", this->metaObject()->className(), this);
    QPushButton *sort_button = new Trans_PushButton(tr("排序"), "排序", this->metaObject()->className(), this);
    QPushButton *load_button = new Trans_PushButton(tr("重载"), "重载", this->metaObject()->className(), this);
    QPushButton *new_button = new Trans_PushButton(tr("新建"), "新建", this->metaObject()->className(), this);
    QPushButton *delete_button = new Trans_PushButton(tr("删除"), "删除", this->metaObject()->className(), this);
    QPushButton *set_file_button = new Trans_PushButton(tr("为当前索引设置文件"), "为当前索引设置文件", this->metaObject()->className(), this);
    QPushButton *load_img_button = new Trans_PushButton(tr("批量加载图像"), "批量加载图像", this->metaObject()->className(), this);
    QPushButton *load_video_button = new Trans_PushButton(tr("批量加载视频"), "批量加载视频", this->metaObject()->className(), this);
    QPushButton *load_plugin_button = new Trans_PushButton(tr("批量加载插件"), "批量加载插件", this->metaObject()->className(), this);
    QPushButton *plugin_settings_button = new Trans_PushButton(tr("插件设置"), "插件设置", this->metaObject()->className(), this);
    QSpinBox *choose_id_box = new QSpinBox(this);
    QLabel *choose_id_text = new Trans_Label(tr("选择id:"), "选择id:", this->metaObject()->className(), this);
private:
    int Get_id_to_Index(int id);
    virtual bool eventFilter(QObject *watched, QEvent *event);
    virtual void resizeEvent(QResizeEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
};

#endif // SETTING_WIDGET_H
