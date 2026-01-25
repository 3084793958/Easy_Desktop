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
    QList<QPair<unsigned long long, unsigned long long>> last_data;
    QList<QPair<unsigned long long, unsigned long long>> new_data;
    QTimer *updateTimer = new QTimer;
    int update_time = 1000;
    int channel = 0;
    int vector_long = 60;
    QColor line_color = QColor(255, 170, 0, 255);
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QAction *start_monitor = new QAction(tr("开始侦测"), this);
    QAction *stop_monitor = new QAction(tr("停止侦测"), this);
    QAction *set_update_time = new QAction(tr("设置侦测间隔"), this);
    QAction *set_vector_long = new QAction(tr("设置数据点数量"), this);
    QAction *set_channel = new QAction(tr("设置侦测频道"), this);
    QAction *set_text_font = new QAction(tr("设置字体"), this);
    QAction *set_line_color = new QAction(tr("设置折线颜色"), this);
    QMenu *menu = new QMenu(this);
    void contextMenuEvent(QContextMenuEvent *event);
    void timeout_slot();
};

#endif // CPU_CHART_H
