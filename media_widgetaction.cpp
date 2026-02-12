#include "media_widgetaction.h"
void Media_WidgetAction::set_color(QColor color)
{
    seat_widget->setStyleSheet(QString("color:rgba(%1,%2,%3,%4)").arg(color.red()).arg(color.green()).arg(color.blue()).arg(color.alpha()));
}
Media_WidgetAction::Media_WidgetAction(QWidget *parent)
    :QWidgetAction(parent)
{
    seat_widget->setStyleSheet("color:rgba(0,0,0,225)");
    sliderbox->addWidget(seat_widget);
    seat_widget->setFixedSize(200 ,120);
    setvolume->move(0, 0);
    set_volume_main->move(50, 0);
    set_volume_main->resize(150,40);
    set_volume_main->setMinimum(0);
    set_volume_main->setMaximum(100);
    set_volume_main->setSingleStep(10);
    set_volume_main->setValue(100);
    set_volume_main->setTickPosition(QSlider::TicksBelow);
    connect(set_volume_main, SIGNAL(valueChanged(int)), this, SLOT(qslider_doing(int)));
    set_volume_main->setTickInterval(10);
    setmusic_time->move(0,40);
    set_music_time_main->move(50,40);
    set_music_time_main->resize(150,40);
    set_music_time_main->setMinimum(0);
    set_music_time_main->setMaximum(100);
    set_music_time_main->setSingleStep(10);
    set_music_time_main->setValue(0);
    set_music_time_main->setTickPosition(QSlider::TicksBelow);
    set_music_time_main->setTickInterval(10);
    connect(set_music_time_main, &QSlider::sliderPressed, this, [=]{position_changing = true;});
    connect(set_music_time_main, &QSlider::sliderReleased, this, [=]
    {
        position_changing = false;
        if (!updating)
        {
            emit change_signals_P(set_music_time_main->value());
        }
    });
    setmusic_speed->move(0,80);
    set_music_speed_main->move(50,80);
    set_music_speed_main->resize(150,40);
    set_music_speed_main->setMinimum(10);
    set_music_speed_main->setMaximum(300);
    set_music_speed_main->setSingleStep(10);
    set_music_speed_main->setValue(100);
    set_music_speed_main->setTickPosition(QSlider::TicksBelow);
    set_music_speed_main->setTickInterval(10);
    connect(set_music_speed_main, SIGNAL(valueChanged(int)), this, SLOT(music_value_speed(int)));
}
QWidget* Media_WidgetAction::createWidget(QWidget *parent)
{
    QWidget *resultWidget = new QWidget(parent);
    resultWidget->setLayout(sliderbox);
    return resultWidget;
}
void Media_WidgetAction::qslider_doing(int value)
{
    if (updating)
    {
        return;
    }
    setvolume->setText(QString("设置音量\n%1%").arg(value));
    emit change_signals_V(value);
}
void Media_WidgetAction::music_value_speed(int value)
{
    if (updating)
    {
        return;
    }
    setmusic_speed->setText(QString("速度\n%1%").arg(value));
    emit change_signals_S(value);
}
void Media_WidgetAction::set_value(int m_volume, int m_speed)
{
    updating = true;
    setvolume->setText(QString("设置音量\n%1%").arg(m_volume));
    set_volume_main->setValue(m_volume);
    setmusic_speed->setText(QString("速度\n%1%").arg(m_speed));
    set_music_speed_main->setValue(m_speed);
    updating = false;
}
void Media_WidgetAction::set_second(int value, QString text)
{
    setmusic_time->setText(QString("进度条\n%1").arg(text));
    if (!position_changing)
    {
        set_music_time_main->setValue(value);
    }
}
void Media_WidgetAction::save(QSettings *settings)
{
    settings->beginGroup("Media_Slider");
    settings->setValue("Volume", set_volume_main->value());
    settings->setValue("music_time", set_music_time_main->value());
    settings->setValue("music_speed", set_music_speed_main->value());
    settings->endGroup();
}
void Media_WidgetAction::load(QSettings *settings)
{
    settings->beginGroup("Media_Slider");
    int volume_value = settings->value("Volume", 100).toInt();
    int music_time_value = settings->value("music_time", 0).toInt();
    int music_speed_value = settings->value("music_speed", 100).toInt();
    qslider_doing(volume_value);
    music_value_speed(music_speed_value);
    set_second(music_time_value, QString("%1%").arg(music_time_value));
    settings->endGroup();
    set_volume_main->setValue(volume_value);
    set_music_time_main->setValue(music_time_value);
    set_music_speed_main->setValue(music_speed_value);
    emit change_signals_V(volume_value);
    emit change_signals_P(set_music_time_main->value());
    emit change_signals_S(music_speed_value);
}
void Media_WidgetAction::slider_set_volume(int value)
{
    set_volume_main->setValue(value);
    emit change_signals_V(set_volume_main->value());
    qslider_doing(value);
}
void Media_WidgetAction::slider_set_position(int value)
{
    set_music_time_main->setValue(value);
    emit change_signals_P(set_music_time_main->value());
    set_second(set_music_time_main->value(), QString("%1%").arg(set_music_time_main->value()));
}
void Media_WidgetAction::slider_set_speed(int value)
{
    set_music_speed_main->setValue(value);
    emit change_signals_S(set_music_speed_main->value());
    music_value_speed(value);
}
