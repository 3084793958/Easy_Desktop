#ifndef BASIC_CHART_H
#define BASIC_CHART_H
//mingGW和g++似乎不太一样 max的宏定义在<stdlib.h>(mingGW),g++中没有
#ifdef min
#define QT_HAD_MIN
#pragma push_macro("min")
#undef min
#endif
#ifdef max
#define QT_HAD_MAX
#pragma push_macro("max")
#undef max
#endif
#include <QtCharts>
#ifdef QT_HAD_MIN
#pragma pop_macro("min")
#undef QT_HAD_MIN
#endif
#ifdef QT_HAD_MAX
#pragma pop_macro("max")
#undef QT_HAD_MAX
#endif
#include <core/basic_widget.h>
class Basic_Chart : public Basic_Widget
{
    Q_OBJECT
public:
    explicit Basic_Chart(QWidget *parent = nullptr);
protected:
    QChartView *chartView = new QChartView(this->get_self());
    QChart *chart = new QChart();
    QLineSeries *series = new QLineSeries();
    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY = new QValueAxis;
    QVector<double> m_data;
    double Y_max = 100;
    double Y_min = 0;
    void update_data();
};

#endif // BASIC_CHART_H
