#ifndef EXPERIMENTAL_SETTINGS_H
#define EXPERIMENTAL_SETTINGS_H
#include <QtWidgets>
#include <QX11Info>
class Theme_Set_Dialog : public QWidget
{
    Q_OBJECT
public:
    explicit Theme_Set_Dialog(QWidget *parent = nullptr, QColor *m_theme_color = nullptr, QColor *m_theme_background_color = nullptr, QColor *m_theme_text_color = nullptr, QColor *m_select_text_color = nullptr, QColor *m_disabled_text_color = nullptr, QString *m_checked_icon_path = nullptr);
    void update_data();
signals:
    void Call_X11_Raise();
private:
    QLabel *theme_color_label = new QLabel(tr("主题色:"), this);
    QPushButton *theme_color_button = new QPushButton(this);
    QLabel *theme_background_color_label = new QLabel(tr("背景色:"), this);
    QPushButton *theme_background_color_button = new QPushButton(this);
    QLabel *theme_text_color_label = new QLabel(tr("字体色:"), this);
    QPushButton *theme_text_color_button = new QPushButton(this);
    QLabel *select_text_color_label = new QLabel(tr("选择字体色:"), this);
    QPushButton *select_text_color_button = new QPushButton(this);
    QLabel *disabled_text_color_label = new QLabel(tr("禁用字体色:"), this);
    QPushButton *disabled_text_color_button = new QPushButton(this);
    QLabel *checked_icon_path_label = new QLabel(tr("选中图标:"), this);
    QComboBox *checked_icon_path_box = new QComboBox(this);
    QPushButton *checked_icon_path_load_button = new QPushButton(tr("导入图像"), this);
    void set_color(QColor *ptr);
private:
    QColor *theme_color;
    QColor *theme_background_color;
    QColor *theme_text_color;
    QColor *select_text_color;
    QColor *disabled_text_color;
    QString *checked_icon_path;
};
class Experimental_Settings : public QWidget
{
    Q_OBJECT
public:
    explicit Experimental_Settings(QWidget *parent = nullptr);
    QLabel *load_path_label = new QLabel(tr("配置文件:"), this);
    QLineEdit *load_path_edit = new QLineEdit(this);
    QPushButton *set_load_path =new QPushButton(tr("设置路径"), this);
    QLabel *workspace_label = new QLabel(tr("工作区:"), this);
    QSpinBox *workspace_box = new QSpinBox(this);
    QCheckBox *stay_on_top_box = new QCheckBox(tr("置顶"), this);
    QLabel *stay_on_top_timer_label = new QLabel(tr("置顶侦测(ms):"), this);
    QSpinBox *stay_on_top_timer_edit = new QSpinBox(this);
    QLabel *keyscan_label = new QLabel(tr("鼠标侦测(ms):"), this);
    QSpinBox *keyscan_timer_edit = new QSpinBox(this);
    QCheckBox *allow_drop_box = new QCheckBox(tr("允许拖入"), this);
    QLabel *dbus_id_label = new QLabel(tr("dbus-id:"), this);
    QSpinBox *dbus_id_edit = new QSpinBox(this);
    QLabel *geometry_label = new QLabel(tr("空间结构:"), this);
    QCheckBox *always_refresh = new QCheckBox(tr("持续刷新"), this);
    QLineEdit *geometry_edit = new QLineEdit(this);
    QLabel *file_open_way_label = new QLabel(tr("[打开方式]进程:"), this);
    QLineEdit *file_open_way_edit = new QLineEdit(this);
    QLabel *file_path_way_label = new QLabel(tr("[打开文件位置]进程:"), this);
    QLineEdit *file_path_way_edit = new QLineEdit(this);
    QLabel *file_info_way_label = new QLabel(tr("[属性]进程:"), this);
    QLineEdit *file_info_way_edit = new QLineEdit(this);
    QPushButton *set_theme_color_button = new QPushButton(tr("颜色设置"), this);
    QPushButton *update_button = new QPushButton(tr("更新"), this);
    void update_data();
    void send_data();
    bool has_been_set = false;
    WId winId;
signals:
    void has_sended();
public:
    QString *load_path;
    int *workspace;
    bool *stay_on_top;
    int *on_top_time;
    int *dbus_id;
    int *keyscan_timer;
    QRect *geometry;
    bool *always_refresh_geometry;
    bool *allow_drop;
    QString *file_open_way_process;
    QString *file_open_path_process;
    QString *file_open_info_process;
    QColor *theme_color;
    QColor *theme_background_color;
    QColor *theme_text_color;
    QColor *select_text_color;
    QColor *disabled_text_color;
    QString *checked_icon_path;
    QColor m_theme_color;
    QColor m_theme_background_color;
    QColor m_theme_text_color;
    QColor m_select_text_color;
    QColor m_disabled_text_color;
    QString m_checked_icon_path;
    Theme_Set_Dialog *set_theme_dialog = new Theme_Set_Dialog(nullptr, &m_theme_color, &m_theme_background_color, &m_theme_text_color, &m_select_text_color, &m_disabled_text_color, &m_checked_icon_path);
private:
    virtual void resizeEvent(QResizeEvent *event);
    void X11_Raise();
};

#endif // EXPERIMENTAL_SETTINGS_H
