#include "my_program_container.h"
My_Program_Container::My_Program_Container(QWidget *parent)
    :Basic_Widget(parent)
{
    close_button->raise();
    menu->addAction(set_program);
    menu->addAction(break_program);
    menu->addAction(focus_action);
    menu->addAction(set_distance_action);
    Basic_Widget::basic_context(menu);
    Container_Widget->setStyleSheet("background:rgba(0,0,0,0)");
    Container_Widget->setMouseTracking(true);
    connect(this, &Basic_Widget::size_changed, this, [=] (QSize size)
    {
        Container_Widget->move(distance_width, distance_height);
        Container_Widget->resize(size - QSize(2 * distance_width, 2 * distance_height));
        if (targetWindow == 0)
        {
            return;
        }
        display = QX11Info::display();
        XWindowAttributes attr;
        if (!XGetWindowAttributes(display, targetWindow, &attr))
        {
            targetWindow = 0;
            display = nullptr;
            return;
        }
        if (targetWidget)
        {
            targetWidget->resize(Container_Widget->size());
        }
        XSetInputFocus(display, targetWindow, RevertToParent, CurrentTime);
        XFlush(display);
        display = nullptr;
    });
    resize(800, 600);
    show();
}
void My_Program_Container::focusInEvent(QFocusEvent *event)
{
    Basic_Widget::focusInEvent(event);
    display = QX11Info::display();
    XWindowAttributes attr;
    if (!XGetWindowAttributes(display, targetWindow, &attr))
    {
        targetWindow = 0;
        display = nullptr;
        return;
    }
    XSetInputFocus(display, targetWindow, RevertToParent, CurrentTime);
    XFlush(display);
}
void My_Program_Container::mousePressEvent(QMouseEvent *event)
{
    Basic_Widget::mousePressEvent(event);
    display = QX11Info::display();
    XWindowAttributes attr;
    if (!XGetWindowAttributes(display, targetWindow, &attr))
    {
        targetWindow = 0;
        display = nullptr;
        return;
    }
    XSetInputFocus(display, targetWindow, RevertToParent, CurrentTime);
    XFlush(display);
}
void My_Program_Container::mouseReleaseEvent(QMouseEvent *event)
{
    Basic_Widget::mouseReleaseEvent(event);
    display = QX11Info::display();
    XWindowAttributes attr;
    if (!XGetWindowAttributes(display, targetWindow, &attr))
    {
        targetWindow = 0;
        display = nullptr;
        return;
    }
    XSetInputFocus(display, targetWindow, RevertToParent, CurrentTime);
    XFlush(display);
}
void My_Program_Container::wheelEvent(QWheelEvent *event)
{
    if (targetWindow != 0)
    {
        event->accept();
        return;
    }
}
void My_Program_Container::contextMenuEvent(QContextMenuEvent *event)
{
    set_program->setEnabled(targetWindow == 0);
    break_program->setEnabled(targetWindow != 0);
    focus_action->setEnabled(targetWindow != 0);
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == set_program)
    {
        QProcess *xdotoolProcess = new QProcess;
        xdotoolProcess->start("/bin/bash", QStringList() << "-c" << "xwininfo -int | grep 'xwininfo: Window id:'");
        connect(xdotoolProcess, static_cast<void (QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished), this, [=]
        {
            QByteArray output = xdotoolProcess->readAllStandardOutput();
            QStringList windowIds = QString(output).split(" ", Qt::SkipEmptyParts);
            disconnect(xdotoolProcess, nullptr, this, nullptr);
            delete xdotoolProcess;
            if (windowIds.size() < 4)
            {
                return;
            }
            bool ok = false;
            Window windowId = windowIds[3].toULong(&ok);
            if (!ok)
            {
                return;
            }
            display = QX11Info::display();
            Window windowPId = static_cast<unsigned long>(get_window_pid(display, windowId));
            QString windowType = get_window_class(display, windowId);
            if (windowPId == static_cast<unsigned long>(QCoreApplication::applicationPid()))
            {
                QMessageBox::question(nullptr, "无法设置", QString("(#`Д´)ﾉ不能自食其果!\nPID:%1").arg(windowPId));
                return;
            }
            QMessageBox::StandardButton final_ans = QMessageBox::question(nullptr, "确认WinId", QString("选取WinId:%1").arg(windowId));
            if (final_ans != QMessageBox::StandardButton::Yes)
            {
                return;
            }
            if (windowType != "_NET_WM_WINDOW_TYPE_NORMAL")
            {
                QMessageBox::StandardButton final_ans = QMessageBox::question(nullptr, "确认_NET_WM_WINDOW_TYPE", QString(">_<|||窗口类型:%1\n强行设置可能出错,是否设置").arg(windowType));
                if (final_ans != QMessageBox::StandardButton::Yes)
                {
                    return;
                }
            }
            targetWindow = windowId;
            update_container();
        });
    }
    else if (know_what == break_program)
    {
        break_out_con();
    }
    else if (know_what == focus_action)
    {
        display = QX11Info::display();
        XWindowAttributes attr;
        if (!XGetWindowAttributes(display, targetWindow, &attr))
        {
            targetWindow = 0;
            display = nullptr;
            return;
        }
        XSetInputFocus(display, targetWindow, RevertToParent, CurrentTime);
        XFlush(display);
    }
    else if (know_what == set_distance_action)
    {
        bool ok = false;
        int new_width = QInputDialog::getInt(nullptr, "获取数值", "宽度距离:", distance_width, 0, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int new_height = QInputDialog::getInt(nullptr, "获取数值", "高度距离:", distance_height, 0, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        distance_width = new_width;
        distance_height = new_height;
        emit Basic_Widget::size_changed(this->get_self()->size());
    }
    else
    {
        basic_action_func(know_what);
    }
}
void My_Program_Container::update_container()
{
    close_button->raise();
    if (targetWindow == 0)
    {
        return;
    }
    display = QX11Info::display();
    XWindowAttributes attr;
    if (!XGetWindowAttributes(display, targetWindow, &attr))
    {
        targetWindow = 0;
        display = nullptr;
        return;
    }
    targetWidget = QWidget::createWindowContainer(QWindow::fromWinId(targetWindow), this->Container_Widget);
    targetWidget->setAttribute(Qt::WA_TranslucentBackground, true);
    targetWidget->setAutoFillBackground(false);
    targetWidget->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    targetWidget->setFocus();
    targetWidget->setMouseTracking(false);
    targetWidget->resize(Container_Widget->size());
    targetWidget->show();
    targetWidget->installEventFilter(this);
    //
    //燃尽了
    display = nullptr;
    emit Basic_Widget::size_changed(this->get_self()->size());
}
void My_Program_Container::break_out_con()
{
    if (targetWindow == 0)
    {
        return;
    }
    display = QX11Info::display();
    XWindowAttributes attr;
    if (!XGetWindowAttributes(display, targetWindow, &attr))
    {
        targetWindow = 0;
        display = nullptr;
        return;
    }
    targetWidget->setParent(nullptr);
    targetWidget->deleteLater();
    targetWidget = nullptr;
    Window rootWindow = DefaultRootWindow(display);
    XSelectInput(display, targetWindow, 0);
    XReparentWindow(display, targetWindow, rootWindow, this->Container_Widget->mapToGlobal(QPoint(0, 0)).x(), this->Container_Widget->mapToGlobal(QPoint(0, 0)).y());
    XMapWindow(display, targetWindow);
    XResizeWindow(display, targetWindow, static_cast<unsigned int>(this->Container_Widget->width()), static_cast<unsigned int>(this->Container_Widget->height()));
    XSetInputFocus(display, targetWindow, RevertToParent, CurrentTime);
    XFlush(display);
    targetWindow = 0;
    display = nullptr;
}
QString My_Program_Container::get_window_class(Display *display, Window window)
{
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *prop = nullptr;
    QString result = "";
    Atom new_wm_TYPE = XInternAtom(display, "_NET_WM_WINDOW_TYPE", False);
    if (XGetWindowProperty(display, window, new_wm_TYPE, 0, 1024, False, XA_ATOM, &actual_type, &actual_format, &nitems, &bytes_after, &prop) != Success)
    {
        return result;
    }
    if (prop && nitems > 0)
    {
        result = QString::fromStdString(XGetAtomName(display, reinterpret_cast<Atom *>(prop)[0]));
    }
    if (prop)
    {
        XFree(prop);
    }
    return result;
}
pid_t My_Program_Container::get_window_pid(Display *display, Window window)
{
    Atom actual_type;
    int actual_format;
    unsigned long nitems;
    unsigned long bytes_after;
    unsigned char *prop = nullptr;
    pid_t pid = 0;
    Atom new_wm_pid = XInternAtom(display, "_NET_WM_PID", False);
    if (XGetWindowProperty(display, window, new_wm_pid, 0, 1, False, XA_CARDINAL, &actual_type, &actual_format, &nitems, &bytes_after, &prop) != Success)
    {
        return pid;
    }
    if (prop && nitems > 0)
    {
        pid = *reinterpret_cast<pid_t *>(prop);
    }
    if (prop)
    {
        XFree(prop);
    }
    return pid;

}
My_Program_Container::~My_Program_Container()
{
    break_out_con();
    if (my_program_container_list)
    {
        my_program_container_list->removeOne(this);
    }
}
void My_Program_Container::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    settings->setValue("distance_width", distance_width);
    settings->setValue("distance_height", distance_height);
}
void My_Program_Container::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    int new_width = settings->value("distance_width", 20).toInt();
    int new_height = settings->value("distance_height", 20).toInt();
    distance_width = new_width;
    distance_height = new_height;
    emit Basic_Widget::size_changed(this->get_self()->size());
}
