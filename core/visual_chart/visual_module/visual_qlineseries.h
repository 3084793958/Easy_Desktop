#ifndef VISUAL_QLINESERIES_H
#define VISUAL_QLINESERIES_H

#include <QObject>
#include <QVector>
#include <QPointF>
#include <QColor>
#include <QString>

class Visual_Chart;
class Visual_QLineSeries : public QObject
{
    Q_OBJECT
public:
    explicit Visual_QLineSeries(QObject *parent = nullptr);

    void append(const QPointF &point);
    void append(double x, double y);
    void clear();
    void setColor(const QColor &color);
    QColor color() const;
    void setName(const QString &name);
    QString name() const;

    bool isVisible() const;
    void setVisible(bool visible);
    void show();
    void hide();

    Visual_Chart *chart() const;
    void setChart(Visual_Chart *chart);

    const QVector<QPointF> &points() const;
    int count() const;

    void attachAxis(QObject *axis);

private:
    QVector<QPointF> m_points;
    QColor m_color = Qt::blue;
    QString m_name;
    bool m_visible = true;
    Visual_Chart *m_chart = nullptr;
};

#endif // VISUAL_QLINESERIES_H
