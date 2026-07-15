#ifndef CPU_CHART_H
#define CPU_CHART_H
#include "basic_chart.h"
class CPU_Chart : public Basic_Chart
{
    Q_OBJECT
public:
    explicit CPU_Chart(QWidget *parent = nullptr);
    ~CPU_Chart();
    QList<CPU_Chart *> *cpu_chart_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
private:
    void get_cpu_data();
    QVector<double> each_cpu_data;
    QList<QVector<double> *> send_cpu_data_list;
    QVector<My_QLineSeries *> series_list;
    QList<QPair<unsigned long long, unsigned long long>> last_data;
    QList<QPair<unsigned long long, unsigned long long>> new_data;
    QTimer *updateTimer = new QTimer(this);
    int update_time = 1000;
    int channel = 0;
    int vector_long = 60;
    QColor line_color = QColor(255, 170, 0, 255);
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QAction *start_monitor = new Trans_Action(tr("开始侦测"), "开始侦测", this->metaObject()->className(), this);
    QAction *stop_monitor = new Trans_Action(tr("停止侦测"), "停止侦测", this->metaObject()->className(), this);
    QAction *set_update_time = new Trans_Action(tr("设置侦测间隔"), "设置侦测间隔", this->metaObject()->className(), this);
    QAction *set_vector_long = new Trans_Action(tr("设置数据点数量"), "设置数据点数量", this->metaObject()->className(), this);
    QAction *set_channel = new Trans_Action(tr("设置侦测频道"), "设置侦测频道", this->metaObject()->className(), this);
    QAction *set_text_font = new Trans_Action(tr("设置字体"), "设置字体", this->metaObject()->className(), this);
    QAction *set_line_color = new Trans_Action(tr("设置折线颜色"), "设置折线颜色", this->metaObject()->className(), this);
    QMenu *menu = new QMenu(this);
    void contextMenuEvent(QContextMenuEvent *event);
    void timeout_slot();
};

#endif // CPU_CHART_H
