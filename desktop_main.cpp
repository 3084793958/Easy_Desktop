#include "desktop_main.h"
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>
Basic_Desktop::Basic_Desktop(QWidget *parent, int m_desktop_width, int m_desktop_height)
    :QWidget(parent)
{
    desktop_width = m_desktop_width;
    desktop_height = m_desktop_height;
    Basic_Desktop::Update_Widget();
}
void Basic_Desktop::Update_Widget()
{
    move(0, 0);
    resize(desktop_width, desktop_height);
    show();
}
void Basic_Desktop::set_Desktop_Size(int d_width, int d_height)
{
    desktop_width = d_width;
    desktop_height = d_height;
}
Dock_Button::Dock_Button(QWidget *parent)
    :QPushButton(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setStyleSheet("QPushButton{border-radius:5px 5px;background:rgba(255,255,255,150)}"
                  "QPushButton:hover{border-radius:5px 5px;background:rgba(255,255,255,200)}"
                  "QPushButton:pressed{border-radius:5px 5px;background:rgba(255,255,255,150)}");
    connect(this, &QPushButton::pressed, this, [=]
    {
        /*if (*locking_desktop)
        {
            return;
        }*/
        int delta_page = Button_Number - *now_page;
        *now_page = Button_Number;
        dynamic_cast<Desktop_Control_Dock*>(this->parent())->Changed_Signals();
        dynamic_cast<Desktop_Main*>(this->parent()->parent())->desktop_Move_Update(delta_page);
    });
}
void Dock_Button::set_locking_desktop(bool *m_locking_desktop)
{
    locking_desktop = m_locking_desktop;
}
void Dock_Button::set_Number(int number)
{
    Button_Number = number;
}
void Dock_Button::set_Now_Page(int *m_now_page)
{
    now_page = m_now_page;
}
void Dock_Button::Update_Button()
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
Desktop_Control_Dock::Desktop_Control_Dock(QWidget *parent)
    :QWidget(parent)
{
    resize(0, 30);
    background->resize(0, 0);
    background->setStyleSheet("border-radius:10px 10px;background:rgba(0,0,0,75)");
    menu->addAction(to_center);
    menu->addAction(call_update);
    menu->addAction(hide_update);
    show();
}
void Desktop_Control_Dock::Update_Widget()
{
    int basic_x = x() + width() / 2;
    int delta_number = *Desktop_NUmber - Dock_Button_List.count();
    if (delta_number > 0)
    {
        for (int i = 0; i < delta_number; i++)
        {
            Dock_Button *push_button = new Dock_Button(this);
            Dock_Button_List<<push_button;
            push_button->set_Now_Page(now_page);
            push_button->set_locking_desktop(locking_desktop);
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
            Dock_Button_List[i]->move(25, 10);
        }
        else
        {
            Dock_Button_List[i]->move(Dock_Button_List[i - 1]->x() + Dock_Button_List[i - 1]->width() + 20, 10);
        }
        Dock_Button_List[i]->raise();
    }
    if (Dock_Button_List.count() <= 1)
    {
        hide();
        return;
    }
    resize(Dock_Button_List.back()->x() + Dock_Button_List.back()->width() + 25, 30);
    background->resize(this->size());
    basic_x -= width() / 2;
    move(basic_x, y());
    if (*allow_dock_show)
    {
        show();
    }
    else
    {
        hide();
    }
    raise();
}
void Desktop_Control_Dock::Changed_Signals()
{
    for (int i = 0; i < Dock_Button_List.count(); i++)
    {
        Dock_Button_List[i]->Update_Button();
    }
    Update_Widget();
}
void Desktop_Control_Dock::First_Set()
{
    resize(0, 30);
    move(desktop_width / 2, desktop_height - 120);
    Desktop_Control_Dock::Update_Widget();
}
void Desktop_Control_Dock::set_Desktop_Size(int d_width, int d_height)
{
    desktop_width = d_width;
    desktop_height = d_height;
}
void Desktop_Control_Dock::Set_Desktop_Number(int *number)
{
    Desktop_NUmber = number;
}
void Desktop_Control_Dock::Set_Dock_Show_Bool(bool *m_allow_dock_show)
{
    allow_dock_show = m_allow_dock_show;
}
void Desktop_Control_Dock::Set_Now_page(int *m_now_page)
{
    now_page = m_now_page;
}
void Desktop_Control_Dock::set_locking_desktop(bool *m_locking_desktop)
{
    locking_desktop = m_locking_desktop;
}
void Desktop_Control_Dock::mousePressEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        press_point = event->pos();
        on_press = true;
    }
}
void Desktop_Control_Dock::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        on_press = false;
    }
}
void Desktop_Control_Dock::mouseMoveEvent(QMouseEvent *event)
{
    if (on_press)
    {
        if (basic_pos)
        {
            move(event->globalPos() - *basic_pos - press_point);
        }
        else
        {
            move(event->globalPos() - press_point);
        }
    }
}
void Desktop_Control_Dock::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == to_center)
    {
        hide();
        resize(0, 30);
        move(desktop_width / 2, y());
        Desktop_Control_Dock::Update_Widget();
    }
    else if (know_what == call_update)
    {
        Desktop_Control_Dock::Update_Widget();
    }
    else if (know_what == hide_update)
    {
        *allow_dock_show = false;
        hide();
    }
}
Desktop_Main::Desktop_Main(QWidget *parent)
    :QWidget(parent)
{
    background_Add_Action->addAction(my_clock_action);
    background_Add_Action->addAction(text_edit_action);
    background_Add_Action->addAction(my_label_action);
    background_Add_Action->addAction(my_process_action);
    background_Add_Action->addAction(my_file_action);
    background_Add_Action->addAction(file_tree_action);
    background_Add_Action->addAction(my_process_Carrier_action);
    background_Add_Action->addAction(my_program_INNER_action);
    my_chart_menu->addAction(cpu_chart_action);
    my_chart_menu->addAction(ram_chart_action);
    my_chart_menu->addAction(net_chart_action);
    my_chart_menu->addAction(disk_chart_action);
    my_chart_menu->addAction(pulseaudio_chart_action);
    background_Add_Action->addMenu(my_chart_menu);
    menu->addMenu(background_Add_Action);
    desktop_control_action->addAction(lock_desktop);
    desktop_control_action->addAction(unlock_desktop);
    desktop_control_action->addAction(show_dock_action);
    desktop_control_action->addAction(hide_dock_action);
    desktop_control_action->addAction(Add_new_desktop);
    desktop_control_action->addAction(delete_this_desktop);
    menu->addMenu(desktop_control_action);
    menu->addSeparator();
    play_menu->addAction(background_play_Action);
    play_menu->addAction(background_pause_Action);
    menu->addMenu(play_menu);
    mouse_menu->addAction(mouse_play_Action);
    mouse_menu->addAction(mouse_pause_Action);
    menu->addMenu(mouse_menu);
    Slider_menu->addAction(slider_action);
    menu->addMenu(Slider_menu);
    menu->addSeparator();
    menu->addAction(save_Action);
    menu->addAction(load_Action);
    menu->addSeparator();
    menu->addAction(background_setting_Action);
    menu->addAction(debugging_setting_Action);
    menu->addAction(Quit_Action);
    this->setAcceptDrops(true);
    move_Timer->setInterval(30);
    connect(move_Timer, &QTimer::timeout, this, &Desktop_Main::Timer_End);
    control_Dock->basic_pos = &basic_pos;
}
void Desktop_Main::Connection_Update()
{
    connect(desktop_background, &Desktop_Background::send_position, this, [=](int value, QString text)
    {
        slider_action->set_second(value, text);
    });
    connect(slider_action, &Media_WidgetAction::change_signals_P, this, [=](int value)
    {
        desktop_background->Set_Position(value);
    });
    connect(slider_action, &Media_WidgetAction::change_signals_V, this, [=](int value)
    {
        desktop_background->Set_Volume(value);
    });
    connect(slider_action, &Media_WidgetAction::change_signals_S, this, [=](int value)
    {
        desktop_background->Set_Speed(value);
    });
}
void Desktop_Main::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what=menu->exec(mapToGlobal(event->pos()));
    if (know_what == background_setting_Action)
    {
        if (setting_widget)
        {
            setting_widget->activateWindow();
            setting_widget->show();
            setting_widget->Table_Update();
        }
    }
    else if (know_what == debugging_setting_Action)
    {
        if (experimental_settings)
        {
            experimental_settings->activateWindow();
            experimental_settings->show();
        }
    }
    else if (know_what == Quit_Action)
    {
        int count = file_widget_list.count();
        for (int i = 0; i < count; i++)
        {
            file_widget_list[0]->~File_Widget();
        }
        count = my_clock_list.count();
        for (int i = 0; i < count; i++)
        {
            my_clock_list[0]->~My_Clock();
        }
        count = my_label_list.count();
        for (int i = 0; i < count; i++)
        {
            my_label_list[0]->~My_Label();
        }
        count = my_lineedit_list.count();
        for (int i = 0; i < count; i++)
        {
            my_lineedit_list[0]->~My_LineEdit();
        }
        count = my_process_carrier_list.count();
        for (int i = 0; i < count; i++)
        {
            my_process_carrier_list[0]->~My_Process_Carrier();
        }
        count = my_program_container_list.count();
        for (int i = 0; i < count; i++)
        {
            my_program_container_list[0]->break_out_con();
            my_program_container_list[0]->~My_Program_Container();
        }
        count = process_widget_list.count();
        for (int i = 0; i < count; i++)
        {
            process_widget_list[0]->~Process_Widget();
        }
        count = file_tree_list.count();
        for (int i = 0; i < count; i++)
        {
            file_tree_list[0]->~File_Tree();
        }
        count = cpu_chart_list.count();
        for (int i = 0; i < count; i++)
        {
            cpu_chart_list[0]->~CPU_Chart();
        }
        count = ram_chart_list.count();
        for (int i = 0; i < count; i++)
        {
            ram_chart_list[0]->~RAM_Chart();
        }
        count = net_chart_list.count();
        for (int i = 0; i < count; i++)
        {
            net_chart_list[0]->~NET_Chart();
        }
        count = disk_chart_list.count();
        for (int i = 0; i < count; i++)
        {
            disk_chart_list[0]->~DISK_Chart();
        }
        count = pulseaudio_chart_list.count();
        for (int i = 0; i < count; i++)
        {
            pulseaudio_chart_list[0]->~PulseAudio_Chart();
        }
        QApplication::quit();
    }
    else if (know_what == background_play_Action)
    {
        desktop_background->Play();
    }
    else if (know_what == background_pause_Action)
    {
        desktop_background->Pause();
    }
    else if (know_what == mouse_play_Action)
    {
        desktop_background->Mouse_Play();
    }
    else if (know_what == mouse_pause_Action)
    {
        desktop_background->Mouse_Pause();
    }
    else if (know_what == Add_new_desktop)
    {
        Basic_Desktop *basic_desktop = new Basic_Desktop(this, desktop_width, desktop_height);
        desktop_core_dock_list<<basic_desktop;
        Desktop_NUmber = desktop_core_dock_list.count();
        Desktop_Main::Update_Basic_Desktop();
        control_Dock->Update_Widget();
    }
    else if (know_what == delete_this_desktop)
    {
        QMessageBox::StandardButton box = QMessageBox::question(this, "删除当前桌面", "是否删除当前桌面?", QMessageBox::Yes | QMessageBox::No);
        if (box != QMessageBox::Yes)
        {
            return;
        }
        if (desktop_core_dock_list.count() <= 1)
        {
            auto desktop = desktop_core_dock_list[now_page];
            desktop_core_dock_list.removeAt(now_page);
            delete desktop;
            Basic_Desktop *basic_desktop = new Basic_Desktop(this, desktop_width, desktop_height);
            desktop_core_dock_list<<basic_desktop;
            Desktop_Main::Update_Basic_Desktop();
            return;
        }
        if (now_page >= 0 && now_page < desktop_core_dock_list.count())
        {
            auto desktop = desktop_core_dock_list[now_page];
            desktop_core_dock_list.removeAt(now_page);
            delete desktop;
            Desktop_NUmber = desktop_core_dock_list.count();
            if (now_page >= desktop_core_dock_list.count())
            {
                now_page = desktop_core_dock_list.count() - 1;
            }
            Desktop_Main::Update_Basic_Desktop();
            control_Dock->Update_Widget();
            control_Dock->raise();
        }
    }
    else if (know_what == show_dock_action)
    {
        allow_dock_show = true;
        control_Dock->show();
    }
    else if (know_what == hide_dock_action)
    {
        allow_dock_show = false;
        control_Dock->hide();
    }
    else if (know_what == lock_desktop)
    {
        locking_desktop = true;
    }
    else if (know_what == unlock_desktop)
    {
        locking_desktop = false;
    }
    else if (know_what == save_Action)
    {
        save();
    }
    else if (know_what == load_Action)
    {
        load();
    }
    else if (know_what == my_clock_action)
    {
        My_Clock *my_clock = new My_Clock(desktop_core_dock_list[now_page]);
        my_clock->set_now_page(&now_page);
        my_clock->set_desktop_number(&Desktop_NUmber);
        my_clock->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_clock->move(event->globalPos() - basic_pos);
        my_clock_list.append(my_clock);
        my_clock->my_clock_list = &my_clock_list;
    }
    else if (know_what == text_edit_action)
    {
        My_LineEdit *my_lineedit = new My_LineEdit(desktop_core_dock_list[now_page]);
        my_lineedit->set_now_page(&now_page);
        my_lineedit->set_desktop_number(&Desktop_NUmber);
        my_lineedit->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_lineedit->set_WinId(m_WinId);
        my_lineedit->move(event->globalPos() - basic_pos);
        my_lineedit_list.append(my_lineedit);
        my_lineedit->my_lineedit_list = &my_lineedit_list;
    }
    else if (know_what == my_label_action)
    {
        My_Label *my_label = new My_Label(desktop_core_dock_list[now_page]);
        my_label->set_now_page(&now_page);
        my_label->set_desktop_number(&Desktop_NUmber);
        my_label->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_label->set_WinId(m_WinId);
        my_label->move(event->globalPos() - basic_pos);
        my_label_list.append(my_label);
        my_label->my_label_list = &my_label_list;
    }
    else if (know_what == my_process_action)
    {
        Process_Widget *my_process = new Process_Widget(desktop_core_dock_list[now_page]);
        connect(my_process, &Process_Widget::set_to_Carrier_Sig, this, [=]
        {
            process_widget_p = my_process;
        });
        connect(my_process, &Process_Widget::move_To_Desktop_Sig, this, [=](QPoint pos_)
        {
            my_process->in_carrier = false;
            my_process->setParent(desktop_core_dock_list[now_page]);
            my_process->set_now_page(&now_page);
            my_process->set_desktop_number(&Desktop_NUmber);
            my_process->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
            my_process->set_WinId(m_WinId);
            my_process->move(pos_);
            my_process->show();
        });
        my_process->set_now_page(&now_page);
        my_process->set_desktop_number(&Desktop_NUmber);
        my_process->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_process->set_WinId(m_WinId);
        my_process->move(event->globalPos() - basic_pos);
        process_widget_list.append(my_process);
        my_process->process_widget_list = &process_widget_list;
    }
    else if (know_what == my_file_action)
    {
        File_Widget *my_file = new File_Widget(desktop_core_dock_list[now_page]);
        connect(my_file, &Process_Widget::set_to_Carrier_Sig, this, [=]
        {
            process_widget_p = my_file;
        });
        connect(my_file, &Process_Widget::move_To_Desktop_Sig, this, [=](QPoint pos_)
        {
            my_file->in_carrier = false;
            my_file->setParent(desktop_core_dock_list[now_page]);
            my_file->set_now_page(&now_page);
            my_file->set_desktop_number(&Desktop_NUmber);
            my_file->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
            my_file->set_WinId(m_WinId);
            my_file->move(pos_);
            my_file->show();
        });
        my_file->set_now_page(&now_page);
        my_file->set_desktop_number(&Desktop_NUmber);
        my_file->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_file->set_WinId(m_WinId);
        my_file->move(event->globalPos() - basic_pos);
        file_widget_list.append(my_file);
        my_file->file_widget_list = &file_widget_list;
        my_file->file_open_way_process = file_open_way_process;
        my_file->file_open_info_process = file_open_info_process;
        my_file->file_open_path_process = file_open_path_process;
    }
    else if (know_what == my_process_Carrier_action)
    {
        My_Process_Carrier *my_process_Carrier = new My_Process_Carrier(desktop_core_dock_list[now_page]);
        my_process_Carrier->set_now_page(&now_page);
        my_process_Carrier->set_desktop_number(&Desktop_NUmber);
        my_process_Carrier->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_process_Carrier->m_WinId = m_WinId;
        my_process_Carrier->process_widget_p = &process_widget_p;
        my_process_Carrier->move(event->globalPos() - basic_pos);
        my_process_carrier_list.append(my_process_Carrier);
        my_process_Carrier->my_process_carrier_list = &my_process_carrier_list;
        my_process_Carrier->process_widget_list = &process_widget_list;
        my_process_Carrier->file_widget_list = &file_widget_list;
        my_process_Carrier->m_allow_drop = allow_drop;
        my_process_Carrier->file_open_way_process = file_open_way_process;
        my_process_Carrier->file_open_info_process = file_open_info_process;
        my_process_Carrier->file_open_path_process = file_open_path_process;
    }
    else if (know_what == my_program_INNER_action)
    {
        My_Program_Container *my_program_Carrier = new My_Program_Container(desktop_core_dock_list[now_page]);
        my_program_Carrier->set_now_page(&now_page);
        my_program_Carrier->set_desktop_number(&Desktop_NUmber);
        my_program_Carrier->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_program_Carrier->move(event->globalPos() - basic_pos);
        my_program_container_list.append(my_program_Carrier);
        my_program_Carrier->my_program_container_list = &my_program_container_list;
    }
    else if (know_what == cpu_chart_action)
    {
        CPU_Chart *cpu_chart = new CPU_Chart(desktop_core_dock_list[now_page]);
        cpu_chart->set_now_page(&now_page);
        cpu_chart->set_desktop_number(&Desktop_NUmber);
        cpu_chart->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        cpu_chart->move(event->globalPos() - basic_pos);
        cpu_chart_list.append(cpu_chart);
        cpu_chart->cpu_chart_list = &cpu_chart_list;
    }
    else if (know_what == ram_chart_action)
    {
        RAM_Chart *ram_chart = new RAM_Chart(desktop_core_dock_list[now_page]);
        ram_chart->set_now_page(&now_page);
        ram_chart->set_desktop_number(&Desktop_NUmber);
        ram_chart->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        ram_chart->move(event->globalPos() - basic_pos);
        ram_chart_list.append(ram_chart);
        ram_chart->ram_chart_list = &ram_chart_list;
    }
    else if (know_what == net_chart_action)
    {
        NET_Chart *net_chart = new NET_Chart(desktop_core_dock_list[now_page]);
        net_chart->set_now_page(&now_page);
        net_chart->set_desktop_number(&Desktop_NUmber);
        net_chart->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        net_chart->move(event->globalPos() - basic_pos);
        net_chart_list.append(net_chart);
        net_chart->net_chart_list = &net_chart_list;
    }
    else if (know_what == disk_chart_action)
    {
        DISK_Chart *disk_chart = new DISK_Chart(desktop_core_dock_list[now_page]);
        disk_chart->set_now_page(&now_page);
        disk_chart->set_desktop_number(&Desktop_NUmber);
        disk_chart->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        disk_chart->move(event->globalPos() - basic_pos);
        disk_chart_list.append(disk_chart);
        disk_chart->disk_chart_list = &disk_chart_list;
    }
    else if (know_what == pulseaudio_chart_action)
    {
        PulseAudio_Chart *pulseaudio_chart = new PulseAudio_Chart(desktop_core_dock_list[now_page]);
        pulseaudio_chart->set_now_page(&now_page);
        pulseaudio_chart->set_desktop_number(&Desktop_NUmber);
        pulseaudio_chart->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        pulseaudio_chart->move(event->globalPos() - basic_pos);
        pulseaudio_chart_list.append(pulseaudio_chart);
        pulseaudio_chart->pulseaudio_chart_list = &pulseaudio_chart_list;
    }
    else if (know_what == file_tree_action)
    {
        File_Tree *file_tree = new File_Tree(desktop_core_dock_list[now_page]);
        file_tree->set_now_page(&now_page);
        file_tree->set_desktop_number(&Desktop_NUmber);
        file_tree->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        file_tree->m_WinId = m_WinId;
        file_tree->move(event->globalPos() - basic_pos);
        file_tree_list.append(file_tree);
        file_tree->file_tree_list = &file_tree_list;
        file_tree->file_open_way_process = file_open_way_process;
        file_tree->file_open_info_process = file_open_info_process;
        file_tree->file_open_path_process = file_open_path_process;
        file_tree->m_allow_drop = allow_drop;
    }
}
void Desktop_Main::dropEvent(QDropEvent *event)
{
    if (*allow_drop && event->mimeData()->hasUrls())
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
            File_Widget *my_file = new File_Widget(desktop_core_dock_list[now_page]);
            connect(my_file, &Process_Widget::set_to_Carrier_Sig, this, [=]
            {
                process_widget_p = my_file;
            });
            connect(my_file, &Process_Widget::move_To_Desktop_Sig, this, [=](QPoint pos_)
            {
                my_file->in_carrier = false;
                my_file->setParent(desktop_core_dock_list[now_page]);
                my_file->set_now_page(&now_page);
                my_file->set_desktop_number(&Desktop_NUmber);
                my_file->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
                my_file->set_WinId(m_WinId);
                my_file->move(pos_);
                my_file->show();
            });
            my_file->set_now_page(&now_page);
            my_file->set_desktop_number(&Desktop_NUmber);
            my_file->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
            my_file->set_WinId(m_WinId);
            my_file->Basic_Widget::resize(width, height);
            my_file->move(x + (width + delta_x) * (i % x_num), y + (height + delta_y) * (i / x_num));
            file_widget_list.append(my_file);
            my_file->file_widget_list = &file_widget_list;
            my_file->quickly_set(filenames[i]);
            my_file->show_close_button->setChecked(show_close_button_bool);
            my_file->close_button->setVisible(show_close_button_bool);
            my_file->file_open_way_process = file_open_way_process;
            my_file->file_open_info_process = file_open_info_process;
            my_file->file_open_path_process = file_open_path_process;
        }
    }
}
void Desktop_Main::dragEnterEvent(QDragEnterEvent *event)
{
    if (*allow_drop && event->mimeData()->hasUrls())
    {
        event->accept();
    }
}
void Desktop_Main::set_Desktop_Size(int d_width, int d_height)
{
    desktop_width = d_width;
    desktop_height = d_height;
}
void Desktop_Main::Update_Widget()
{
    move(0,0);
    resize(desktop_width,desktop_height);
    Basic_Desktop *basic_desktop = new Basic_Desktop(this, desktop_width, desktop_height);
    desktop_core_dock_list<<basic_desktop;
    Desktop_NUmber = desktop_core_dock_list.count();
    Desktop_Main::Update_Basic_Desktop();
    control_Dock->set_Desktop_Size(desktop_width ,desktop_height);
    control_Dock->Set_Dock_Show_Bool(&allow_dock_show);
    control_Dock->set_locking_desktop(&locking_desktop);
    control_Dock->Set_Desktop_Number(&Desktop_NUmber);
    control_Dock->Set_Now_page(&now_page);
    control_Dock->First_Set();
    control_Dock->raise();
}
void Desktop_Main::geometry_change()
{
    move(0, 0);
    resize(desktop_width,desktop_height);
    for (Basic_Desktop *basic_desktop : desktop_core_dock_list)
    {
        basic_desktop->resize(desktop_width, desktop_height);
    }
    Desktop_Main::Update_Basic_Desktop();
    control_Dock->set_Desktop_Size(desktop_width ,desktop_height);
    control_Dock->raise();
}
void Desktop_Main::update_for_lineedit(QColor theme1, QColor theme2, QColor theme3)
{
    for (My_LineEdit *lineedit : my_lineedit_list)
    {
        lineedit->update_style(theme1, theme2, theme3);
    }
}
void Desktop_Main::wheelEvent(QWheelEvent *event)
{
    if (locking_desktop)
    {
        return;
    }
    if (event->angleDelta().y() != 0)
    {
        int angle = event->angleDelta().ry();
        if (angle > 0)
        {
            if (now_page <= 0)
            {
                return;
            }
            now_page--;
            move_Timer->stop();
            control_Dock->raise();
            desktop_move_x += desktop_width;
            Desktop_Main::Call_Timer_Move();
        }
        else
        {
            if (now_page >= Desktop_NUmber - 1)
            {
                return;
            }
            now_page++;
            move_Timer->stop();
            control_Dock->raise();
            desktop_move_x -= desktop_width;
            Desktop_Main::Call_Timer_Move();
        }
        control_Dock->Update_Widget();
    }
    event->accept();
}
void Desktop_Main::desktop_Move_Update(int delta_move)
{
    move_Timer->stop();
    control_Dock->raise();
    desktop_move_x -= delta_move * desktop_width;
    Desktop_Main::Call_Timer_Move();
}
void Desktop_Main::Call_Timer_Move()
{
    move_Timer->stop();
    timer_move_x = desktop_move_x;
    run_time = 0;
    for (int i = 0; i < desktop_core_dock_list.count(); i++)
    {
        desktop_core_dock_list[i]->show();
    }
    move_Timer->start();
}
void Desktop_Main::Timer_End()
{
    if (run_time >= 20)
    {
        move_Timer->stop();
        Desktop_Main::Update_Basic_Desktop();
        return;
    }
    run_time++;
    int move_x;
    move_x = timer_move_x / 20;
    desktop_move_x -= move_x;
    for (int i = 0; i < desktop_core_dock_list.count(); i++)
    {
        desktop_core_dock_list[i]->move(desktop_core_dock_list[i]->x() + move_x, 0);
    }
}
void Desktop_Main::Update_Basic_Desktop()
{
    for (int i = 0; i < desktop_core_dock_list.count(); i++)
    {
        desktop_core_dock_list[i]->move((i - now_page) * desktop_width, 0);
        if (i == now_page)
        {
            desktop_core_dock_list[i]->show();
        }
        else
        {
            desktop_core_dock_list[i]->hide();
        }
    }
}
void Desktop_Main::save(QString path)
{
    QSettings settings(path, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    settings.clear();
    desktop_background->save(&settings);
    //
    settings.beginGroup("Desktop");
    settings.setValue("locking_desktop", locking_desktop);
    settings.setValue("allow_dock_show", allow_dock_show);
    settings.setValue("background_playing", desktop_background->Get_Playing_State());
    settings.setValue("mouse_moving", desktop_background->Get_Mouse_Playing_State());
    settings.setValue("desktop_number", Desktop_NUmber);
    settings.setValue("now_page", now_page);
    settings.setValue("control_Dock_pos", control_Dock->pos());
    settings.setValue("file_widget_list_count", file_widget_list.count());
    settings.setValue("my_clock_list_count", my_clock_list.count());
    settings.setValue("my_label_list_count", my_label_list.count());
    settings.setValue("my_lineedit_list_count", my_lineedit_list.count());
    settings.setValue("my_process_carrier_list_count", my_process_carrier_list.count());
    settings.setValue("my_program_container_list_count", my_program_container_list.count());
    settings.setValue("process_widget_list_count", process_widget_list.count());
    settings.setValue("cpu_chart_list_count", cpu_chart_list.count());
    settings.setValue("ram_chart_list_count", ram_chart_list.count());
    settings.setValue("net_chart_list_count", net_chart_list.count());
    settings.setValue("disk_chart_list_count", disk_chart_list.count());
    settings.setValue("pulseaudio_chart_list_count", pulseaudio_chart_list.count());
    settings.setValue("file_tree_list_count", file_tree_list.count());
    settings.setValue("stay_on_top", *stay_on_top);
    settings.setValue("on_top_time", *on_top_time);
    settings.setValue("keyscan_timer", *keyscan_timer);
    settings.setValue("allow_drop", *allow_drop);
    settings.setValue("file_open_way_process", *file_open_way_process);
    settings.setValue("file_open_path_process", *file_open_path_process);
    settings.setValue("file_open_info_process", *file_open_info_process);
    settings.setValue("theme_color", theme_color->rgba());
    settings.setValue("theme_background_color", theme_background_color->rgba());
    settings.setValue("theme_text_color", theme_text_color->rgba());
    settings.endGroup();
    slider_action->save(&settings);
    //
    int count = my_process_carrier_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("my_process_carrier%1").arg(i));
        my_process_carrier_list[i]->save_index_number = i;
        my_process_carrier_list[i]->save(&settings);
        settings.endGroup();
    }
    count = file_widget_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("file_widget%1").arg(i));
        file_widget_list[i]->save(&settings);
        settings.endGroup();
    }
    count = my_clock_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("my_clock%1").arg(i));
        my_clock_list[i]->save(&settings);
        settings.endGroup();
    }
    count = my_label_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("my_label%1").arg(i));
        my_label_list[i]->save(&settings);
        settings.endGroup();
    }
    count = my_lineedit_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("my_lineedit%1").arg(i));
        my_lineedit_list[i]->save(&settings);
        settings.endGroup();
    }
    count = my_program_container_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("my_program_container%1").arg(i));
        my_program_container_list[i]->save(&settings);
        settings.endGroup();
    }
    count = process_widget_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("process_widget%1").arg(i));
        process_widget_list[i]->save(&settings);
        settings.endGroup();
    }
    count = cpu_chart_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("cpu_chart%1").arg(i));
        cpu_chart_list[i]->save(&settings);
        settings.endGroup();
    }
    count = ram_chart_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("ram_chart%1").arg(i));
        ram_chart_list[i]->save(&settings);
        settings.endGroup();
    }
    count = net_chart_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("net_chart%1").arg(i));
        net_chart_list[i]->save(&settings);
        settings.endGroup();
    }
    count = disk_chart_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("disk_chart%1").arg(i));
        disk_chart_list[i]->save(&settings);
        settings.endGroup();
    }
    count = pulseaudio_chart_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("pulseaudio_chart%1").arg(i));
        pulseaudio_chart_list[i]->save(&settings);
        settings.endGroup();
    }
    count = file_tree_list.count();
    for (int i = 0; i < count; i++)
    {
        settings.beginGroup(QString("file_tree%1").arg(i));
        file_tree_list[i]->save(&settings);
        settings.endGroup();
    }
    //
    settings.sync();
}
void Desktop_Main::save()
{
    Desktop_Main::save(load_path);
}
void Desktop_Main::load()
{
    QSettings settings(load_path, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    desktop_background->load(&settings);
    setting_widget->Table_Update();
    settings.beginGroup("Desktop");
    locking_desktop = settings.value("locking_desktop", false).toBool();
    allow_dock_show = settings.value("allow_dock_show", true).toBool();
    control_Dock->setVisible(allow_dock_show);
    *stay_on_top = settings.value("stay_on_top", true).toBool();
    *on_top_time = settings.value("on_top_time", 5000).toInt();
    *keyscan_timer = settings.value("keyscan_timer", 20).toInt();
    *allow_drop = settings.value("allow_drop", true).toBool();
    *file_open_way_process = settings.value("file_open_way_process", "dde-file-manager -d -o").toString();
    *file_open_path_process = settings.value("file_open_path_process", "dde-file-manager --show-item").toString();
    *file_open_info_process = settings.value("file_open_info_process", "dde-file-manager -p").toString();
    *theme_color = QColor::fromRgba(settings.value("theme_color", QColor(0,129,255,255).rgba()).toUInt());
    *theme_background_color = QColor::fromRgba(settings.value("theme_background_color", QColor(255,255,255,75).rgba()).toUInt());
    *theme_text_color = QColor::fromRgba(settings.value("theme_text_color", QColor(0,0,0,255).rgba()).toUInt());
    emit keyscan_loaded();
    bool background_playing = settings.value("background_playing", true).toBool();
    bool mouse_moving = settings.value("mouse_moving", true).toBool();
    if (background_playing)
    {
        desktop_background->Play();
    }
    else
    {
        desktop_background->Pause();
    }
    if (mouse_moving)
    {
        desktop_background->Mouse_Play();
    }
    else
    {
        desktop_background->Mouse_Pause();
    }
    Desktop_NUmber = settings.value("desktop_number", 0).toInt();
    int desktop_core_dock_list_count = desktop_core_dock_list.count();
    for (int i = 0; i <desktop_core_dock_list_count; i++)
    {
        desktop_core_dock_list[0]->~Basic_Desktop();
        desktop_core_dock_list.removeAt(0);
    }
    for (int i = 0; i < Desktop_NUmber; i++)
    {
        Basic_Desktop *basic_desktop = new Basic_Desktop(this, desktop_width, desktop_height);
        desktop_core_dock_list<<basic_desktop;
    }
    if (desktop_core_dock_list.count() == 0)
    {
        Basic_Desktop *basic_desktop = new Basic_Desktop(this, desktop_width, desktop_height);
        desktop_core_dock_list<<basic_desktop;
    }
    now_page = settings.value("now_page", 0).toInt();
    if (now_page < 0)
    {
        now_page = 0;
    }
    if (now_page >= desktop_core_dock_list.count())
    {
        now_page = desktop_core_dock_list.count() - 1;
    }
    Update_Basic_Desktop();
    control_Dock->Update_Widget();
    control_Dock->move(settings.value("control_Dock_pos", control_Dock->pos()).toPoint());
    control_Dock->raise();
    int file_widget_list_count = settings.value("file_widget_list_count", 0).toInt();
    int my_clock_list_count = settings.value("my_clock_list_count", 0).toInt();
    int my_label_list_count = settings.value("my_label_list_count", 0).toInt();
    int my_lineedit_list_count = settings.value("my_lineedit_list_count", 0).toInt();
    int my_process_carrier_list_count = settings.value("my_process_carrier_list_count", 0).toInt();
    int my_program_container_list_count = settings.value("my_program_container_list_count", 0).toInt();
    int cpu_chart_list_count = settings.value("cpu_chart_list_count", 0).toInt();
    int ram_chart_list_count = settings.value("ram_chart_list_count", 0).toInt();
    int net_chart_list_count = settings.value("net_chart_list_count", 0).toInt();
    int disk_chart_list_count = settings.value("disk_chart_list_count", 0).toInt();
    int pulseaudio_chart_list_count = settings.value("pulseaudio_chart_list_count", 0).toInt();
    int process_widget_list_count = settings.value("process_widget_list_count", 0).toInt();
    int file_tree_list_count = settings.value("file_tree_list_count", 0).toInt();
    settings.endGroup();
    slider_action->load(&settings);
    file_widget_list.clear();
    my_clock_list.clear();
    my_label_list.clear();
    my_lineedit_list.clear();
    my_process_carrier_list.clear();
    my_program_container_list.clear();
    process_widget_list.clear();
    file_tree_list.clear();
    cpu_chart_list.clear();
    ram_chart_list.clear();
    net_chart_list.clear();
    disk_chart_list.clear();
    pulseaudio_chart_list.clear();
    for (int i = 0; i < my_clock_list_count; i++)
    {
        My_Clock *my_clock = new My_Clock(desktop_core_dock_list[now_page]);
        my_clock->set_now_page(&now_page);
        my_clock->set_desktop_number(&Desktop_NUmber);
        my_clock->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_clock->move(0, 0);
        my_clock_list.append(my_clock);
        my_clock->my_clock_list = &my_clock_list;
        settings.beginGroup(QString("my_clock%1").arg(i));
        my_clock->load(&settings);
        settings.endGroup();
        my_clock->show();
    }
    for (int i = 0; i < my_lineedit_list_count; i++)
    {
        My_LineEdit *my_lineedit = new My_LineEdit(desktop_core_dock_list[now_page]);
        my_lineedit->set_now_page(&now_page);
        my_lineedit->set_desktop_number(&Desktop_NUmber);
        my_lineedit->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_lineedit->set_WinId(m_WinId);
        my_lineedit->move(0, 0);
        my_lineedit_list.append(my_lineedit);
        my_lineedit->my_lineedit_list = &my_lineedit_list;
        settings.beginGroup(QString("my_lineedit%1").arg(i));
        my_lineedit->load(&settings);
        settings.endGroup();
        my_lineedit->show();
    }
    for (int i = 0; i < my_label_list_count; i++)
    {
        My_Label *my_label = new My_Label(desktop_core_dock_list[now_page]);
        my_label->set_now_page(&now_page);
        my_label->set_desktop_number(&Desktop_NUmber);
        my_label->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_label->set_WinId(m_WinId);
        my_label->move(0, 0);
        my_label_list.append(my_label);
        my_label->my_label_list = &my_label_list;
        settings.beginGroup(QString("my_label%1").arg(i));
        my_label->load(&settings);
        settings.endGroup();
        my_label->show();
    }
    for (int i = 0; i < my_program_container_list_count; i++)
    {
        My_Program_Container *my_program_Carrier = new My_Program_Container(desktop_core_dock_list[now_page]);
        my_program_Carrier->set_now_page(&now_page);
        my_program_Carrier->set_desktop_number(&Desktop_NUmber);
        my_program_Carrier->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_program_Carrier->move(0, 0);
        my_program_container_list.append(my_program_Carrier);
        my_program_Carrier->my_program_container_list = &my_program_container_list;
        settings.beginGroup(QString("my_program_container%1").arg(i));
        my_program_Carrier->load(&settings);
        settings.endGroup();
        my_program_Carrier->show();
    }
    for (int i = 0; i < my_process_carrier_list_count; i++)
    {
        My_Process_Carrier *my_process_Carrier = new My_Process_Carrier(desktop_core_dock_list[now_page]);
        my_process_Carrier->set_now_page(&now_page);
        my_process_Carrier->set_desktop_number(&Desktop_NUmber);
        my_process_Carrier->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_process_Carrier->m_WinId = m_WinId;
        my_process_Carrier->process_widget_p = &process_widget_p;
        my_process_Carrier->move(0, 0);
        my_process_carrier_list.append(my_process_Carrier);
        my_process_Carrier->my_process_carrier_list = &my_process_carrier_list;
        my_process_Carrier->process_widget_list = &process_widget_list;
        my_process_Carrier->file_widget_list = &file_widget_list;
        my_process_Carrier->m_allow_drop = allow_drop;
        my_process_Carrier->file_open_way_process = file_open_way_process;
        my_process_Carrier->file_open_info_process = file_open_info_process;
        my_process_Carrier->file_open_path_process = file_open_path_process;
        settings.beginGroup(QString("my_process_carrier%1").arg(i));
        my_process_Carrier->load(&settings);
        settings.endGroup();
        my_process_Carrier->show();
    }
    for (int i = 0; i < process_widget_list_count; i++)
    {
        Process_Widget *my_process = new Process_Widget(desktop_core_dock_list[now_page]);
        connect(my_process, &Process_Widget::set_to_Carrier_Sig, this, [=]
        {
            process_widget_p = my_process;
        });
        connect(my_process, &Process_Widget::move_To_Desktop_Sig, this, [=](QPoint pos_)
        {
            my_process->in_carrier = false;
            my_process->setParent(desktop_core_dock_list[now_page]);
            my_process->set_now_page(&now_page);
            my_process->set_desktop_number(&Desktop_NUmber);
            my_process->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
            my_process->set_WinId(m_WinId);
            my_process->move(pos_);
            my_process->show();
        });
        //
        settings.beginGroup(QString("process_widget%1").arg(i));
        my_process->in_carrier = settings.value("in_carrier", false).toBool();
        if (my_process->in_carrier)
        {
            my_process->carrier_index = settings.value("carrier_index", -1).toInt();
        }
        settings.endGroup();
        if (my_process->in_carrier && my_process->carrier_index >= 0)
        {
            my_process->setParent(my_process_carrier_list[my_process->carrier_index]->carrier_widget_list[0]);
            my_process->set_now_page(&my_process_carrier_list[my_process->carrier_index]->carrier_now_page);
            my_process->set_desktop_number(&my_process_carrier_list[my_process->carrier_index]->carrier_page_number);
            my_process->set_basic_list(&my_process_carrier_list[my_process->carrier_index]->carrier_widget_list);
        }
        else
        {
            my_process->set_now_page(&now_page);
            my_process->set_desktop_number(&Desktop_NUmber);
            my_process->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        }
        my_process->set_WinId(m_WinId);
        my_process->move(0, 0);
        process_widget_list.append(my_process);
        my_process->process_widget_list = &process_widget_list;
        settings.beginGroup(QString("process_widget%1").arg(i));
        my_process->load(&settings);
        settings.endGroup();
        my_process->show();
    }
    for (int i = 0; i < file_widget_list_count; i++)
    {
        File_Widget *my_file = new File_Widget(desktop_core_dock_list[now_page]);
        connect(my_file, &Process_Widget::set_to_Carrier_Sig, this, [=]
        {
            process_widget_p = my_file;
        });
        connect(my_file, &Process_Widget::move_To_Desktop_Sig, this, [=](QPoint pos_)
        {
            my_file->in_carrier = false;
            my_file->setParent(desktop_core_dock_list[now_page]);
            my_file->set_now_page(&now_page);
            my_file->set_desktop_number(&Desktop_NUmber);
            my_file->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
            my_file->set_WinId(m_WinId);
            my_file->move(pos_);
            my_file->show();
        });
        settings.beginGroup(QString("file_widget%1").arg(i));
        my_file->in_carrier = settings.value("in_carrier", false).toBool();
        if (my_file->in_carrier)
        {
            my_file->carrier_index = settings.value("carrier_index", -1).toInt();
        }
        settings.endGroup();
        if (my_file->in_carrier && my_file->carrier_index >= 0)
        {
            my_file->setParent(my_process_carrier_list[my_file->carrier_index]->carrier_widget_list[0]);
            my_file->set_now_page(&my_process_carrier_list[my_file->carrier_index]->carrier_now_page);
            my_file->set_desktop_number(&my_process_carrier_list[my_file->carrier_index]->carrier_page_number);
            my_file->set_basic_list(&my_process_carrier_list[my_file->carrier_index]->carrier_widget_list);
        }
        else
        {
            my_file->set_now_page(&now_page);
            my_file->set_desktop_number(&Desktop_NUmber);
            my_file->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        }
        my_file->set_WinId(m_WinId);
        my_file->move(0, 0);
        file_widget_list.append(my_file);
        my_file->file_widget_list = &file_widget_list;
        my_file->file_open_way_process = file_open_way_process;
        my_file->file_open_info_process = file_open_info_process;
        my_file->file_open_path_process = file_open_path_process;
        settings.beginGroup(QString("file_widget%1").arg(i));
        my_file->load(&settings);
        settings.endGroup();
        my_file->show();
    }
    for (int i = 0; i < cpu_chart_list_count; i++)
    {
        CPU_Chart *cpu_chart = new CPU_Chart(desktop_core_dock_list[now_page]);
        cpu_chart->set_now_page(&now_page);
        cpu_chart->set_desktop_number(&Desktop_NUmber);
        cpu_chart->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        cpu_chart->move(0, 0);
        cpu_chart_list.append(cpu_chart);
        cpu_chart->cpu_chart_list = &cpu_chart_list;
        settings.beginGroup(QString("cpu_chart%1").arg(i));
        cpu_chart->load(&settings);
        settings.endGroup();
        cpu_chart->show();
    }
    for (int i = 0; i < ram_chart_list_count; i++)
    {
        RAM_Chart *ram_chart = new RAM_Chart(desktop_core_dock_list[now_page]);
        ram_chart->set_now_page(&now_page);
        ram_chart->set_desktop_number(&Desktop_NUmber);
        ram_chart->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        ram_chart->move(0, 0);
        ram_chart_list.append(ram_chart);
        ram_chart->ram_chart_list = &ram_chart_list;
        settings.beginGroup(QString("ram_chart%1").arg(i));
        ram_chart->load(&settings);
        settings.endGroup();
        ram_chart->show();
    }
    for (int i = 0; i < net_chart_list_count; i++)
    {
        NET_Chart *net_chart = new NET_Chart(desktop_core_dock_list[now_page]);
        net_chart->set_now_page(&now_page);
        net_chart->set_desktop_number(&Desktop_NUmber);
        net_chart->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        net_chart->move(0, 0);
        net_chart_list.append(net_chart);
        net_chart->net_chart_list = &net_chart_list;
        settings.beginGroup(QString("net_chart%1").arg(i));
        net_chart->load(&settings);
        settings.endGroup();
        net_chart->show();
    }
    for (int i = 0; i < disk_chart_list_count; i++)
    {
        DISK_Chart *disk_chart = new DISK_Chart(desktop_core_dock_list[now_page]);
        disk_chart->set_now_page(&now_page);
        disk_chart->set_desktop_number(&Desktop_NUmber);
        disk_chart->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        disk_chart->move(0, 0);
        disk_chart_list.append(disk_chart);
        disk_chart->disk_chart_list = &disk_chart_list;
        settings.beginGroup(QString("disk_chart%1").arg(i));
        disk_chart->load(&settings);
        settings.endGroup();
        disk_chart->show();
    }
    for (int i = 0; i < pulseaudio_chart_list_count; i++)
    {
        PulseAudio_Chart *pulseaudio_chart = new PulseAudio_Chart(desktop_core_dock_list[now_page]);
        pulseaudio_chart->set_now_page(&now_page);
        pulseaudio_chart->set_desktop_number(&Desktop_NUmber);
        pulseaudio_chart->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        pulseaudio_chart->move(0, 0);
        pulseaudio_chart_list.append(pulseaudio_chart);
        pulseaudio_chart->pulseaudio_chart_list = &pulseaudio_chart_list;
        settings.beginGroup(QString("pulseaudio_chart%1").arg(i));
        pulseaudio_chart->load(&settings);
        settings.endGroup();
        pulseaudio_chart->show();
    }
    for (int i = 0; i < file_tree_list_count; i++)
    {
        File_Tree *file_tree = new File_Tree(desktop_core_dock_list[now_page]);
        file_tree->set_now_page(&now_page);
        file_tree->set_desktop_number(&Desktop_NUmber);
        file_tree->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        file_tree->m_WinId = m_WinId;
        file_tree->move(0, 0);
        file_tree_list.append(file_tree);
        file_tree->file_tree_list = &file_tree_list;
        file_tree->file_open_way_process = file_open_way_process;
        file_tree->file_open_info_process = file_open_info_process;
        file_tree->file_open_path_process = file_open_path_process;
        file_tree->m_allow_drop = allow_drop;
        settings.beginGroup(QString("file_tree%1").arg(i));
        file_tree->load(&settings);
        settings.endGroup();
        file_tree->show();
    }
}
