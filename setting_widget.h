#ifndef SETTING_WIDGET_H
#define SETTING_WIDGET_H
#include <QtWidgets>
#include "desktop_background.h"
class Setting_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Setting_Widget(QWidget *parent = nullptr);
    void Table_Update();
    void private_update();
    void List_Update();
public:
    Desktop_Background *background_path;
    WId winId;
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
    QPushButton *update_button = new QPushButton(tr("更新"),this);
    QPushButton *sort_button = new QPushButton(tr("排序"),this);
    QPushButton *load_button = new QPushButton(tr("重载"),this);
    QPushButton *new_button = new QPushButton(tr("新建"),this);
    QPushButton *delete_button = new QPushButton(tr("删除"),this);
    QPushButton *set_file_button = new QPushButton(tr("为当前索引设置文件"),this);
    QSpinBox *choose_id_box = new QSpinBox(this);
    QLabel *choose_id_text = new QLabel(tr("选择id:"),this);
private:
    int Get_id_to_Index(int id);
    virtual bool eventFilter(QObject *watched, QEvent *event);
    void X11_Rasie();
    virtual void resizeEvent(QResizeEvent *event);
};

#endif // SETTING_WIDGET_H
