#include "my_x11_libs.h"
Window My_X11_Libs::WinId = 0;
void My_X11_Libs::X11_Raise()
{
    Display *display = QX11Info::display();
    XRaiseWindow(display, WinId);
    XFlush(display);
}
#undef CursorShape
