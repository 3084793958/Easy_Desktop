#include "my_process_carrier.h"
#include <QX11Info>
#include <X11/Xlib.h>
void My_Process_Carrier::X11_Raise()
{
    Window win_Id = static_cast<Window>(m_WinId);
    Display *display = QX11Info::display();
    XRaiseWindow(display, win_Id);
    XFlush(display);
}
#undef CursorShape
My_Process_Carrier::My_Process_Carrier(QWidget *parent)
    :Basic_Widget(parent)
{
    this->setMouseTracking(true);
    this->setAcceptDrops(true);
    Basic_Carrier->setMouseTracking(true);
    control_Dock->setMouseTracking(true);
    menu->addAction(create_process_widget_action);
    menu->addAction(create_file_widget_action);
    menu->addAction(get_process_widget_action);
    load_file_action->addAction(load_files);
    load_file_action->addAction(load_dir);
    menu->addMenu(load_file_action);
    menu->addSeparator();
    menu->addAction(create_carrier_action);
    menu->addAction(delete_carrier_action);
    menu->addSeparator();
    basic_context(menu);
    control_Dock->Set_Now_page(&carrier_now_page);
    control_Dock->Set_Desktop_Number(&carrier_page_number);
    control_Dock->show();
    connect(this, &Basic_Widget::size_changed, this, [=]
    {
        Basic_Carrier->setGeometry(this->get_self()->geometry());
        My_Process_Carrier::Update_Basic_Desktop();
        control_Dock->resize(Basic_Carrier->width(), 20);
        control_Dock->move(0, Basic_Carrier->height() - 20);
        control_Dock->Update_Widget();
    });
    carrier_widget_list << new QWidget(this->Basic_Carrier);
    Update_Basic_Desktop();
    resize(245, 265);
    move_Timer->setInterval(30);
    connect(move_Timer, &QTimer::timeout, this, &My_Process_Carrier::Timer_End);
    show();
    close_button->raise();
}

