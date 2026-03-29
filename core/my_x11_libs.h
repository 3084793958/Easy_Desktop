#ifndef MY_X11_LIBS_H
#define MY_X11_LIBS_H
#include <QX11Info>
#include <X11/Xatom.h>
#include <X11/Xlib.h>

#ifdef Bool
#undef Bool
#endif

struct My_X11_Libs
{
    static Window WinId;
    static void X11_Raise();
};
#undef CursorShape
#endif // MY_X11_LIBS_H
