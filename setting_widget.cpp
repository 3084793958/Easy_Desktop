#include "setting_widget.h"
#include <QComboBox>
#include "core/tools/my_x11_libs.h"
Setting_Widget::Setting_Widget(QWidget *parent)
    :QWidget(parent)
{
    this->setAcceptDrops(true);
    this->setMinimumSize(600, 300);
    setWindowTitle(tr("壁纸设置"));
    resize(600,400);
    table_widget->resize(600,300);
    table_widget->setColumnCount(15);
    table_widget->setHorizontalHeaderLabels({tr("ID"),tr("名称"),tr("显示方式"),tr("路径"),tr("缩放方式"),tr("居中"),tr("鼠标效果"),tr("鼠标效果宽度系数"),tr("鼠标效果高度系数"),tr("X轴偏移量"),tr("Y轴偏移量"),tr("抗锯齿"),tr("鼠标控制类型"),tr("自定宽"),tr("自定高")});
    table_widget->setAlternatingRowColors(true);
    update_button->move(540,360);
    connect(update_button, &QPushButton::pressed, this, &Setting_Widget::List_Update);
    sort_button->move(480,360);
    connect(sort_button, &QPushButton::pressed, this, [=]
    {
        path_list.clear();
        for (int i = 0; i < id_box_list.count(); i++)
        {
            path_list<<Path_Info(
                           static_cast<uint>(id_box_list[i]->value()),
                           name_box_list[i]->text(),
                           !image_box_list[i]->currentIndex(),
                           path_box_list[i]->text(),
                           static_cast<Scale_Type>(scale_box_list[i]->currentIndex()),
                           !center_box_list[i]->currentIndex(),
                           !mouse_effect_box_list[i]->currentIndex(),
                           mouse_width_box_list[i]->value(),
                           mouse_height_box_list[i]->value(),
                           delta_x_box_list[i]->value(),
                           delta_y_box_list[i]->value(),
                           !on_Antialiasing_box_list[i]->currentIndex(),
                           static_cast<Mouse_Control_Type>(mouse_control_type_box_list[i]->currentIndex()),
                           wallpaper_width_box_list[i]->value(),
                           wallpaper_height_box_list[i]->value()
                           );
        }
        path_list.Sort();
        int id_number = id_box_list.count();
        for (int i = 0; i < id_number; i++)
        {
            id_box_list[i]->setValue(static_cast<int>(path_list[i].id));
            name_box_list[i]->setText(path_list[i].name);
            image_box_list[i]->setCurrentIndex(!path_list[i].is_image);
            path_box_list[i]->setText(path_list[i].path);
            int index;
            switch (path_list[i].scale_type)
            {
            case Scale_Type::No:
            {
                index = 0;
                break;
            }
            case Scale_Type::Each:
            {
                index = 1;
                break;
            }
            case Scale_Type::Width:
            {
                index = 2;
                break;
            }
            case Scale_Type::Height:
            {
                index = 3;
                break;
            }
            case Scale_Type::Short:
            {
                index = 4;
                break;
            }
            case Scale_Type::Long:
            {
                index = 5;
                break;
            }
            case Scale_Type::Full:
            {
                index = 6;
                break;
            }
            case Scale_Type::User:
            {
                index = 7;
                break;
            }
            }
            scale_box_list[i]->setCurrentIndex(index);
            center_box_list[i]->setCurrentIndex(!path_list[i].center);
            mouse_effect_box_list[i]->setCurrentIndex(!path_list[i].mouse_effect);
            mouse_width_box_list[i]->setValue(path_list[i].k_mouse_move_width);
            mouse_height_box_list[i]->setValue(path_list[i].k_mouse_move_height);
            delta_x_box_list[i]->setValue(path_list[i].delta_x);
            delta_y_box_list[i]->setValue(path_list[i].delta_y);
            on_Antialiasing_box_list[i]->setCurrentIndex(!path_list[i].on_Antialiasing);
            index = 0;
            switch (path_list[i].mouse_control_type)
            {
            case Mouse_Control_Type::Follow_Desktop:
            {
                index = 0;
                break;
            }
            case Mouse_Control_Type::Follow_Wallpaper:
            {
                index = 1;
                break;
            }
            }
            mouse_control_type_box_list[i]->setCurrentIndex(index);
            wallpaper_width_box_list[i]->setValue(path_list[i].wallpaper_width);
            wallpaper_height_box_list[i]->setValue(path_list[i].wallpaper_height);
        }
    });
    load_button->move(420,360);
    connect(load_button, &QPushButton::pressed, this, &Setting_Widget::Table_Update);
    new_button->move(480, 320);
    delete_button->move(540, 320);
    connect(new_button, &QPushButton::pressed, this, [=]
    {
        path_list.clear();
        for (int i = 0; i < id_box_list.count(); i++)
        {
            path_list<<Path_Info(
                           static_cast<uint>(id_box_list[i]->value()),
                           name_box_list[i]->text(),
                           !image_box_list[i]->currentIndex(),
                           path_box_list[i]->text(),
                           static_cast<Scale_Type>(scale_box_list[i]->currentIndex()),
                           !center_box_list[i]->currentIndex(),
                           !mouse_effect_box_list[i]->currentIndex(),
                           mouse_width_box_list[i]->value(),
                           mouse_height_box_list[i]->value(),
                           delta_x_box_list[i]->value(),
                           delta_y_box_list[i]->value(),
                           !on_Antialiasing_box_list[i]->currentIndex(),
                           static_cast<Mouse_Control_Type>(mouse_control_type_box_list[i]->currentIndex()),
                           wallpaper_width_box_list[i]->value(),
                           wallpaper_height_box_list[i]->value()
                           );
        }
        uint max_id = 0;
        for (int i = 0; i < path_list.count(); i++)
        {
            if (max_id <= path_list[i].id)
            {
                max_id = path_list[i].id + 1;
            }
        }
        path_list<<Path_Info(max_id,"",true,"",Scale_Type::Full,true,false,0,0,0,0,true);
        Setting_Widget::private_update();
    });
    connect(delete_button, &QPushButton::released, this, [=]
    {
        path_list.clear();
        for (int i = 0; i < id_box_list.count(); i++)
        {
            path_list<<Path_Info(
                           static_cast<uint>(id_box_list[i]->value()),
                           name_box_list[i]->text(),
                           !image_box_list[i]->currentIndex(),
                           path_box_list[i]->text(),
                           static_cast<Scale_Type>(scale_box_list[i]->currentIndex()),
                           !center_box_list[i]->currentIndex(),
                           !mouse_effect_box_list[i]->currentIndex(),
                           mouse_width_box_list[i]->value(),
                           mouse_height_box_list[i]->value(),
                           delta_x_box_list[i]->value(),
                           delta_y_box_list[i]->value(),
                           !on_Antialiasing_box_list[i]->currentIndex(),
                           static_cast<Mouse_Control_Type>(mouse_control_type_box_list[i]->currentIndex()),
                           wallpaper_width_box_list[i]->value(),
                           wallpaper_height_box_list[i]->value()
                           );
        }
        bool get_ok = false;
        uint delete_id = static_cast<uint>(QInputDialog::getInt(this,tr("删除"),tr("请输入删除id(重复id删除第一个):"),
                                                                static_cast<int>(path_list[table_widget->currentRow()].id),0,2147483647,1,
                                                                &get_ok));
        if (!get_ok)
        {
            return;
        }
        for (int i = 0; i <path_list.count(); i++)
        {
            if (path_list[i].id == delete_id)
            {
                path_list.removeAt(i);
                break;
            }
        }
        if (path_list.isEmpty())
        {
            path_list<<Path_Info(0,"",true,"",Scale_Type::Full,true,false,0,0,0,0,true);
        }
        Setting_Widget::private_update();
    });
    choose_id_text->move(0, 365);
    choose_id_box->setRange(0, 2147483647);
    choose_id_box->resize(140,30);
    choose_id_box->move(50, 360);
    set_file_button->move(200, 360);
    connect(set_file_button, &QPushButton::released, this, [=]
    {
        if (path_box_list.count() == 0)
        {
            return;
        }
        int index;
        index = this->Get_id_to_Index(choose_id_box->value());
        if (index >= path_box_list.count())
        {
            index = path_box_list.count() - 1;
        }
        if (index < 0)
        {
            index = 0;
        }
        if (index >= path_box_list.count())
        {
            return;
        }
        QString file_name = QFileDialog::getOpenFileName(nullptr, tr("获取文件"), QDir::homePath(), tr("所有文件") + "(*.*)");
        My_X11_Libs::X11_Raise();//没办法,要跟dde-desktop争夺[桌面显示权]
        path_box_list[index]->setText(file_name);
    });
    load_img_button->move(5, 320);
    load_video_button->move(150, 320);
    connect(load_img_button, &QPushButton::released, this, [=]
    {
        QStringList filenames = QFileDialog::getOpenFileNames(nullptr, tr("获取文件"), QDir::homePath(), tr("图像文件") + "(*.png *.jpg *.jpeg *.svg *.gif *.bmp);;" + tr("所有文件") + "(*.*)");
        My_X11_Libs::X11_Raise();
        path_list.clear();
        for (int i = 0; i < id_box_list.count(); i++)
        {
            path_list<<Path_Info(
                           static_cast<uint>(id_box_list[i]->value()),
                           name_box_list[i]->text(),
                           !image_box_list[i]->currentIndex(),
                           path_box_list[i]->text(),
                           static_cast<Scale_Type>(scale_box_list[i]->currentIndex()),
                           !center_box_list[i]->currentIndex(),
                           !mouse_effect_box_list[i]->currentIndex(),
                           mouse_width_box_list[i]->value(),
                           mouse_height_box_list[i]->value(),
                           delta_x_box_list[i]->value(),
                           delta_y_box_list[i]->value(),
                           !on_Antialiasing_box_list[i]->currentIndex(),
                           static_cast<Mouse_Control_Type>(mouse_control_type_box_list[i]->currentIndex()),
                           wallpaper_width_box_list[i]->value(),
                           wallpaper_height_box_list[i]->value()
                           );
        }
        uint max_id = 0;
        for (int i = 0; i < path_list.count(); i++)
        {
            if (max_id <= path_list[i].id)
            {
                max_id = path_list[i].id + 1;
            }
        }
        for (int i = 0; i < filenames.count(); i++)
        {
            path_list<<Path_Info(max_id + static_cast<unsigned>(i),QFileInfo(filenames[i]).fileName(),true,filenames[i],Scale_Type::Full,true,false,0,0,0,0,true);
        }
        Setting_Widget::private_update();
    });
    connect(load_video_button, &QPushButton::released, this, [=]
    {
        QStringList filenames = QFileDialog::getOpenFileNames(nullptr, tr("获取文件"), QDir::homePath(), tr("视频文件") + "(*.*);;" + tr("所有文件") + "(*.*)");
        My_X11_Libs::X11_Raise();
        path_list.clear();
        for (int i = 0; i < id_box_list.count(); i++)
        {
            path_list<<Path_Info(
                           static_cast<uint>(id_box_list[i]->value()),
                           name_box_list[i]->text(),
                           !image_box_list[i]->currentIndex(),
                           path_box_list[i]->text(),
                           static_cast<Scale_Type>(scale_box_list[i]->currentIndex()),
                           !center_box_list[i]->currentIndex(),
                           !mouse_effect_box_list[i]->currentIndex(),
                           mouse_width_box_list[i]->value(),
                           mouse_height_box_list[i]->value(),
                           delta_x_box_list[i]->value(),
                           delta_y_box_list[i]->value(),
                           !on_Antialiasing_box_list[i]->currentIndex(),
                           static_cast<Mouse_Control_Type>(mouse_control_type_box_list[i]->currentIndex()),
                           wallpaper_width_box_list[i]->value(),
                           wallpaper_height_box_list[i]->value()
                           );
        }
        uint max_id = 0;
        for (int i = 0; i < path_list.count(); i++)
        {
            if (max_id <= path_list[i].id)
            {
                max_id = path_list[i].id + 1;
            }
        }
        for (int i = 0; i < filenames.count(); i++)
        {
            path_list<<Path_Info(max_id + static_cast<unsigned>(i),QFileInfo(filenames[i]).fileName(),false,filenames[i],Scale_Type::Full,true,false,0,0,0,0,true);
        }
        Setting_Widget::private_update();
    });
    table_widget->viewport()->installEventFilter(this);//eventFilter需要注册
}
void Setting_Widget::resizeEvent(QResizeEvent *event)
{
    QSize new_size = event->size();
    table_widget->resize(new_size.width(), new_size.height() - 100);
    update_button->move(new_size.width() - 60, new_size.height() - 40);
    sort_button->move(new_size.width() - 120, new_size.height() - 40);
    load_button->move(new_size.width() - 180, new_size.height() - 40);
    new_button->move(new_size.width() - 120, new_size.height() - 80);
    delete_button->move(new_size.width() - 60, new_size.height() - 80);
    load_img_button->move(5, new_size.height() - 80);
    load_video_button->move(150, new_size.height() - 80);
    choose_id_text->move(0, new_size.height() - 35);
    choose_id_box->move(50, new_size.height() - 40);
    set_file_button->move(200, new_size.height() - 40);
    QWidget::resizeEvent(event);
}
void Setting_Widget::private_update()
{
    if (path_list.count() == 0)
    {
        for (int i = 0; i < id_box_list.count(); i++)
        {
            auto B1 = id_box_list[0];
            id_box_list.removeAt(0);
            delete B1;
            auto B2 = name_box_list[0];
            name_box_list.removeAt(0);
            delete B2;
            auto B3 = image_box_list[0];
            image_box_list.removeAt(0);
            delete B3;
            auto B4 = path_box_list[0];
            path_box_list.removeAt(0);
            delete B4;
            auto B5 = scale_box_list[0];
            scale_box_list.removeAt(0);
            delete B5;
            auto B6 = center_box_list[0];
            center_box_list.removeAt(0);
            delete B6;
            auto B7 = mouse_effect_box_list[0];
            mouse_effect_box_list.removeAt(0);
            delete B7;
            auto B8 = mouse_width_box_list[0];
            mouse_width_box_list.removeAt(0);
            delete B8;
            auto B9 = mouse_height_box_list[0];
            mouse_height_box_list.removeAt(0);
            delete B9;
            auto B10 = delta_x_box_list[0];
            delta_x_box_list.removeAt(0);
            delete B10;
            auto B11 = delta_y_box_list[0];
            delta_y_box_list.removeAt(0);
            delete B11;
            auto B12 = on_Antialiasing_box_list[0];
            on_Antialiasing_box_list.removeAt(0);
            delete B12;
            auto B13 = mouse_control_type_box_list[0];
            mouse_control_type_box_list.removeAt(0);
            delete B13;
            auto B14 = wallpaper_width_box_list[0];
            wallpaper_width_box_list.removeAt(0);
            delete B14;
            auto B15 = wallpaper_height_box_list[0];
            wallpaper_height_box_list.removeAt(0);
            delete B15;
        }
        return;
    }
    if (path_list_index >= path_list.count())
    {
        path_list_index = path_list.count() - 1;
    }
    if (path_list_index < 0)
    {
        path_list_index = 0;
    }
    choose_id_box->setValue(static_cast<int>(path_list[path_list_index].id));
    table_widget->setRowCount(path_list.count());
    int basic_delta = path_list.count() - id_box_list.count();
    int basic_count = id_box_list.count();
    if (basic_delta < 0)
    {
        for (int i = 0; i < -basic_delta; i++)
        {
            auto B1 = id_box_list[path_list.count()];
            id_box_list.removeAt(path_list.count());
            delete B1;
            auto B2 = name_box_list[path_list.count()];
            name_box_list.removeAt(path_list.count());
            delete B2;
            auto B3 = image_box_list[path_list.count()];
            image_box_list.removeAt(path_list.count());
            delete B3;
            auto B4 = path_box_list[path_list.count()];
            path_box_list.removeAt(path_list.count());
            delete B4;
            auto B5 = scale_box_list[path_list.count()];
            scale_box_list.removeAt(path_list.count());
            delete B5;
            auto B6 = center_box_list[path_list.count()];
            center_box_list.removeAt(path_list.count());
            delete B6;
            auto B7 = mouse_effect_box_list[path_list.count()];
            mouse_effect_box_list.removeAt(path_list.count());
            delete B7;
            auto B8 = mouse_width_box_list[path_list.count()];
            mouse_width_box_list.removeAt(path_list.count());
            delete B8;
            auto B9 = mouse_height_box_list[path_list.count()];
            mouse_height_box_list.removeAt(path_list.count());
            delete B9;
            auto B10 = delta_x_box_list[path_list.count()];
            delta_x_box_list.removeAt(path_list.count());
            delete B10;
            auto B11 = delta_y_box_list[path_list.count()];
            delta_y_box_list.removeAt(path_list.count());
            delete B11;
            auto B12 = on_Antialiasing_box_list[path_list.count()];
            on_Antialiasing_box_list.removeAt(path_list.count());
            delete B12;
            auto B13 = mouse_control_type_box_list[path_list.count()];
            mouse_control_type_box_list.removeAt(path_list.count());
            delete B13;
            auto B14 = wallpaper_width_box_list[path_list.count()];
            wallpaper_width_box_list.removeAt(path_list.count());
            delete B14;
            auto B15 = wallpaper_height_box_list[path_list.count()];
            wallpaper_height_box_list.removeAt(path_list.count());
            delete B15;
        }
    }
    else
    {
        for (int i = 0; i < basic_delta; i++)
        {
            QSpinBox *B1 = new QSpinBox();
            QLineEdit *B2 = new QLineEdit();
            QComboBox *B3 = new QComboBox();
            QLineEdit *B4 = new QLineEdit();
            QComboBox *B5 = new QComboBox();
            QComboBox *B6 = new QComboBox();
            QComboBox *B7 = new QComboBox();
            QDoubleSpinBox *B8 = new QDoubleSpinBox();
            QDoubleSpinBox *B9 = new QDoubleSpinBox();
            QSpinBox *B10 = new QSpinBox();
            QSpinBox *B11 = new QSpinBox();
            QComboBox *B12 = new QComboBox();
            QComboBox *B13 = new QComboBox();
            QSpinBox *B14 = new QSpinBox();
            QSpinBox *B15 = new QSpinBox();
            B1->setRange(0, 2147483647);
            B1->setValue(static_cast<int>(path_list[basic_count + i].id));
            B2->setPlaceholderText(tr("输入名称:"));
            B2->setText(path_list[basic_count + i].name);
            B3->addItems({tr("图像"),tr("视频")});
            B3->setCurrentIndex(!path_list[basic_count + i].is_image);
            B4->setPlaceholderText(tr("输入路径:"));
            B4->setText(path_list[basic_count + i].path);
            B5->addItems({tr("不缩放"),tr("全缩放"),tr("宽基准"),tr("高基准"),tr("短基准"),tr("长基准"),tr("饱满"),tr("自定义宽高")});
            int index;
            switch (path_list[basic_count + i].scale_type)
            {
            case Scale_Type::No:
            {
                index = 0;
                break;
            }
            case Scale_Type::Each:
            {
                index = 1;
                break;
            }
            case Scale_Type::Width:
            {
                index = 2;
                break;
            }
            case Scale_Type::Height:
            {
                index = 3;
                break;
            }
            case Scale_Type::Short:
            {
                index = 4;
                break;
            }
            case Scale_Type::Long:
            {
                index = 5;
                break;
            }
            case Scale_Type::Full:
            {
                index = 6;
                break;
            }
            case Scale_Type::User:
            {
                index = 7;
                break;
            }
            }
            B5->setCurrentIndex(index);
            B6->addItems({tr("是"),tr("否")});
            B6->setCurrentIndex(!path_list[basic_count + i].center);
            B7->addItems({tr("是"),tr("否")});
            B7->setCurrentIndex(!path_list[basic_count + i].mouse_effect);
            B8->setRange(-1024.0, 1024.0);
            B8->setValue(path_list[basic_count + i].k_mouse_move_width);
            B9->setRange(-1024.0, 1024.0);
            B9->setValue(path_list[basic_count + i].k_mouse_move_height);
            B10->setRange(-2147483647, 2147483647);
            B10->setValue(path_list[basic_count + i].delta_x);
            B11->setRange(-2147483647, 2147483647);
            B11->setValue(path_list[basic_count + i].delta_y);
            B12->addItems({tr("是"),tr("否")});
            B12->setCurrentIndex(!path_list[basic_count + i].on_Antialiasing);
            B13->addItems({tr("跟随桌面"), tr("跟随壁纸")});
            index = 0;
            switch (path_list[basic_count + i].mouse_control_type)
            {
            case Mouse_Control_Type::Follow_Desktop:
            {
                index = 0;
                break;
            }
            case Mouse_Control_Type::Follow_Wallpaper:
            {
                index = 1;
                break;
            }
            }
            B13->setCurrentIndex(index);
            B14->setRange(-2147483647, 2147483647);
            B14->setValue(path_list[basic_count + i].wallpaper_width);
            B15->setRange(-2147483647, 2147483647);
            B15->setValue(path_list[basic_count + i].wallpaper_height);
            id_box_list.append(B1);
            name_box_list.append(B2);
            image_box_list.append(B3);
            path_box_list.append(B4);
            scale_box_list.append(B5);
            center_box_list.append(B6);
            mouse_effect_box_list.append(B7);
            mouse_width_box_list.append(B8);
            mouse_height_box_list.append(B9);
            delta_x_box_list.append(B10);
            delta_y_box_list.append(B11);
            on_Antialiasing_box_list.append(B12);
            mouse_control_type_box_list.append(B13);
            wallpaper_width_box_list.append(B14);
            wallpaper_height_box_list.append(B15);
        }
    }
    for (int i = 0; i < path_list.count(); i++)
    {
        id_box_list[i]->setValue(static_cast<int>(path_list[i].id));
        name_box_list[i]->setText(path_list[i].name);
        image_box_list[i]->setCurrentIndex(!path_list[i].is_image);
        path_box_list[i]->setText(path_list[i].path);
        int index;
        switch (path_list[i].scale_type)
        {
        case Scale_Type::No:
        {
            index = 0;
            break;
        }
        case Scale_Type::Each:
        {
            index = 1;
            break;
        }
        case Scale_Type::Width:
        {
            index = 2;
            break;
        }
        case Scale_Type::Height:
        {
            index = 3;
            break;
        }
        case Scale_Type::Short:
        {
            index = 4;
            break;
        }
        case Scale_Type::Long:
        {
            index = 5;
            break;
        }
        case Scale_Type::Full:
        {
            index = 6;
            break;
        }
        case Scale_Type::User:
        {
            index = 7;
            break;
        }
        }
        scale_box_list[i]->setCurrentIndex(index);
        center_box_list[i]->setCurrentIndex(!path_list[i].center);
        mouse_effect_box_list[i]->setCurrentIndex(!path_list[i].mouse_effect);
        mouse_width_box_list[i]->setValue(path_list[i].k_mouse_move_width);
        mouse_height_box_list[i]->setValue(path_list[i].k_mouse_move_height);
        delta_x_box_list[i]->setValue(path_list[i].delta_x);
        delta_y_box_list[i]->setValue(path_list[i].delta_y);
        on_Antialiasing_box_list[i]->setCurrentIndex(!path_list[i].on_Antialiasing);
        index = 0;
        switch (path_list[i].mouse_control_type)
        {
        case Mouse_Control_Type::Follow_Desktop:
        {
            index = 0;
            break;
        }
        case Mouse_Control_Type::Follow_Wallpaper:
        {
            index = 1;
            break;
        }
        }
        mouse_control_type_box_list[i]->setCurrentIndex(index);
        wallpaper_width_box_list[i]->setValue(path_list[i].wallpaper_width);
        wallpaper_height_box_list[i]->setValue(path_list[i].wallpaper_height);
    }
    for (int i = 0; i < path_list.count(); i++)
    {
        table_widget->setCellWidget(i, 0, this->id_box_list[i]);
        table_widget->setCellWidget(i, 1, this->name_box_list[i]);
        table_widget->setCellWidget(i, 2, this->image_box_list[i]);
        table_widget->setCellWidget(i, 3, this->path_box_list[i]);
        table_widget->setCellWidget(i, 4, this->scale_box_list[i]);
        table_widget->setCellWidget(i, 5, this->center_box_list[i]);
        table_widget->setCellWidget(i, 6, this->mouse_effect_box_list[i]);
        table_widget->setCellWidget(i, 7, this->mouse_width_box_list[i]);
        table_widget->setCellWidget(i, 8, this->mouse_height_box_list[i]);
        table_widget->setCellWidget(i, 9, this->delta_x_box_list[i]);
        table_widget->setCellWidget(i, 10, this->delta_y_box_list[i]);
        table_widget->setCellWidget(i, 11, this->on_Antialiasing_box_list[i]);
        table_widget->setCellWidget(i, 12, this->mouse_control_type_box_list[i]);
        table_widget->setCellWidget(i, 13, this->wallpaper_width_box_list[i]);
        table_widget->setCellWidget(i, 14, this->wallpaper_height_box_list[i]);
    }
    table_widget->setColumnWidth(0, 145);
    table_widget->setColumnWidth(1, 145);
    table_widget->setColumnWidth(2, 100);
    table_widget->setColumnWidth(3, 300);
    table_widget->setColumnWidth(4, 100);
    table_widget->setColumnWidth(5, 100);
    table_widget->setColumnWidth(6, 100);
    table_widget->setColumnWidth(7, 145);
    table_widget->setColumnWidth(8, 145);
    table_widget->setColumnWidth(9, 145);
    table_widget->setColumnWidth(10, 145);
    table_widget->setColumnWidth(11, 100);
    table_widget->setColumnWidth(12, 145);
    table_widget->setColumnWidth(13, 145);
    table_widget->setColumnWidth(14, 145);
    table_widget->show();
}
void Setting_Widget::Table_Update()
{
    path_list = background_path->path_list;
    path_list_index = background_path->Path_List_Index;
    Setting_Widget::private_update();
}
void Setting_Widget::List_Update()
{
    path_list.clear();
    for (int i = 0; i < id_box_list.count(); i++)
    {
        path_list<<Path_Info(
                       static_cast<uint>(id_box_list[i]->value()),
                       name_box_list[i]->text(),
                       !image_box_list[i]->currentIndex(),
                       path_box_list[i]->text(),
                       static_cast<Scale_Type>(scale_box_list[i]->currentIndex()),
                       !center_box_list[i]->currentIndex(),
                       !mouse_effect_box_list[i]->currentIndex(),
                       mouse_width_box_list[i]->value(),
                       mouse_height_box_list[i]->value(),
                       delta_x_box_list[i]->value(),
                       delta_y_box_list[i]->value(),
                       !on_Antialiasing_box_list[i]->currentIndex(),
                       static_cast<Mouse_Control_Type>(mouse_control_type_box_list[i]->currentIndex()),
                       wallpaper_width_box_list[i]->value(),
                       wallpaper_height_box_list[i]->value()
                       );
    }
    choose_id = choose_id_box->value();
    path_list_index = Get_id_to_Index(choose_id);
    if (path_list.count() == 0 || path_list_index < 0 || path_list_index >= path_list.count())
    {
        return;
    }
    background_path->sending_info = true;
    background_path->path_list = path_list;
    background_path->Path_List_Index = path_list_index;
    background_path->sending_info = false;
    background_path->Update_Widget();
}
void Setting_Widget::set_wallpaper(int wallpaper_id)
{
    choose_id_box->setValue(wallpaper_id);
    List_Update();
}
void Setting_Widget::remove_wallpaper(int wallpaper_id)
{
    path_list.clear();
    for (int i = 0; i < id_box_list.count(); i++)
    {
        path_list<<Path_Info(
                       static_cast<uint>(id_box_list[i]->value()),
                       name_box_list[i]->text(),
                       !image_box_list[i]->currentIndex(),
                       path_box_list[i]->text(),
                       static_cast<Scale_Type>(scale_box_list[i]->currentIndex()),
                       !center_box_list[i]->currentIndex(),
                       !mouse_effect_box_list[i]->currentIndex(),
                       mouse_width_box_list[i]->value(),
                       mouse_height_box_list[i]->value(),
                       delta_x_box_list[i]->value(),
                       delta_y_box_list[i]->value(),
                       !on_Antialiasing_box_list[i]->currentIndex(),
                       static_cast<Mouse_Control_Type>(mouse_control_type_box_list[i]->currentIndex()),
                       wallpaper_width_box_list[i]->value(),
                       wallpaper_height_box_list[i]->value()
                       );
    }
    bool get_ok = false;
    uint delete_id = static_cast<uint>(wallpaper_id);
    if (!get_ok)
    {
        return;
    }
    for (int i = 0; i <path_list.count(); i++)
    {
        if (path_list[i].id == delete_id)
        {
            path_list.removeAt(i);
            break;
        }
    }
    if (path_list.isEmpty())
    {
        path_list<<Path_Info(0,"",true,"",Scale_Type::Full,true,false,0,0,0,0,true);
    }
    Setting_Widget::private_update();
}
void Setting_Widget::add_wallpaper(uint m_id, QString m_name, bool m_is_image, QString m_path, Scale_Type m_scale_type, bool m_center, bool m_mouse_effect,
                                   qreal m_k_mouse_move_width, qreal m_k_mouse_move_height, int m_delta_x, int m_delta_y,
                                   bool m_on_Antialiasing, Mouse_Control_Type m_mouse_control_type, int m_wallpaper_width, int m_wallpaper_height)
{
    path_list.clear();
    for (int i = 0; i < id_box_list.count(); i++)
    {
        path_list<<Path_Info(
                       static_cast<uint>(id_box_list[i]->value()),
                       name_box_list[i]->text(),
                       !image_box_list[i]->currentIndex(),
                       path_box_list[i]->text(),
                       static_cast<Scale_Type>(scale_box_list[i]->currentIndex()),
                       !center_box_list[i]->currentIndex(),
                       !mouse_effect_box_list[i]->currentIndex(),
                       mouse_width_box_list[i]->value(),
                       mouse_height_box_list[i]->value(),
                       delta_x_box_list[i]->value(),
                       delta_y_box_list[i]->value(),
                       !on_Antialiasing_box_list[i]->currentIndex(),
                       static_cast<Mouse_Control_Type>(mouse_control_type_box_list[i]->currentIndex()),
                       wallpaper_width_box_list[i]->value(),
                       wallpaper_height_box_list[i]->value()
                       );
    }
    path_list<<Path_Info(m_id, m_name, m_is_image, m_path, m_scale_type, m_center, m_mouse_effect,
                         m_k_mouse_move_width, m_k_mouse_move_height, m_delta_x, m_delta_y,
                         m_on_Antialiasing, m_mouse_control_type, m_wallpaper_width, m_wallpaper_height);
    Setting_Widget::private_update();
}
int Setting_Widget::Get_id_to_Index(int id)
{
    int result = 0;
    for (int i = 0; i < id_box_list.count(); i++)
    {
        if (id_box_list[i]->value() == id)
        {
            result = i;
            break;
        }
    }
    if (result < 0 || result >= path_list.count())
    {
        result = 0;
    }
    return result;
}
bool Setting_Widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == table_widget->viewport())
    {
        if (event->type() == QEvent::MouseButtonDblClick)
        {
            if (id_box_list.empty()) return QObject::eventFilter(watched, event);
            choose_id_box->setValue(id_box_list[table_widget->currentRow()]->value());
        }
    }
    return QObject::eventFilter(watched, event);
}
void Setting_Widget::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        QMimeDatabase mimeDb;
        QMimeType mimeType;
        for (QUrl url : event->mimeData()->urls())
        {
            mimeType = mimeDb.mimeTypeForUrl(url);
            QString mimeName = mimeType.name();
            if (mimeName.startsWith("image/") || mimeName.startsWith("video/"))
            {
                event->accept();
                return;
            }
        }
    }
}
void Setting_Widget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasUrls())
    {
        path_list.clear();
        for (int i = 0; i < id_box_list.count(); i++)
        {
            path_list<<Path_Info(
                           static_cast<uint>(id_box_list[i]->value()),
                           name_box_list[i]->text(),
                           !image_box_list[i]->currentIndex(),
                           path_box_list[i]->text(),
                           static_cast<Scale_Type>(scale_box_list[i]->currentIndex()),
                           !center_box_list[i]->currentIndex(),
                           !mouse_effect_box_list[i]->currentIndex(),
                           mouse_width_box_list[i]->value(),
                           mouse_height_box_list[i]->value(),
                           delta_x_box_list[i]->value(),
                           delta_y_box_list[i]->value(),
                           !on_Antialiasing_box_list[i]->currentIndex(),
                           static_cast<Mouse_Control_Type>(mouse_control_type_box_list[i]->currentIndex()),
                           wallpaper_width_box_list[i]->value(),
                           wallpaper_height_box_list[i]->value()
                           );
        }
        uint max_id = 0;
        for (int i = 0; i < path_list.count(); i++)
        {
            if (max_id <= path_list[i].id)
            {
                max_id = path_list[i].id + 1;
            }
        }
        QMimeDatabase mimeDb;
        QMimeType mimeType;
        for (QUrl url : event->mimeData()->urls())
        {
            mimeType = mimeDb.mimeTypeForUrl(url);
            QString mimeName = mimeType.name();
            if (mimeName.startsWith("image/") || mimeName.startsWith("video/"))
            {
                path_list<<Path_Info(max_id,url.fileName(),true,url.path(),Scale_Type::Full,true,false,0,0,0,0,true);
                max_id++;
            }
        }
        Setting_Widget::private_update();
    }
}
