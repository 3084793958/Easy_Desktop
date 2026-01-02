#ifndef PULSEAUDIO_CHART_H
#define PULSEAUDIO_CHART_H
#include "basic_chart.h"
#include <pulse/pulseaudio.h>
struct AudioLevel
{
    double left_level;
    double right_level;
    double peak_level;
    double rms_level;
};
class PulseAudio_Chart : public Basic_Chart
{
    Q_OBJECT
public:
    explicit PulseAudio_Chart(QWidget *parent = nullptr);
    ~PulseAudio_Chart();
    QList<PulseAudio_Chart *> *pulseaudio_chart_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
private:
    QTimer *pa_timer = new QTimer;
    QLineSeries *sec_series = new QLineSeries;
    int update_time = 100;
    bool use_rms = true;
    int vector_long = 120;
    QVector<float> left_data;
    QVector<float> right_data;
    QColor line1_color = QColor(255, 0, 0, 255);
    QColor line2_color = QColor(0, 0, 255, 255);
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    void update_data_size();
    void contextMenuEvent(QContextMenuEvent *event);
    QAction *start_monitor = new QAction(tr("开始侦测"), this);
    QAction *stop_monitor = new QAction(tr("停止侦测"), this);
    QAction *set_update_time = new QAction(tr("设置侦测间隔"), this);
    QAction *set_vector_long = new QAction(tr("设置数据点数量"), this);
    QMenu *set_use_rms = new QMenu(tr("设置侦测格式"), this);
    QAction *use_rms_action = new QAction(tr("RMS"), this);
    QAction *use_dB_action = new QAction(tr("dB"), this);
    QMenu *set_monitor_type = new QMenu(tr("设置侦测模式"), this);
    QAction *output_action = new QAction(tr("扬声器"), this);
    QAction *input_action = new QAction(tr("麦克风"), this);
    QAction *set_max_can_process = new QAction(tr("单次数据点数"), this);
    QAction *set_text_font = new QAction(tr("设置字体"), this);
    QAction *set_line_color = new QAction(tr("设置折线颜色"), this);
    QMenu *menu = new QMenu(this);
private:
    bool get_input = false;
    bool connecting = true;
    pa_mainloop *mainloop = nullptr;
    pa_mainloop_api *mainloop_api = nullptr;
    pa_context *context = nullptr;
    pa_stream *input_stream = nullptr;
    pa_stream *output_stream = nullptr;
    pa_sample_spec input_spec;
    pa_sample_spec output_spec;
    char *input_name;
    char *output_name;
    int pa_can_process = 2;
    int pa_now_process = 0;
    void pa_init();
    void pa_delete();
    void pa_update();
    void pa_start_monitor();
    void pa_start_input_finished();
    void pa_start_output_finished();
    static void stream_input_read_callback(pa_stream *p, size_t nbytes, void *userdata);
    static void stream_output_read_callback(pa_stream *p, size_t nbytes, void *userdata);
};

#endif // PULSEAUDIO_CHART_H
