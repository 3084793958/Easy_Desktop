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
    QTimer *updateTimer = new QTimer(this);
    int update_time = 1000;
    int channel = 0;
    int vector_long = 60;
    QColor line_color = QColor(0, 166, 255, 255);

    My_QLineSeries *sec_series = new My_QLineSeries(this);
    QColor sec_line_color = QColor(198, 255, 230, 255);

    QFont font = QFontDatabase::systemFont(QFontDatabase::FixedFont);
    QAction *start_monitor = new Trans_Action(tr("开始侦测"), "开始侦测", this->metaObject()->className(), this);
    QAction *stop_monitor = new Trans_Action(tr("停止侦测"), "停止侦测", this->metaObject()->className(), this);
    QAction *set_update_time = new Trans_Action(tr("设置侦测间隔"), "设置侦测间隔", this->metaObject()->className(), this);
    QAction *set_vector_long = new Trans_Action(tr("设置数据点数量"), "设置数据点数量", this->metaObject()->className(), this);
    QMenu *set_channel = new Trans_Menu(tr("设置侦测频道"), "设置侦测频道", this->metaObject()->className(), this);
    QAction *ram_channel = new Trans_Action(tr("内存"), "内存", this->metaObject()->className(), this);
    QAction *swap_channel = new Trans_Action(tr("交换空间"), "交换空间", this->metaObject()->className(), this);
    QAction *all_channel = new Trans_Action(tr("我全都要"), "我全都要", this->metaObject()->className(), this);
    QAction *set_text_font = new Trans_Action(tr("设置字体"), "设置字体", this->metaObject()->className(), this);
    QAction *set_line_color = new Trans_Action(tr("设置折线颜色"), "设置折线颜色", this->metaObject()->className(), this);
    QMenu *menu = new QMenu(this);
    void contextMenuEvent(QContextMenuEvent *event);
    void timeout_slot();
};

#endif // RAM_CHART_H
