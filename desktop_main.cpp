#include "desktop_main.h"
#include <QMessageBox>
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
        move(event->globalPos() - press_point);
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
    background_Add_Action->addAction(my_process_Carrier_action);
    background_Add_Action->addAction(my_program_INNER_action);
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
    menu->addAction(Quit_Action);
    move_Timer->setInterval(30);
    connect(move_Timer, &QTimer::timeout, this, &Desktop_Main::Timer_End);
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
        my_clock->move(event->globalPos());
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
        my_lineedit->move(event->globalPos());
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
        my_label->move(event->globalPos());
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
        my_process->move(event->globalPos());
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
        my_file->move(event->globalPos());
        file_widget_list.append(my_file);
        my_file->file_widget_list = &file_widget_list;
    }
    else if (know_what == my_process_Carrier_action)
    {
        My_Process_Carrier *my_process_Carrier = new My_Process_Carrier(desktop_core_dock_list[now_page]);
        my_process_Carrier->set_now_page(&now_page);
        my_process_Carrier->set_desktop_number(&Desktop_NUmber);
        my_process_Carrier->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_process_Carrier->m_WinId = m_WinId;
        my_process_Carrier->process_widget_p = &process_widget_p;
        my_process_Carrier->move(event->globalPos());
        my_process_carrier_list.append(my_process_Carrier);
        my_process_Carrier->my_process_carrier_list = &my_process_carrier_list;
        my_process_Carrier->process_widget_list = &process_widget_list;
        my_process_Carrier->file_widget_list = &file_widget_list;
    }
    else if (know_what == my_program_INNER_action)
    {
        My_Program_Container *my_program_Carrier = new My_Program_Container(desktop_core_dock_list[now_page]);
        my_program_Carrier->set_now_page(&now_page);
        my_program_Carrier->set_desktop_number(&Desktop_NUmber);
        my_program_Carrier->set_basic_list(reinterpret_cast<QList<QWidget *> *>(&desktop_core_dock_list));
        my_program_Carrier->move(event->globalPos());
        my_program_container_list.append(my_program_Carrier);
        my_program_Carrier->my_program_container_list = &my_program_container_list;
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
void Desktop_Main::save()
{
    QSettings settings(QDir::homePath() + "/.local/lib/easy_desktop/config.ini", QSettings::IniFormat);
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
    //
    settings.sync();
}
void Desktop_Main::load()
{
    QSettings settings(QDir::homePath() + "/.local/lib/easy_desktop/config.ini", QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    desktop_background->load(&settings);
    setting_widget->Table_Update();
    settings.beginGroup("Desktop");
    locking_desktop = settings.value("locking_desktop", false).toBool();
    allow_dock_show = settings.value("allow_dock_show", true).toBool();
    control_Dock->setVisible(allow_dock_show);
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
    int process_widget_list_count = settings.value("process_widget_list_count", 0).toInt();
    settings.endGroup();
    slider_action->load(&settings);
    file_widget_list.clear();
    my_clock_list.clear();
    my_label_list.clear();
    my_lineedit_list.clear();
    my_process_carrier_list.clear();
    my_program_container_list.clear();
    process_widget_list.clear();
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
        settings.beginGroup(QString("file_widget%1").arg(i));
        my_file->load(&settings);
        settings.endGroup();
        my_file->show();
    }
}
