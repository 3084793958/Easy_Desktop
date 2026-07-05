#ifndef MEDIA_WIDGETACTION_H
#define MEDIA_WIDGETACTION_H
#include <QtWidgets>
#include "interfaces/media-widget-action/media_widgetaction_interface.h"
#include "core/tools/trans_action.h"
class Media_WidgetAction : public QWidgetAction, public Media_WidgetAction_Interface
{
    Q_OBJECT
public:
    explicit Media_WidgetAction(QWidget *parent);
    virtual void set_value(int m_volume, int m_speed) override;
    virtual void set_second(int value, QString text) override;
    void save(QSettings *settings);
    void load(QSettings *settings);
    virtual void slider_set_volume(int value) override;
    virtual void slider_set_position(int value) override;
    virtual void slider_set_speed(int value) override;
    virtual void set_color(QColor color) override;
signals:
    void change_signals_V(int m_volume);
    void change_signals_P(int m_Position);
    void change_signals_S(int m_speed);
protected:
    virtual QWidget* createWidget(QWidget *parent) override;
private:
    bool updating = false;
    bool position_changing = false;
    QWidget *seat_widget = new QWidget;
    QHBoxLayout *sliderbox = new QHBoxLayout();
    QLabel *setvolume = new Trans_Label(tr("设置音量\n100%"), "设置音量\n100%", this->metaObject()->className(), seat_widget);
    QSlider *set_volume_main = new QSlider(Qt::Horizontal,seat_widget);
    QLabel *setmusic_time=new Trans_Label(tr("进度条\n00:00"), "进度条\n00:00", this->metaObject()->className(), seat_widget);
    QSlider *set_music_time_main=new QSlider(Qt::Horizontal,seat_widget);
    QLabel *setmusic_speed=new Trans_Label(tr("速度\n100%"), "速度\n100%", this->metaObject()->className(), seat_widget);
    QSlider *set_music_speed_main=new QSlider(Qt::Horizontal,seat_widget);
private slots:
    void qslider_doing(int value);
    void music_value_speed(int value);
};

#endif // MEDIA_WIDGETACTION_H
