#ifndef ALL_CONTROL_H
#define ALL_CONTROL_H
#include <QtWidgets>
#include "desktop_background.h"
#include "desktop_main.h"
#include "setting_widget.h"
class All_Control : public QWidget
{
    Q_OBJECT
public:
    explicit All_Control(QWidget *parent = nullptr);
    void Update_Widget();
private:
    int desktop_width;
    int desktop_height;
    Desktop_Background *background = new Desktop_Background(this);
    Desktop_Main *main_desktop = new Desktop_Main(this);
    Setting_Widget *setting_widget = new Setting_Widget;
};

#endif // ALL_CONTROL_H
