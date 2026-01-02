#ifndef RAM_CHART_H
#define RAM_CHART_H
#include "basic_chart.h"

class RAM_Chart : public Basic_Chart
{
    Q_OBJECT
public:
    explicit RAM_Chart(QWidget *parent = nullptr);
    ~RAM_Chart();
    QList<RAM_Chart *> *ram_chart_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
private:
    void get_ram_data();
    double ram_data = 0.0;
    double swap_data = 0.0;
    QString ram_data_str = "";
    QString swap_data_str = "";
    QVector<double> ram_data_vec;
    QVector<double> swap_data_vec;
    QTimer *updateTimer = new QTimer;
    int update_time = 1000;
    int channel = 0;
    int vector_long = 60;
    QColor line_color = QColor(0, 166, 255, 255);
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QAction *start_monitor = new QAction(tr("开始侦测"), this);
    QAction *stop_monitor = new QAction(tr("停止侦测"), this);
    QAction *set_update_time = new QAction(tr("设置侦测间隔"), this);
    QAction *set_vector_long = new QAction(tr("设置数据点数量"), this);
    QMenu *set_channel = new QMenu(tr("设置侦测频道"), this);
    QAction *ram_channel = new QAction(tr("内存"), this);
    QAction *swap_channel = new QAction(tr("交换空间"), this);
    QAction *set_text_font = new QAction(tr("设置字体"), this);
    QAction *set_line_color = new QAction(tr("设置折线颜色"), this);
    QMenu *menu = new QMenu(this);
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // RAM_CHART_H
