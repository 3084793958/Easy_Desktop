#ifndef VISUAL_CHART_H
#define VISUAL_CHART_H

#include <QObject>
#include <QList>
#include <QRect>
#include <QColor>
#include <QPainter>
#include "visual_qlineseries.h"
#include "visual_valueaxis.h"

class Visual_Chart : public QObject
{
    Q_OBJECT
public:
    explicit Visual_Chart(QObject *parent = nullptr);
    ~Visual_Chart();

    void addSeries(Visual_QLineSeries *series);
    void removeSeries(Visual_QLineSeries *series);
    void clearSeries();

    void addAxis(Visual_ValueAxis *axis, Qt::Alignment alignment);
    void removeAxis(Visual_ValueAxis *axis);

    void paint(QPainter *painter, const QRect &rect);

    Visual_Chart *chart() const;
private:
    QList<Visual_QLineSeries *> m_series;
    Visual_ValueAxis *m_axisX = nullptr;//这里有人(Basic_Chart)会为其赋值
    Visual_ValueAxis *m_axisY = nullptr;
    int m_margin = 50;

    void drawAxesAndGrid(QPainter *painter, const QRect &chartRect);
    void drawSeries(QPainter *painter, const QRect &chartRect);
    void drawLegend(QPainter *painter, const QRect &chartRect);
};

#endif // VISUAL_CHART_H
