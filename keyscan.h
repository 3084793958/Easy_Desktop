#ifndef KEYSCAN_H
#define KEYSCAN_H
#include <QtWidgets>
#include <iostream>
#include <fstream>
#include <X11/Xlib.h>
#include <X11/Xlibint.h>
#include <X11/extensions/record.h>
class keyScan : public QThread
{
    Q_OBJECT
public:
    bool doing = true;
    explicit keyScan(QObject *parent = nullptr);
    virtual ~keyScan();
    void call_move(int mouse_x, int mouse_y);
signals:
    void mouse_move(int mouse_x, int mouse_y);
private:
    int last_mouse_x;
    int last_mouse_y;
    qint64 last_time;
    Display *m_display;
    XRecordRange *range;
    XRecordContext m_context;
    XRecordClientSpec clients;
protected:
    void run();
private:
    void registerd();
    void unregister();
};

#endif // KEYSCAN_H
