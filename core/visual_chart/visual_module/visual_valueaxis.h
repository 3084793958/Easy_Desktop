#ifndef VISUAL_VALUEAXIS_H
#define VISUAL_VALUEAXIS_H

#include <QObject>
#include <QFont>
#include <QString>

class Visual_ValueAxis : public QObject
{
    Q_OBJECT
public:
    explicit Visual_ValueAxis(QObject *parent = nullptr);

    void setRange(double min, double max);
    double min() const;
    double max() const;

    void setLabelsFont(const QFont &font);
    QFont labelsFont() const;

    void setTitleText(const QString &title);
    QString titleText() const;

private:
    double m_min = 0.0;
    double m_max = 1.0;
    QFont m_labelsFont;
    QString m_title;
};

#endif // VISUAL_VALUEAXIS_H
