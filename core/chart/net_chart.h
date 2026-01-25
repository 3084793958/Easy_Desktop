#ifndef NET_CHART_H
#define NET_CHART_H
#include "basic_chart.h"

class NET_Chart : public Basic_Chart
{
    Q_OBJECT
public:
    explicit NET_Chart(QWidget *parent = nullptr);
    ~NET_Chart();
    QList<NET_Chart *> *net_chart_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
private:
    QLineSeries *sec_series = new QLineSeries;
    void get_net_data();
    unsigned long long last_Tra = 0, last_Rec = 0;
    unsigned long long new_Tra = 0, new_Rec = 0;
    QVector<double> Tra_data_vec;
    QVector<double> Rec_data_vec;
    QString Tra_str = "";
    QString Rec_str = "";
    QTimer *updateTimer = new QTimer;
    int update_time = 1000;
    int vector_long = 60;
    int axis_type = 0;
    QColor line1_color = QColor(254, 170, 142, 255);
    QColor line2_color = QColor(198, 255, 230, 255);
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QAction *start_monitor = new QAction(tr("开始侦测"), this);
    QAction *stop_monitor = new QAction(tr("停止侦测"), this);
    QAction *set_update_time = new QAction(tr("设置侦测间隔"), this);
    QAction *set_vector_long = new QAction(tr("设置数据点数量"), this);
    QMenu *set_axis = new QMenu(tr("设置轴"), this);
    QAction *same_axis = new QAction(tr("等差轴"), this);
    QAction *unsame_axis = new QAction(tr("非等差轴"), this);
    QAction *set_text_font = new QAction(tr("设置字体"), this);
    QAction *set_line_color = new QAction(tr("设置折线颜色"), this);
    QMenu *menu = new QMenu(this);
    void contextMenuEvent(QContextMenuEvent *event);
    void timeout_slot();
};

#endif // NET_CHART_H
