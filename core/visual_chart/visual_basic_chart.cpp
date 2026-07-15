#include "visual_basic_chart.h"

Visual_Basic_Chart::Visual_Basic_Chart(QWidget *parent)
    : Basic_Widget(parent)
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

    connect(this, &Visual_Basic_Chart::size_changed, this, [=](QSize size)
    {
        chartView->move(0, 0);
        chartView->resize(size);
    });

    resize(400, 300);
    show();
}
Visual_Basic_Chart::~Visual_Basic_Chart()
{
    chart->deleteLater();
    disconnect();
}
void Visual_Basic_Chart::update_data()
{
    if (m_data.size() == 0)
    {
        return;
    }
    series->clear();

    if (!qFuzzyCompare(axisY->max(), Y_max) || !qFuzzyCompare(axisY->min(), Y_min))
    {
        axisY->setRange(Y_min, Y_max);
    }

    if (!qFuzzyCompare(axisX->max(), static_cast<double>(m_data.size())) || !qFuzzyCompare(axisX->min(), 0.0))
    {
        axisX->setRange(0, m_data.size());
    }

    for (int i = 0; i < m_data.size(); ++i)
    {
        series->append(i, m_data.at(i));
    }

    chartView->update();
}
