#include "visual_valueaxis.h"

Visual_ValueAxis::Visual_ValueAxis(QObject *parent)
    : QObject(parent)
{}
void Visual_ValueAxis::setRange(double min, double max)
{
    m_min = min;
    m_max = max;
}
double Visual_ValueAxis::min() const
{
    return m_min;
}
double Visual_ValueAxis::max() const
{
    return m_max;
}
void Visual_ValueAxis::setLabelsFont(const QFont &font)
{
    m_labelsFont = font;
}
QFont Visual_ValueAxis::labelsFont() const
{
    return m_labelsFont;
}
void Visual_ValueAxis::setTitleText(const QString &title)
{
    m_title = title;
}
QString Visual_ValueAxis::titleText() const
{
    return m_title;
}
