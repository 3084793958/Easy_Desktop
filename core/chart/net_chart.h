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
    virtual void set_icon(QString checked_icon_path);
private:
    My_QLineSeries *sec_series = new My_QLineSeries(this);
    void get_net_data();
    unsigned long long last_Tra = 0, last_Rec = 0;
    unsigned long long new_Tra = 0, new_Rec = 0;
    QVector<double> Tra_data_vec;
    QVector<double> Rec_data_vec;
    QString Tra_str = "";
    QString Rec_str = "";
    QTimer *updateTimer = new QTimer(this);
    int update_time = 1000;
    int vector_long = 60;
    int axis_type = 0;
    QColor line1_color = QColor(254, 170, 142, 255);
    QColor line2_color = QColor(198, 255, 230, 255);
    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QAction *start_monitor = new Trans_Action(tr("开始侦测"), "开始侦测", this->metaObject()->className(), this);
    QAction *stop_monitor = new Trans_Action(tr("停止侦测"), "停止侦测", this->metaObject()->className(), this);
    QAction *set_update_time = new Trans_Action(tr("设置侦测间隔"), "设置侦测间隔", this->metaObject()->className(), this);
    QAction *set_vector_long = new Trans_Action(tr("设置数据点数量"), "设置数据点数量", this->metaObject()->className(), this);
    QMenu *set_axis = new Trans_Menu(tr("设置轴"), "设置轴", this->metaObject()->className(), this);
    QAction *same_axis = new Trans_Action(tr("等差轴"), "等差轴", this->metaObject()->className(), this);
    QAction *unsame_axis = new Trans_Action(tr("非等差轴"), "非等差轴", this->metaObject()->className(), this);
    QAction *set_text_font = new Trans_Action(tr("设置字体"), "设置字体", this->metaObject()->className(), this);
    QAction *set_line_color = new Trans_Action(tr("设置折线颜色"), "设置折线颜色", this->metaObject()->className(), this);
    QMenu *menu = new QMenu(this);
    void contextMenuEvent(QContextMenuEvent *event);
    void timeout_slot();
};

#endif // NET_CHART_H
