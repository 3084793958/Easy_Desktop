#ifndef MY_MUSIC_VISIBLE_H
#define MY_MUSIC_VISIBLE_H
#include "basic_widget.h"
#include <pulse/pulseaudio.h>
struct AudioLevel
{
    double left_level;
    double right_level;
    double peak_level;
    double rms_level;
};
class Audio_Wave_Monitor : public QObject
{
    Q_OBJECT
public:
    explicit Audio_Wave_Monitor(QObject *parent);
    ~Audio_Wave_Monitor();
public:
    void _init();
    void _delete();
    void start();
    void stop();
    void set_timer_Interval(int Interval = -1);
    void get_device_list();
    void set_stream_info(int type, uint index);
    QList<QPair<QString, uint>> sink_list;
    QList<QPair<QString, uint>> source_list;
    QList<QPair<QString, uint>> sink_input_list;
    QList<QPair<QString, uint>> source_output_list;
public:
    int timer_interval = 25;
    int get_interval = 100;
    bool is_monitoring = false;
    bool first_running_get_device = false;
    char m_padding[6];
    pa_mainloop *mainloop = nullptr;
    pa_mainloop_api *mainloop_api = nullptr;
    pa_context *context = nullptr;
    pa_stream *stream = nullptr;
private:
    static void get_sink_info_callback(pa_context *c, const pa_sink_info *i, int eol, void *userdata);
    static void get_source_info_callback(pa_context *c, const pa_source_info *i, int eol, void *userdata);
    static void get_sink_input_info_callback(pa_context *c, const pa_sink_input_info *i, int eol, void *userdata);
    static void get_source_output_info_callback(pa_context *c, const pa_source_output_info *i, int eol, void *userdata);
    static void stream_read_callback(pa_stream *p, size_t nbytes, void *userdata);
private:
    QTimer *mainloop_timer = new QTimer;
    void mainloop_slot();
signals:
    void device_got_end();
};
class My_Music_Visible : public Basic_Widget
{
    Q_OBJECT
public:
    explicit My_Music_Visible(QWidget *parent);
    ~My_Music_Visible();
    QList<My_Music_Visible *> *my_music_visible_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
private:
    QPoint menu_pos = QPoint(0, 0);
    QMenu *menu = new QMenu(this);
    QAction *set_program_action = new QAction(tr("设置程序接收"), this);
    QAction *start_device_action = new QAction(tr("开始服务"), this);
    QAction *stop_device_action = new QAction(tr("停止服务"), this);
    QMenu *device_menu = new QMenu(this);
    QMenu *Input_List_menu = new QMenu(tr("输入设备"), this);
    QMenu *Output_List_menu = new QMenu(tr("输出设备"), this);
    QMenu *Program_Input_List_menu = new QMenu(tr("录音"), this);
    QMenu *Program_Output_List_menu = new QMenu(tr("回放"), this);
    QList<QAction *> device_action_list;
    Audio_Wave_Monitor *monitor = new Audio_Wave_Monitor(this);
private:
    void contextMenuEvent(QContextMenuEvent *event);
};
#endif // MY_MUSIC_VISIBLE_H
