#include "visual_chartview.h"

Visual_ChartView::Visual_ChartView(QWidget *parent)
    : QWidget(parent)
{
    setAttribute(Qt::WA_TranslucentBackground);
    setMouseTracking(false);
}
Visual_ChartView::~Visual_ChartView()
{}
void Visual_ChartView::setChart(Visual_Chart *chart)
{
    m_chart = chart;
    update();
}
void Visual_ChartView::setRenderHint(QPainter::RenderHint hint, bool enabled)
{
    if (enabled)
    {
        m_renderHints |= hint;
    }
    else
    {
        m_renderHints &= ~hint;
    }

}

Visual_Chart *Visual_ChartView::chart() const
{
    return m_chart;
}
void Visual_ChartView::paintEvent(QPaintEvent *event)
{
    (void)event;
    QPainter painter(this);
    painter.setRenderHints(m_renderHints);
    if (m_chart)
    {
        m_chart->paint(&painter, rect());
    }
    else
    {
        painter.fillRect(rect(), Qt::transparent);
    }
}
