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
void Basic_Chart::update_data()
{
    if (m_data.size() == 0)
    {
        return;
    }
    series->clear();
    axisY->setRange(Y_min , Y_max);
    axisX->setRange(0, m_data.size());
    for (int i = 0; i < m_data.size(); i++)
    {
        series->append(i, m_data.at(i));
    }
}
