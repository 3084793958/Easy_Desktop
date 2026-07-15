#include "basic_chart.h"

Basic_Chart::Basic_Chart(QWidget *parent)
    :Basic_Widget(parent)
{
    chartView->setChart(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chart->addSeries(series);
    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisX);
    series->attachAxis(axisY);
    chartView->setMouseTracking(true);
    chartView->installEventFilter(this);
    chartView->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    chartView->setInteractive(false);
    chartView->setRubberBand(QChartView::NoRubberBand);
    chartView->setStyleSheet("background:rgba(0,0,0,0)");
    chart->setBackgroundVisible(false);
    connect(this, &Basic_Chart::size_changed, this, [=](QSize size)
    {
        chartView->move(0, 0);
        chartView->resize(size);
    });
    resize(300, 300);
    show();
}
Basic_Chart::~Basic_Chart()
{
    chart->deleteLater();
    disconnect();
}
void Basic_Chart::update_data()
{
    if (m_data.size() == 0)
    {
        return;
    }
    series->clear();
    if (!qFuzzyCompare(axisY->max(), Y_max) || !qFuzzyCompare(axisY->min(), Y_min))
    {
        axisY->setRange(Y_min , Y_max);
    }
    if (!qFuzzyCompare(axisX->max(), m_data.size()) || !qFuzzyCompare(axisX->min(), 0))
    {
        axisX->setRange(0, m_data.size());
    }
    for (int i = 0; i < m_data.size(); i++)
    {
        series->append(i, m_data.at(i));
    }
}
My_QLineSeries::My_QLineSeries(QWidget *parent)
    :QLineSeries(parent)
{
    QPen p = this->pen();
    p.setWidth(2);
    this->setPen(p);
}
