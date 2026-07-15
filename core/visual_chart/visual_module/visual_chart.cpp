#include "visual_chart.h"
#include <QtMath>

Visual_Chart::Visual_Chart(QObject *parent)
    : QObject(parent)
{}
Visual_Chart::~Visual_Chart()
{}
void Visual_Chart::addSeries(Visual_QLineSeries *series)
{
    if (!m_series.contains(series))
    {
        m_series.append(series);
        series->setChart(this);
    }
}
void Visual_Chart::removeSeries(Visual_QLineSeries *series)
{
    if (m_series.removeOne(series))
    {
        series->setChart(nullptr);
    }
}
void Visual_Chart::clearSeries()
{
    m_series.clear();
}
void Visual_Chart::addAxis(Visual_ValueAxis *axis, Qt::Alignment alignment)
{
    if (alignment == Qt::AlignBottom || alignment == Qt::AlignTop)
    {
        m_axisX = axis;
    }
    else if (alignment == Qt::AlignLeft || alignment == Qt::AlignRight)
    {
        m_axisY = axis;
    }
}
void Visual_Chart::removeAxis(Visual_ValueAxis *axis)
{
    if (m_axisX == axis)
    {
        m_axisX = nullptr;
    }
    if (m_axisY == axis)
    {
        m_axisY = nullptr;
    }
}
void Visual_Chart::paint(QPainter *painter, const QRect &rect)
{
    if (!painter)
    {
        return;
    }
    painter->setRenderHint(QPainter::Antialiasing);

    painter->fillRect(rect, Qt::transparent);

    if (m_series.isEmpty() || !m_axisX || !m_axisY)
    {
        return;
    }

    QRect chartRect = rect.adjusted(m_margin, m_margin, -m_margin - 120, -m_margin);
    if (chartRect.width() <= 0 || chartRect.height() <= 0)
    {
        return;
    }
    drawAxesAndGrid(painter, chartRect);
    drawSeries(painter, chartRect);

    drawLegend(painter, chartRect);
}
Visual_Chart *Visual_Chart::chart() const
{
    return const_cast<Visual_Chart *>(this);
}
void Visual_Chart::drawLegend(QPainter *painter, const QRect &chartRect)
{
    int legendX = chartRect.right() + 10;
    int legendY = chartRect.top() + 10;
    int itemHeight = 20;
    int itemWidth = 20;
    int spacing = 5;
    int textOffset = 25;

    int maxWidth = 0;
    int totalHeight = 0;
    QFontMetrics fm(painter->font());
    for (Visual_QLineSeries *series : m_series)
    {
        QString name = series->name();
        if (name.isEmpty())
        {
            name = " ";
        }
        int textWidth = fm.horizontalAdvance(name);
        if (textWidth > maxWidth)
        {
            maxWidth = textWidth;
        }
        totalHeight += itemHeight + spacing;
    }
    totalHeight -= spacing;

    int yPos = legendY + 5;
    for (Visual_QLineSeries *series : m_series)
    {
        if (!series->isVisible())
        {
            continue;
        }
        QString name = series->name();
        if (name.isEmpty())
        {
            name = " ";
        }

        painter->setPen(Qt::NoPen);
        painter->setBrush(series->color());
        painter->drawRect(legendX + 5, yPos, itemWidth, itemHeight - 4);

        painter->setPen(Qt::black);
        painter->drawText(legendX + textOffset, yPos, maxWidth, itemHeight - 4, Qt::AlignLeft | Qt::AlignVCenter, name);
        yPos += itemHeight + spacing;
    }
}
void Visual_Chart::drawAxesAndGrid(QPainter *painter, const QRect &chartRect)
{
    double xMin = m_axisX->min(), xMax = m_axisX->max();
    double yMin = m_axisY->min(), yMax = m_axisY->max();
    double xRange = xMax - xMin;
    double yRange = yMax - yMin;
    if (qFuzzyIsNull(xRange) || qFuzzyIsNull(yRange))
    {
        return;
    }
    int left = chartRect.left(), right = chartRect.right();
    int top = chartRect.top(), bottom = chartRect.bottom();
    int width = chartRect.width(), height = chartRect.height();

    auto xToPixel = [&](double x) -> int
    {
        return left + static_cast<int>((x - xMin) / xRange * width);
    };
    auto yToPixel = [&](double y) -> int
    {
        return bottom - static_cast<int>((y - yMin) / yRange * height);
    };

    QPen gridPen(QColor(200, 200, 200, 150));
    gridPen.setStyle(Qt::DashLine);
    painter->setPen(gridPen);
    painter->setFont(m_axisX->labelsFont());

    // Y轴网格线+标签
    int yTickCount = 5;
    for (int i = 0; i <= yTickCount; ++i)
    {
        double y = yMin + i * (yRange / yTickCount);
        int yPos = yToPixel(y);
        painter->drawLine(left, yPos, right, yPos);
        painter->setPen(Qt::black);
        QString label = QString::number(y, 'f', 1);
        painter->drawText(QRect(0, yPos - 10, m_margin - 5, 20), Qt::AlignRight | Qt::AlignVCenter, label);
    }

    // X轴网格线+标签
    int xTickCount = 6;
    for (int i = 0; i <= xTickCount; ++i)
    {
        double x = xMin + i * (xRange / xTickCount);
        int xPos = xToPixel(x);
        painter->drawLine(xPos, top, xPos, bottom);
        painter->setPen(Qt::black);
        QString label = QString::number(x, 'f', 0);
        painter->drawText(QRect(xPos - 20, bottom + 5, 40, 20), Qt::AlignHCenter | Qt::AlignTop, label);
    }

    // 边框
    painter->setPen(QPen(Qt::black, 1));
    painter->drawRect(chartRect);
}
void Visual_Chart::drawSeries(QPainter *painter, const QRect &chartRect)
{
    double xMin = m_axisX->min(), xMax = m_axisX->max();
    double yMin = m_axisY->min(), yMax = m_axisY->max();
    double xRange = xMax - xMin;
    double yRange = yMax - yMin;
    if (qFuzzyIsNull(xRange) || qFuzzyIsNull(yRange))
    {
        return;
    }

    int left = chartRect.left();
    int bottom = chartRect.bottom();
    int width = chartRect.width(), height = chartRect.height();

    auto xToPixel = [&](double x) -> int
    {
        return left + static_cast<int>((x - xMin) / xRange * width);
    };
    auto yToPixel = [&](double y) -> int
    {
        return bottom - static_cast<int>((y - yMin) / yRange * height);
    };

    for (Visual_QLineSeries *series : m_series)
    {
        if (!series->isVisible())
        {
            continue;
        }
        const auto &points = series->points();
        if (points.size() < 2)
        {
            continue;
        }
        QPen pen(series->color(), 2);
        painter->setPen(pen);
        QPointF prev;
        for (int i = 0; i < points.size(); ++i)
        {
            QPointF p = points.at(i);
            QPointF pixelP(xToPixel(p.x()), yToPixel(p.y()));
            if (i > 0)
            {
                painter->drawLine(prev, pixelP);
            }
            prev = pixelP;
        }
    }
}
