#include "visual_qlineseries.h"

Visual_QLineSeries::Visual_QLineSeries(QObject *parent)
    : QObject(parent)
{}
void Visual_QLineSeries::append(const QPointF &point)
{
    m_points.append(point);
}
void Visual_QLineSeries::append(double x, double y)
{
    m_points.append(QPointF(x, y));
}
void Visual_QLineSeries::clear()
{
    m_points.clear();
}
void Visual_QLineSeries::setColor(const QColor &color)
{
    m_color = color;
}
QColor Visual_QLineSeries::color() const
{
    return m_color;
}
void Visual_QLineSeries::setName(const QString &name)
{
    m_name = name;
}
QString Visual_QLineSeries::name() const
{
    return m_name;
}
bool Visual_QLineSeries::isVisible() const
{
    return m_visible;
}
void Visual_QLineSeries::setVisible(bool visible)
{
    m_visible = visible;
}
void Visual_QLineSeries::show()
{
    setVisible(true);
}
void Visual_QLineSeries::hide()
{
    setVisible(false);
}
Visual_Chart *Visual_QLineSeries::chart() const
{
    return m_chart;
}
void Visual_QLineSeries::setChart(Visual_Chart *chart)
{
    m_chart = chart;
}
const QVector<QPointF> &Visual_QLineSeries::points() const
{
    return m_points;
}
int Visual_QLineSeries::count() const
{
    return m_points.size();
}
void Visual_QLineSeries::attachAxis(QObject *axis)
{
    (void)axis;
}