My_Process_Carrier::~My_Process_Carrier()
{
    if (my_process_carrier_list)
    {
        my_process_carrier_list->removeOne(this);
    }
}
void My_Process_Carrier::contextMenuEvent(QContextMenuEvent *event)
{
    get_process_widget_action->setEnabled(process_widget_p != nullptr && (*process_widget_p) != nullptr);
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    context_solution(know_what, event->globalPos());
}
void My_Process_Carrier::context_solution(QAction *know_what, QPoint pos)
{
    if (know_what == nullptr)
    {
        return;
    }
    else if (know_what == create_process_widget_action)
    {
        Process_Widget *new_process_widget = new Process_Widget(carrier_widget_list[carrier_now_page]);
        connect(new_process_widget, &Process_Widget::set_to_Carrier_Sig, this, [=]
        {
            (*process_widget_p) = new_process_widget;
        });
        connect(new_process_widget, &Process_Widget::move_To_Desktop_Sig, this, [=](QPoint pos_)
        {
            new_process_widget->in_carrier = false;
            new_process_widget->setParent(this->parentWidget());
            new_process_widget->set_now_page(now_page);
            new_process_widget->set_desktop_number(desktop_number);
            new_process_widget->set_basic_list(basic_list);
            new_process_widget->move(pos_);
            new_process_widget->show();
        });
        new_process_widget->in_carrier = true;
        new_process_widget->set_now_page(&carrier_now_page);
        new_process_widget->set_desktop_number(&carrier_page_number);
        new_process_widget->set_basic_list(&carrier_widget_list);
        new_process_widget->set_WinId(m_WinId);
        new_process_widget->move(Basic_Carrier->mapFromGlobal(pos));
        process_widget_list->append(new_process_widget);
        new_process_widget->process_widget_list = process_widget_list;
    }
    else if (know_what == create_file_widget_action)
    {
        File_Widget *new_process_widget = new File_Widget(carrier_widget_list[carrier_now_page]);
        connect(new_process_widget, &Process_Widget::set_to_Carrier_Sig, this, [=]
        {
            (*process_widget_p) = new_process_widget;
        });
        connect(new_process_widget, &Process_Widget::move_To_Desktop_Sig, this, [=](QPoint pos_)
        {
            new_process_widget->in_carrier = false;
            new_process_widget->setParent(this->parentWidget());
            new_process_widget->set_now_page(now_page);
            new_process_widget->set_desktop_number(desktop_number);
            new_process_widget->set_basic_list(basic_list);
            new_process_widget->move(pos_);
            new_process_widget->show();
        });
        new_process_widget->in_carrier = true;
        new_process_widget->set_now_page(&carrier_now_page);
        new_process_widget->set_desktop_number(&carrier_page_number);
        new_process_widget->set_basic_list(&carrier_widget_list);
        new_process_widget->set_WinId(m_WinId);
        new_process_widget->move(Basic_Carrier->mapFromGlobal(pos));
        file_widget_list->append(new_process_widget);
        new_process_widget->file_widget_list = file_widget_list;
        new_process_widget->file_open_way_process = file_open_way_process;
        new_process_widget->file_open_info_process = file_open_info_process;
        new_process_widget->file_open_path_process = file_open_path_process;
    }
    else if (know_what == get_process_widget_action)
    {
        if (process_widget_p == nullptr)
        {
            return;
        }
        if ((*process_widget_p) == nullptr)
        {
            return;
        }
        (*process_widget_p)->in_carrier = true;
        (*process_widget_p)->setParent(carrier_widget_list[carrier_now_page]);
        (*process_widget_p)->set_now_page(&carrier_now_page);
        (*process_widget_p)->set_desktop_number(&carrier_page_number);
        (*process_widget_p)->set_basic_list(&carrier_widget_list);
        (*process_widget_p)->set_WinId(m_WinId);
        (*process_widget_p)->move(Basic_Carrier->mapFromGlobal(pos));
        (*process_widget_p)->show();
        (*process_widget_p) = nullptr;
    }
    else if (know_what == load_files || know_what == load_dir)
    {
        QStringList filenames;
        if (know_what == load_files)
        {
            filenames = QFileDialog::getOpenFileNames(nullptr, "获取文件", QDir::homePath());
        }
        else
        {
            filenames << QFileDialog::getExistingDirectory(nullptr, "获取文件夹", QDir::homePath());
        }
        X11_Raise();
        int x = pos.x(), y = pos.y(), width = 90, height = 90, delta_x = 5, delta_y = 5;
        bool ok = false;
        x = QInputDialog::getInt(nullptr, "获取数值", "位置坐标:(x,y),宽度:width,高度:height,横向间隔:delta_x,纵向间隔:delta_y\n设置x", x, -2147483647, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        y = QInputDialog::getInt(nullptr, "获取数值", QString("位置坐标:(%1,y),宽度:width,高度:height,横向间隔:delta_x,纵向间隔:delta_y\n设置y").arg(x), y, -2147483647, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        width = QInputDialog::getInt(nullptr, "获取数值", QString("位置坐标:(%1,%2),宽度:width,高度:height,横向间隔:delta_x,纵向间隔:delta_y\n设置width").arg(x).arg(y), width, 0, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        height = QInputDialog::getInt(nullptr, "获取数值", QString("位置坐标:(%1,%2),宽度:%3,高度:height,横向间隔:delta_x,纵向间隔:delta_y\n设置height").arg(x).arg(y).arg(width), height, 0, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        if (filenames.size() <= 1)
        {
            delta_x = QInputDialog::getInt(nullptr, "获取数值", QString("位置坐标:(%1,%2),宽度:%3,高度:%4,横向间隔:delta_x,纵向间隔:delta_y\n设置delta_x").arg(x).arg(y).arg(width).arg(height), delta_x, -2147483647, 2147483647, 1, &ok);
            if (!ok)
            {
                return;
            }
            ok = false;
            delta_y = QInputDialog::getInt(nullptr, "获取数值", QString("位置坐标:(%1,%2),宽度:%3,高度:%4,横向间隔:%5,纵向间隔:delta_y\n设置delta_y").arg(x).arg(y).arg(width).arg(height).arg(delta_x), delta_y, -2147483647, 2147483647, 1, &ok);
            if (!ok)
            {
                return;
            }
            ok = false;
        }
        int x_num = QInputDialog::getInt(nullptr, "获取数值", "设置列数", 2, 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        bool show_close_button_bool = QInputDialog::getItem(nullptr, "显示关闭按钮", "显示关闭按钮", QStringList() << "是" << "否", 0, false, &ok) == "是";
        if (!ok)
        {
            show_close_button_bool = true;
        }
        for (int i = 0; i < filenames.size(); i++)
        {
            File_Widget *new_process_widget = new File_Widget(carrier_widget_list[carrier_now_page]);
            connect(new_process_widget, &Process_Widget::set_to_Carrier_Sig, this, [=]
            {
                (*process_widget_p) = new_process_widget;
            });
            connect(new_process_widget, &Process_Widget::move_To_Desktop_Sig, this, [=](QPoint pos_)
            {
                new_process_widget->in_carrier = false;
                new_process_widget->setParent(this->parentWidget());
                new_process_widget->set_now_page(now_page);
                new_process_widget->set_desktop_number(desktop_number);
                new_process_widget->set_basic_list(basic_list);
                new_process_widget->move(pos_);
                new_process_widget->show();
            });
            new_process_widget->in_carrier = true;
            new_process_widget->set_now_page(&carrier_now_page);
            new_process_widget->set_desktop_number(&carrier_page_number);
            new_process_widget->set_basic_list(&carrier_widget_list);
            new_process_widget->set_WinId(m_WinId);
            new_process_widget->Basic_Widget::resize(width, height);
            new_process_widget->move(x + (width + delta_x) * (i % x_num), y + (height + delta_y) * (i / x_num));
            file_widget_list->append(new_process_widget);
            new_process_widget->file_widget_list = file_widget_list;
            new_process_widget->quickly_set(filenames[i]);
            new_process_widget->show_close_button->setChecked(show_close_button_bool);
            new_process_widget->close_button->setVisible(show_close_button_bool);
            new_process_widget->file_open_way_process = file_open_way_process;
            new_process_widget->file_open_info_process = file_open_info_process;
            new_process_widget->file_open_path_process = file_open_path_process;
        }
    }
    else if (know_what == create_carrier_action)
    {
        carrier_widget_list << new QWidget(this->Basic_Carrier);
        Update_Basic_Desktop();
        carrier_page_number = carrier_widget_list.size();
        control_Dock->Update_Widget();
    }
    else if (know_what == delete_carrier_action)
    {
        QMessageBox::StandardButton box = QMessageBox::question(this, "删除当前载体页", "是否删除当前载体页?", QMessageBox::Yes | QMessageBox::No);
        if (box != QMessageBox::Yes)
        {
            return;
        }
        if (carrier_widget_list.count() <= 1)
        {
            auto desktop = carrier_widget_list[carrier_now_page];
            carrier_widget_list.removeAt(carrier_now_page);
            delete desktop;
            carrier_widget_list << new QWidget(this->Basic_Carrier);
            Update_Basic_Desktop();
            return;
        }
        if (carrier_now_page >= 0 && carrier_now_page < carrier_widget_list.count())
        {
            auto desktop = carrier_widget_list[carrier_now_page];
            carrier_widget_list.removeAt(carrier_now_page);
            delete desktop;
            carrier_page_number = carrier_widget_list.size();
            if (carrier_now_page >= carrier_widget_list.count())
            {
                carrier_now_page = carrier_widget_list.count() - 1;
            }
            Update_Basic_Desktop();
            control_Dock->Update_Widget();
        }
    }
    else
    {
        basic_action_func(know_what);
    }
}
void My_Process_Carrier::wheelEvent(QWheelEvent *event)
{
    if (event->angleDelta().y() != 0)
    {
        int angle = event->angleDelta().ry();
        if (angle > 0)
        {
            if (carrier_now_page <= 0)
            {
                return;
            }
            carrier_now_page--;
            move_Timer->stop();
            desktop_move_x += this->Basic_Carrier->width();
            My_Process_Carrier::Call_Timer_Move();
        }
        else
        {
            if (carrier_now_page >= carrier_page_number - 1)
            {
                return;
            }
            carrier_now_page++;
            move_Timer->stop();
            desktop_move_x -= this->Basic_Carrier->width();
            My_Process_Carrier::Call_Timer_Move();
        }
        control_Dock->Update_Widget();
    }
    event->accept();
}
void My_Process_Carrier::dragEnterEvent(QDragEnterEvent *event)
{
    if (*m_allow_drop && event->mimeData()->hasUrls())
    {
        event->accept();
    }
}
void My_Process_Carrier::dropEvent(QDropEvent *event)
{
    if (*m_allow_drop && event->mimeData()->hasUrls())
    {
        QStringList filenames;
        for (QUrl url : event->mimeData()->urls())
        {
            if (url.isValid())
            {
                filenames << url.path();
            }
        }
        int x = event->pos().x(), y = event->pos().y(), width = 90, height = 90, delta_x = 5, delta_y = 5;
        bool ok = false;
        x = QInputDialog::getInt(nullptr, "获取数值", "位置坐标:(x,y),宽度:width,高度:height,横向间隔:delta_x,纵向间隔:delta_y\n设置x", x, -2147483647, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        y = QInputDialog::getInt(nullptr, "获取数值", QString("位置坐标:(%1,y),宽度:width,高度:height,横向间隔:delta_x,纵向间隔:delta_y\n设置y").arg(x), y, -2147483647, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        width = QInputDialog::getInt(nullptr, "获取数值", QString("位置坐标:(%1,%2),宽度:width,高度:height,横向间隔:delta_x,纵向间隔:delta_y\n设置width").arg(x).arg(y), width, 0, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        height = QInputDialog::getInt(nullptr, "获取数值", QString("位置坐标:(%1,%2),宽度:%3,高度:height,横向间隔:delta_x,纵向间隔:delta_y\n设置height").arg(x).arg(y).arg(width), height, 0, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int x_num = 2;
        if (filenames.size() > 1)
        {
            delta_x = QInputDialog::getInt(nullptr, "获取数值", QString("位置坐标:(%1,%2),宽度:%3,高度:%4,横向间隔:delta_x,纵向间隔:delta_y\n设置delta_x").arg(x).arg(y).arg(width).arg(height), delta_x, -2147483647, 2147483647, 1, &ok);
            if (!ok)
            {
                return;
            }
            ok = false;
            delta_y = QInputDialog::getInt(nullptr, "获取数值", QString("位置坐标:(%1,%2),宽度:%3,高度:%4,横向间隔:%5,纵向间隔:delta_y\n设置delta_y").arg(x).arg(y).arg(width).arg(height).arg(delta_x), delta_y, -2147483647, 2147483647, 1, &ok);
            if (!ok)
            {
                return;
            }
            ok = false;
            x_num = QInputDialog::getInt(nullptr, "获取数值", "设置列数", 2, 1, 2147483647, 1, &ok);
            if (!ok)
            {
                return;
            }
            ok = false;
        }
        bool show_close_button_bool = QInputDialog::getItem(nullptr, "显示关闭按钮", "显示关闭按钮", QStringList() << "是" << "否", 0, false, &ok) == "是";
        if (!ok)
        {
            show_close_button_bool = true;
        }
        for (int i = 0; i < filenames.size(); i++)
        {
            File_Widget *new_process_widget = new File_Widget(carrier_widget_list[carrier_now_page]);
            connect(new_process_widget, &Process_Widget::set_to_Carrier_Sig, this, [=]
            {
                (*process_widget_p) = new_process_widget;
            });
            connect(new_process_widget, &Process_Widget::move_To_Desktop_Sig, this, [=](QPoint pos_)
            {
                new_process_widget->in_carrier = false;
                new_process_widget->setParent(this->parentWidget());
                new_process_widget->set_now_page(now_page);
                new_process_widget->set_desktop_number(desktop_number);
                new_process_widget->set_basic_list(basic_list);
                new_process_widget->move(pos_);
                new_process_widget->show();
            });
            new_process_widget->in_carrier = true;
            new_process_widget->set_now_page(&carrier_now_page);
            new_process_widget->set_desktop_number(&carrier_page_number);
            new_process_widget->set_basic_list(&carrier_widget_list);
            new_process_widget->set_WinId(m_WinId);
            new_process_widget->Basic_Widget::resize(width, height);
            new_process_widget->move(x + (width + delta_x) * (i % x_num), y + (height + delta_y) * (i / x_num));
            file_widget_list->append(new_process_widget);
            new_process_widget->file_widget_list = file_widget_list;
            new_process_widget->quickly_set(filenames[i]);
            new_process_widget->show_close_button->setChecked(show_close_button_bool);
            new_process_widget->close_button->setVisible(show_close_button_bool);
            new_process_widget->file_open_way_process = file_open_way_process;
            new_process_widget->file_open_info_process = file_open_info_process;
            new_process_widget->file_open_path_process = file_open_path_process;
        }
    }
}
void My_Process_Carrier::desktop_Move_Update(int delta_move)
{
    move_Timer->stop();
    desktop_move_x -= delta_move * this->Basic_Carrier->width();
    My_Process_Carrier::Call_Timer_Move();
}
void My_Process_Carrier::Call_Timer_Move()
{
    move_Timer->stop();
    timer_move_x = desktop_move_x;
    run_time = 0;
    for (int i = 0; i < carrier_widget_list.count(); i++)
    {
        carrier_widget_list[i]->show();
    }
    move_Timer->start();
}
void My_Process_Carrier::Timer_End()
{
    if (run_time >= 15)
    {
        move_Timer->stop();
        My_Process_Carrier::Update_Basic_Desktop();
        return;
    }
    run_time++;
    int move_x;
    move_x = timer_move_x / 15;
    desktop_move_x -= move_x;
    for (int i = 0; i < carrier_widget_list.count(); i++)
    {
        carrier_widget_list[i]->move(carrier_widget_list[i]->x() + move_x, 0);
    }
}
void My_Process_Carrier::Update_Basic_Desktop()
{
    for (int i = 0; i < carrier_widget_list.count(); i++)
    {
        carrier_widget_list[i]->move((i - carrier_now_page) * Basic_Carrier->width(), 0);
        carrier_widget_list[i]->resize(Basic_Carrier->size() - QSize(0, 20));
        if (i == carrier_now_page)
        {
            carrier_widget_list[i]->show();
        }
        else
        {
            carrier_widget_list[i]->hide();
        }
        carrier_widget_list[i]->setMouseTracking(true);
    }
}
Page_Dock_Button::Page_Dock_Button(QWidget *parent)
    :QPushButton(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setStyleSheet("QPushButton{border-radius:5px 5px;background:rgba(255,255,255,150)}"
                  "QPushButton:hover{border-radius:5px 5px;background:rgba(255,255,255,200)}"
                  "QPushButton:pressed{border-radius:5px 5px;background:rgba(255,255,255,150)}");
    connect(this, &QPushButton::pressed, this, [=]
    {
        int delta_page = Button_Number - *now_page;
        *now_page = Button_Number;
        dynamic_cast<Page_Control_Dock*>(this->parent())->Changed_Signals();
        dynamic_cast<My_Process_Carrier*>(this->parent()->parent()->parent())->desktop_Move_Update(delta_page);
    });
    show();
}
void Page_Dock_Button::set_Number(int number)
{
    Button_Number = number;
}
void Page_Dock_Button::set_Now_Page(int *m_now_page)
{
    now_page = m_now_page;
}
void Page_Dock_Button::Update_Button()
{
    if (*now_page == Button_Number)
    {
        resize(50, 10);
    }
    else
    {
        resize(10 ,10);
    }
}
Page_Control_Dock::Page_Control_Dock(QWidget *parent)
    :QWidget(parent)
{
    show();
}
void Page_Control_Dock::Update_Widget()
{
    int basic_x = x() + width() / 2;
    int delta_number = *Desktop_NUmber - Dock_Button_List.count();
    if (delta_number > 0)
    {
        for (int i = 0; i < delta_number; i++)
        {
            Page_Dock_Button *push_button = new Page_Dock_Button(this);
            Dock_Button_List<<push_button;
            push_button->setMouseTracking(true);
            push_button->set_Now_Page(now_page);
            push_button->setObjectName("button" + QString::number(i));
            push_button->show();
        }
    }
    else
    {
        for (int i = 0; i < -delta_number; i++)
        {
            QPushButton *button = Dock_Button_List[*Desktop_NUmber];
            Dock_Button_List.removeAt(*Desktop_NUmber);
            disconnect(button, nullptr, this, nullptr);
            delete button;
        }
    }
    //distance = 20;
    for (int i = 0; i < Dock_Button_List.count(); i++)
    {
        Dock_Button_List[i]->set_Number(i);
        Dock_Button_List[i]->Update_Button();
        if (i == 0)
        {
            Dock_Button_List[i]->move(25, 5);
        }
        else
        {
            Dock_Button_List[i]->move(Dock_Button_List[i - 1]->x() + Dock_Button_List[i - 1]->width() + 20, 5);
        }
        Dock_Button_List[i]->raise();
    }
    if (Dock_Button_List.count() <= 1)
    {
        hide();
        return;
    }
    show();
    resize(Dock_Button_List.back()->x() + Dock_Button_List.back()->width() + 25, 20);
    basic_x -= width() / 2;
    move(basic_x, y());
    raise();
}
void Page_Control_Dock::Changed_Signals()
{
    for (int i = 0; i < Dock_Button_List.count(); i++)
    {
        Dock_Button_List[i]->Update_Button();
    }
    Update_Widget();
}
void Page_Control_Dock::Set_Desktop_Number(int *number)
{
    Desktop_NUmber = number;
}
void Page_Control_Dock::Set_Now_page(int *m_now_page)
{
    now_page = m_now_page;
}
void My_Process_Carrier::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    settings->setValue("carrier_now_page", carrier_now_page);
    settings->setValue("carrier_page_number", carrier_page_number);
    for (int i = 0; i < carrier_widget_list.count(); i++)
    {
        for (int j = 0; j < carrier_widget_list[i]->children().count(); j++)
        {
            qobject_cast<Process_Widget *>(carrier_widget_list[i]->children()[j])->carrier_index = save_index_number;
        }
    }
}
void My_Process_Carrier::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    carrier_now_page = settings->value("carrier_now_page", 0).toInt();
    carrier_page_number = settings->value("carrier_page_number", 1).toInt();
    if (carrier_now_page < 0)
    {
        carrier_now_page = 0;
    }
    if (carrier_page_number < 1)
    {
        carrier_page_number = 1;
    }
    if (carrier_now_page >= carrier_page_number)
    {
        carrier_now_page = carrier_page_number - 1;
    }
    int carrier_widget_list_size = carrier_widget_list.count();
    for (int i = 0; i < carrier_widget_list_size; i++)
    {
        carrier_widget_list[0]->~QWidget();
        carrier_widget_list.removeAt(0);
    }
    for (int i = 0; i < carrier_page_number; i++)
    {
        carrier_widget_list << new QWidget(this->Basic_Carrier);
    }
    if (carrier_widget_list.count() == 0)
    {
        carrier_widget_list << new QWidget(this->Basic_Carrier);
    }
    Update_Basic_Desktop();
    control_Dock->Update_Widget();
}
