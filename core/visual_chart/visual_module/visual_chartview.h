#ifndef VISUAL_CHARTVIEW_H
#define VISUAL_CHARTVIEW_H

#include <QWidget>
#include <QPainter>
#include "visual_chart.h"

class Visual_ChartView : public QWidget
{
    Q_OBJECT
public:
    explicit Visual_ChartView(QWidget *parent = nullptr);
    ~Visual_ChartView() override;

    void setChart(Visual_Chart *chart);
    void setRenderHint(QPainter::RenderHint hint, bool enabled = true);

    Visual_Chart *chart() const;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    Visual_Chart *m_chart = nullptr;
    QPainter::RenderHints m_renderHints;
};

#endif // VISUAL_CHARTVIEW_H
