#ifndef EXPERIMENTAL_SETTINGS_H
#define EXPERIMENTAL_SETTINGS_H
#include <QtWidgets>
#include <QX11Info>
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
    QLabel *dbus_id_label = new QLabel(tr("dbus-id:"), this);
    QSpinBox *dbus_id_edit = new QSpinBox(this);
    QLabel *geometry_label = new QLabel(tr("空间结构:"), this);
    QCheckBox *always_refresh = new QCheckBox(tr("持续刷新"), this);
    QLineEdit *geometry_edit = new QLineEdit(this);
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
private:
    virtual void resizeEvent(QResizeEvent *event);
    void X11_Raise();
};

#endif // EXPERIMENTAL_SETTINGS_H
