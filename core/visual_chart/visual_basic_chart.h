#ifndef PAINT_BASIC_CHART_H
#define PAINT_BASIC_CHART_H

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

#include "visual_module/visual_chartview.h"
#include "visual_module/visual_chart.h"
#include "visual_module/visual_qlineseries.h"
#include "visual_module/visual_valueaxis.h"

#ifdef QT_HAD_MIN
#pragma pop_macro("min")
#undef QT_HAD_MIN
#endif
#ifdef QT_HAD_MAX
#pragma pop_macro("max")
#undef QT_HAD_MAX
#endif
//礼尚往来

#include "core/basic_widget.h"

#include <QVector>

class Visual_Basic_Chart : public Basic_Widget
{
    Q_OBJECT
public:
    explicit Visual_Basic_Chart(QWidget *parent = nullptr);
    virtual ~Visual_Basic_Chart() override;

protected:
    Visual_ChartView *chartView = new Visual_ChartView(this->get_self());
    Visual_Chart *chart = new Visual_Chart();
    Visual_QLineSeries *series = new Visual_QLineSeries(this);
    Visual_ValueAxis *axisX = new Visual_ValueAxis(this);
    Visual_ValueAxis *axisY = new Visual_ValueAxis(this);
    QVector<double> m_data;
    double Y_max = 100;
    double Y_min = 0;
    void update_data();
};

#endif // VISUAL_BASIC_CHART_H
