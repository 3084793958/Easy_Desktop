#ifndef VISUAL_PULSEAUDIO_CHART_H
#define VISUAL_PULSEAUDIO_CHART_H
#include "visual_basic_chart.h"
#include <pulse/pulseaudio.h>
class Visual_PulseAudio_Chart : public Visual_Basic_Chart
{
    Q_OBJECT
public:
    explicit Visual_PulseAudio_Chart(QWidget *parent = nullptr);
    ~Visual_PulseAudio_Chart();
    QList<Visual_PulseAudio_Chart *> *visual_pulseaudio_chart_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
    virtual void set_icon(QString checked_icon_path);
private:
    QTimer *pa_timer = new QTimer(this);
    Visual_QLineSeries *sec_series = new Visual_QLineSeries(this);
    Visual_QLineSeries *left_min_series = new Visual_QLineSeries(this);
    Visual_QLineSeries *right_min_series = new Visual_QLineSeries(this);
    int update_time = 100;
    bool use_rms = true;
    bool use_peaks = false;
    int vector_long = 120;
    QVector<float> left_data;
    QVector<float> right_data;
    QVector<float> left_min_data;
    QVector<float> right_min_data;
    QColor line1_color = QColor(255, 0, 0, 255);
    QColor line2_color = QColor(0, 0, 255, 255);
    QColor line3_color = QColor(255, 176, 176, 255);
    QColor line4_color = QColor(72, 226, 237, 255);
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    void update_data_size();
    void contextMenuEvent(QContextMenuEvent *event);
    QAction *start_monitor = new Trans_Action(tr("开始侦测"), "开始侦测", this->metaObject()->className(), this);
    QAction *stop_monitor = new Trans_Action(tr("停止侦测"), "停止侦测", this->metaObject()->className(), this);
    QAction *set_update_time = new Trans_Action(tr("设置侦测间隔"), "设置侦测间隔", this->metaObject()->className(), this);
    QAction *set_vector_long = new Trans_Action(tr("设置数据点数量"), "设置数据点数量", this->metaObject()->className(), this);
    QMenu *set_use_rms = new Trans_Menu(tr("设置侦测格式"), "设置侦测格式", this->metaObject()->className(), this);
    QAction *use_rms_action = new Trans_Action(tr("RMS"), "RMS", this->metaObject()->className(), this);
    QAction *use_dB_action = new Trans_Action(tr("dB"), "dB", this->metaObject()->className(), this);
    QAction *use_peaks_action = new Trans_Action(tr("峰值"), "峰值", this->metaObject()->className(), this);
    QMenu *set_monitor_type = new Trans_Menu(tr("设置侦测模式"), "设置侦测模式", this->metaObject()->className(), this);
    QAction *output_action = new Trans_Action(tr("扬声器"), "扬声器", this->metaObject()->className(), this);
    QAction *input_action = new Trans_Action(tr("麦克风"), "麦克风", this->metaObject()->className(), this);
    QAction *set_max_can_process = new Trans_Action(tr("单次数据点数"), "单次数据点数", this->metaObject()->className(), this);
    QAction *set_text_font = new Trans_Action(tr("设置字体"), "设置字体", this->metaObject()->className(), this);
    QAction *set_line_color = new Trans_Action(tr("设置折线颜色"), "设置折线颜色", this->metaObject()->className(), this);
    QMenu *menu = new QMenu(this);
private:
    enum class Stream_Type
    {
        Input,
        Output
    };
    bool get_input = false;
    bool connecting = true;
    pa_mainloop *mainloop = nullptr;
    pa_mainloop_api *mainloop_api = nullptr;
    pa_context *context = nullptr;
    pa_stream *main_stream = nullptr;
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
    void pa_start_main_finished(Stream_Type type);
    static void stream_main_read_callback(pa_stream *p, size_t nbytes, void *userdata);
};

#endif // VISUAL_PULSEAUDIO_CHART_H
