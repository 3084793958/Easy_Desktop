#include "keyscan.h"
static void recordEventCallback(char *ptr, XRecordInterceptData *data);
keyScan::keyScan(QObject *parent)
    :QThread(parent)
{
    last_mouse_x = QCursor::pos().x();
    last_mouse_y = QCursor::pos().y();
    last_time = QDateTime::currentMSecsSinceEpoch();
    registerd();
}
keyScan::~keyScan()
{
    unregister();
}
void keyScan::registerd()
{
    Display *display = XOpenDisplay(nullptr);
    clients = XRecordAllClients;
    range = XRecordAllocRange();
    memset(range, 0, sizeof(XRecordRange));
    range->device_events.first = MotionNotify;
    range->device_events.last = MotionNotify;//单一处理
    m_context = XRecordCreateContext(display, 0, &clients, 1, &range, 1);
    XFree(range);
    XSync(display, False);
    m_display = XOpenDisplay(nullptr);
}
void keyScan::run()
{
    if (!doing)
    {
        return;
    }
    XRecordEnableContext(m_display, m_context, recordEventCallback, reinterpret_cast<XPointer>(this));//内存重新解释
}
void keyScan::call_move(int mouse_x, int mouse_y)
{
    if (abs(last_mouse_x - mouse_x) + abs(last_mouse_y - mouse_y) < 5)
    {
        return;
    }
    last_mouse_x = mouse_x;
    last_mouse_y = mouse_y;
    if (QDateTime::currentMSecsSinceEpoch() - last_time <= signal_delta_time)
    {
        return;
    }
    last_time = QDateTime::currentMSecsSinceEpoch();
    emit mouse_move(mouse_x, mouse_y);
}
void recordEventCallback(char *ptr, XRecordInterceptData *data)
{
    if (data->category == XRecordFromServer)
    {
        data->client_swapped = false;
        xEvent *event = reinterpret_cast<xEvent *>(data->data);
        reinterpret_cast<keyScan *>(ptr)->call_move(event->u.keyButtonPointer.rootX, event->u.keyButtonPointer.rootY);
    }
    XRecordFreeData(data);
}
void keyScan::unregister()
{
    Display *display = XOpenDisplay(nullptr);
    XRecordDisableContext(display, m_context);
    XSync(display ,0);
    wait();
    XRecordFreeContext(display, m_context);
    XCloseDisplay(m_display);
}
