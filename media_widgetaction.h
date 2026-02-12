#ifndef MEDIA_WIDGETACTION_H
#define MEDIA_WIDGETACTION_H
#include <QtWidgets>

class Media_WidgetAction : public QWidgetAction
{
    Q_OBJECT
public:
    explicit Media_WidgetAction(QWidget *parent);
    void set_value(int m_volume, int m_speed);
    void set_second(int value, QString text);
    void save(QSettings *settings);
    void load(QSettings *settings);
    void slider_set_volume(int value);
    void slider_set_position(int value);
    void slider_set_speed(int value);
    void set_color(QColor color);
signals:
    void change_signals_V(int m_volume);
    void change_signals_P(int m_Position);
    void change_signals_S(int m_speed);
protected:
    virtual QWidget* createWidget(QWidget *parent);
private:
    bool updating = false;
    bool position_changing = false;
    QWidget *seat_widget = new QWidget;
    QHBoxLayout *sliderbox = new QHBoxLayout();
    QLabel *setvolume = new QLabel(tr("设置音量\n100%"),seat_widget);
    QSlider *set_volume_main = new QSlider(Qt::Horizontal,seat_widget);
    QLabel *setmusic_time=new QLabel(tr("进度条\n00:00"),seat_widget);
    QSlider *set_music_time_main=new QSlider(Qt::Horizontal,seat_widget);
    QLabel *setmusic_speed=new QLabel(tr("速度\n100%"),seat_widget);
    QSlider *set_music_speed_main=new QSlider(Qt::Horizontal,seat_widget);
private slots:
    void qslider_doing(int value);
    void music_value_speed(int value);
};

#endif // MEDIA_WIDGETACTION_H
