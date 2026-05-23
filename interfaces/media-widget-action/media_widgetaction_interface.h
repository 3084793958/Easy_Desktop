#ifndef MEDIA_WIDGETACTION_INTERFACE_H
#define MEDIA_WIDGETACTION_INTERFACE_H

class QString;
class QColor;

class Media_WidgetAction_Interface
{
public:
    virtual ~Media_WidgetAction_Interface() {}
    virtual void set_value(int m_volume, int m_speed) = 0;
    virtual void set_second(int value, QString text) = 0;
    virtual void slider_set_volume(int value) = 0;
    virtual void slider_set_position(int value) = 0;
    virtual void slider_set_speed(int value) = 0;
    virtual void set_color(QColor color) = 0;
};

#endif // MEDIA_WIDGETACTION_INTERFACE_H
