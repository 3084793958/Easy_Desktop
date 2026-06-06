#include "desktop_background.h"
#include <iostream>
#include <fstream>
static bool sort_data(Path_Info list1, Path_Info list2);
QSize Desktop_Background::get_Image_Size(QString path)
{
    QImageReader reader(path);
    if (!reader.canRead())
    {
        return QSize(0,0);
    }
    QSize size = reader.size();
    if (!size.isValid())
    {
        QImage image =reader.read();
        if (!image.isNull())
        {
            size = image.size();
        }
    }
    return size;
}
void Desktop_Background::set_Desktop_Size(int d_width, int d_height)
{
    desktop_width = d_width;
    desktop_height = d_height;
}
void Path_List::Sort()
{
    std::sort(begin(), end(), sort_data);
}
void Desktop_Background::mouse_move_event(int mouse_x, int mouse_y)
{
    if (sending_info)
    {
        return;
    }
    if (!btnCheck->doing)
    {
        return;
    }
    if (use_plugin)
    {
        if (plugin_interface)
        {
            if (plugin_interface->Plugin_Version >= P_Version{0, 0, 1})
            {
                if (plugin_interface->inited)
                {
                    plugin_interface->mouseMoveEvent(mouse_x, mouse_y);
                }
            }
        }
    }
    int move_x;
    int move_y;
    switch (path_list[Path_List_Index].mouse_control_type)
    {
    case Mouse_Control_Type::Follow_Desktop:
    {
        move_x = int(path_list[Path_List_Index].k_mouse_move_width * (mouse_x - (QApplication::desktop()->width() / 2)) * (static_cast<double>(desktop_width) / QApplication::desktop()->width()));
        move_y = int(path_list[Path_List_Index].k_mouse_move_height * (mouse_y - (QApplication::desktop()->height() / 2)) * (static_cast<double>(desktop_height) / QApplication::desktop()->height()));
        break;
    }
    case Mouse_Control_Type::Follow_Wallpaper:
    {
        move_x = static_cast<int>((save_final_size.width() * (1 + abs(path_list[Path_List_Index].k_mouse_move_width)) - desktop_width) / 2 * (static_cast<double>(mouse_x * 2) / QApplication::desktop()->width() - 1) * (path_list[Path_List_Index].k_mouse_move_width >= 0 ? 1 : -1));
        move_y = static_cast<int>((save_final_size.height() * (1 + abs(path_list[Path_List_Index].k_mouse_move_height)) - desktop_height) / 2 * (static_cast<double>(mouse_y * 2) / QApplication::desktop()->height() - 1) * (path_list[Path_List_Index].k_mouse_move_height >= 0 ? 1 : -1));
        break;
    }
    }
    if (path_list[Path_List_Index].is_image)
    {
        image_background->move(base_point.x() + move_x, base_point.y() + move_y);//频繁执行占用低
    }
    else
    {
        graphicsView->move(base_point.x() + move_x, base_point.y() + move_y);
    }
}
void Desktop_Background::Update_Widget()
{
    if (sending_info)
    {
        return;
    }
    updating = true;
    btnCheck->doing = false;
    resize(desktop_width, desktop_height);
    if (path_list.count() == 0)
    {
        return;
    }
    if (Path_List_Index < 0 || Path_List_Index >= path_list.count())
    {
        return;
    }
    if (path_list[Path_List_Index].Empty)
    {
        return;
    }
    if (path_list[Path_List_Index].path.isEmpty())
    {
        return;
    }
    image_movie->stop();
    media_player->stop();
    image_background->hide();
    graphicsView->hide();
    unload_plugin();
    if (path_list[Path_List_Index].path.endsWith(".so"))
    {
        use_plugin = true;
        load_plugin(path_list[Path_List_Index].path);
        if (use_plugin)
        {
            if (plugin_interface)
            {
                if (plugin_interface->Plugin_Version >= P_Version{0, 0, 1})
                {
                    if (plugin_interface->inited)
                    {
                        wallpaper_plugin_item = plugin_interface->wallpaperItem();
                        wallpaper_plugin_item->setParent(this);
                        plugin_interface->sizeChange(this->size());
                        wallpaper_plugin_item->show();
                    }
                }
            }
        }
        return;
    }
    use_plugin = false;
    if (path_list[Path_List_Index].is_image)
    {
        image_movie->setFileName(path_list[Path_List_Index].path);
        Desktop_Background::Second_Update_Widget();
    }
    else
    {
        graphicsView->resize(desktop_width, desktop_height);
        graphicsView->show();//被优化了,必须show才有emit
        media_player->setMedia(nullptr);
        call_video_update = true;
        media_player->setMedia(QUrl::fromLocalFile(path_list[Path_List_Index].path));
        media_player->play();
    }
}
void Desktop_Background::geometry_change()
{
    if (sending_info)
    {
        return;
    }
    resize(desktop_width, desktop_height);
    if (path_list.count() == 0)
    {
        return;
    }
    if (Path_List_Index < 0 || Path_List_Index >= path_list.count())
    {
        return;
    }
    if (path_list[Path_List_Index].Empty)
    {
        return;
    }
    if (path_list[Path_List_Index].path.isEmpty())
    {
        return;
    }
    if (use_plugin)
    {
        if (plugin_interface)
        {
            if (plugin_interface->Plugin_Version >= P_Version{0, 0, 1})
            {
                if (plugin_interface->inited)
                {
                    plugin_interface->sizeChange(QSize(desktop_width, desktop_height));
                }
            }
        }
    }
    if (path_list[Path_List_Index].is_image)
    {
        image_movie->setFileName(path_list[Path_List_Index].path);
    }
    else
    {
        graphicsView->resize(desktop_width, desktop_height);
        graphicsView->show();//被优化了,必须show才有emit
    }
    Desktop_Background::Second_Update_Widget();
}
void Desktop_Background::load_plugin(QString filepath)
{
    if (filepath.isEmpty()) return;
    if (plugin_loader->isLoaded()) unload_plugin();
    plugin_loader->setFileName(filepath);
    if (!plugin_loader->load())
    {
        qDebug() << tr("插件导入失败:") << plugin_loader->errorString();
        use_plugin = false;
        return;
    }
    QObject *pluginInstance = plugin_loader->instance();
    if (!pluginInstance)
    {
        plugin_loader->unload();
        use_plugin = false;
        return;
    }
    plugin_interface = qobject_cast<Ext_Wallpaper_PluginInterface *>(pluginInstance);
    if (plugin_interface)
    {
        if (is_Ext_plugin())
        {
            if (Desktop_Background::Contains_Ext_Plugin(plugin_interface->Ext_Name, "Easy_Desktop_Wallpaper"))
            {
                if (plugin_interface->Plugin_Version >= P_Version{0, 0, 1})
                {
                    plugin_interface->init(path_list[Path_List_Index], send_position_sender);
                    plugin_interface->inited = true;
                    if (theme_color)
                    {
                        plugin_interface->update_style(*theme_color, *theme_background_color, *theme_text_color, *select_text_color, *disabled_text_color, *checked_icon_path);
                    }
                }
            }
        }
    }
}
void Desktop_Background::unload_plugin()
{
    if (!plugin_loader->isLoaded()) return;
    if (!plugin_interface) return;
    if (is_Ext_plugin())
    {
        if (Desktop_Background::Contains_Ext_Plugin(plugin_interface->Ext_Name, "Easy_Desktop_Wallpaper"))
        {
            if (plugin_interface->Plugin_Version >= P_Version{0, 0, 1})
            {
                wallpaper_plugin_item->setParent(nullptr);
                wallpaper_plugin_item->hide();
                wallpaper_plugin_item = nullptr;
                plugin_interface->RemovePlugin();
            }
        }
    }
    plugin_loader->unload();
}
bool Desktop_Background::Contains_Ext_Plugin(QString Ext_name, QString plugin_controller_name)
{
    QStringList ext_list = Ext_name.split("|");
    return ext_list.contains(plugin_controller_name);
}
bool Desktop_Background::is_Ext_plugin()
{
    QJsonObject root_obj = plugin_loader->metaData();
    if (!root_obj.contains("MetaData")) return false;
    QJsonValue meta_value = root_obj.value("MetaData");
    if (!meta_value.isObject()) return false;
    QJsonObject meta_obj = meta_value.toObject();
    return meta_obj.contains("Ext_Wallpaper_Plugin");
}
void Desktop_Background::style_update()
{
    if (use_plugin)
    {
        if (plugin_interface)
        {
            if (plugin_interface->Plugin_Version >= P_Version{0, 0, 1})
            {
                if (plugin_interface->inited)
                {
                    plugin_interface->update_style(*theme_color, *theme_background_color, *theme_text_color, *select_text_color, *disabled_text_color, *checked_icon_path);
                }
            }
        }
    }
}
void Desktop_Background::Second_Update_Widget()
{
    if (sending_info)
    {
        return;
    }
    if (use_plugin)
    {
        return;
    }
    QSize image_size;
    if (path_list[Path_List_Index].is_image)
    {
        image_size = Desktop_Background::get_Image_Size(path_list[Path_List_Index].path);
    }
    else
    {
        image_size = video_item->size().toSize();
    }
    QSize final_size;
    switch (path_list[Path_List_Index].scale_type)
    {
    case Scale_Type::No:
    {
        final_size = image_size;
        break;
    }
    case Scale_Type::Each:
    {
        final_size = QSize(desktop_width, desktop_height);
        break;
    }
    case Scale_Type::Width:
    {
        int final_height = static_cast<int>(image_size.height() * (static_cast<float>(desktop_width) / image_size.width()));
        final_size = QSize(desktop_width, final_height);
        break;
    }
    case Scale_Type::Height:
    {
        int final_width = static_cast<int>(image_size.width() * (static_cast<float>(desktop_height) / image_size.height()));
        final_size = QSize(final_width, desktop_height);
        break;
    }
    case Scale_Type::Short:
    {
        if (image_size.width() < image_size.height())
        {
            int final_height = static_cast<int>(image_size.height() * (static_cast<float>(desktop_width) / image_size.width()));
            final_size = QSize(desktop_width, final_height);
        }
        else
        {
            int final_width = static_cast<int>(image_size.width() * (static_cast<float>(desktop_height) / image_size.height()));
            final_size = QSize(final_width, desktop_height);
        }
        break;
    }
    case Scale_Type::Long:
    {
        if (image_size.width() > image_size.height())
        {
            int final_height = static_cast<int>(image_size.height() * (static_cast<float>(desktop_width) / image_size.width()));
            final_size = QSize(desktop_width, final_height);
        }
        else
        {
            int final_width = static_cast<int>(image_size.width() * (static_cast<float>(desktop_height) / image_size.height()));
            final_size = QSize(final_width, desktop_height);
        }
        break;
    }
    case Scale_Type::Full:
    {
        float R_width = static_cast<float>(desktop_width) / image_size.width();
        float R_height = static_cast<float>(desktop_height) / image_size.height();
        if (R_width > R_height)
        {
            int final_height = static_cast<int>(image_size.height() * (static_cast<float>(desktop_width) / image_size.width()));
            final_size = QSize(desktop_width, final_height);
        }
        else
        {
            int final_width = static_cast<int>(image_size.width() * (static_cast<float>(desktop_height) / image_size.height()));
            final_size = QSize(final_width, desktop_height);
        }
        break;
    }
    case Scale_Type::User:
    {
        final_size = QSize(path_list[Path_List_Index].wallpaper_width, path_list[Path_List_Index].wallpaper_height);
        break;
    }
    }
    save_final_size = final_size;
    if (path_list[Path_List_Index].mouse_effect)
    {
        final_size = QSizeF(final_size.width() * (1 + abs(path_list[Path_List_Index].k_mouse_move_width)),
                            final_size.height() * (1+ abs(path_list[Path_List_Index].k_mouse_move_height))).toSize();
    }
    else
    {
        final_size = QSizeF(final_size.width() + 4,
                            final_size.height() + 4 * (static_cast<qreal>(final_size.height()) / final_size.width())).toSize();
    }
    if (path_list[Path_List_Index].center)
    {
        base_point = QPoint((desktop_width - final_size.width()) / 2, (desktop_height - final_size.height()) / 2);
    }
    else
    {
        base_point = QPoint(0, 0);
    }
    base_point += QPoint(path_list[Path_List_Index].delta_x, path_list[Path_List_Index].delta_y);
    if (!path_list[Path_List_Index].is_image && path_list[Path_List_Index].on_Antialiasing)
    {
        graphicsView->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    }
    else
    {
        graphicsView->setRenderHint(QPainter::Antialiasing, false);
        graphicsView->setRenderHint(QPainter::SmoothPixmapTransform, false);
    }
    if (path_list[Path_List_Index].is_image)
    {
        image_background->move(base_point);
        image_background->resize(final_size);
        image_movie->setScaledSize(final_size);
        image_background->show();
    }
    else
    {
        graphicsView->move(base_point);
        graphicsView->resize(final_size);
        QTransform transform;
        transform.scale(QSizeF(final_size).width() / video_item->size().width(),QSizeF(final_size).height() / video_item->size().height());
        video_item->setTransform(transform);
        graphicsView->show();
    }
    if (path_list[Path_List_Index].mouse_effect)
    {
        int move_x;
        int move_y;
        switch (path_list[Path_List_Index].mouse_control_type)
        {
        case Mouse_Control_Type::Follow_Desktop:
        {
            move_x = int(path_list[Path_List_Index].k_mouse_move_width * (QCursor::pos().x() - (QApplication::desktop()->width() / 2)) * (static_cast<double>(desktop_width) / QApplication::desktop()->width()));
            move_y = int(path_list[Path_List_Index].k_mouse_move_height * (QCursor::pos().y() - (QApplication::desktop()->height() / 2)) * (static_cast<double>(desktop_height) / QApplication::desktop()->height()));
            break;
        }
        case Mouse_Control_Type::Follow_Wallpaper:
        {
            move_x = static_cast<int>((save_final_size.width() * (1 + abs(path_list[Path_List_Index].k_mouse_move_width)) - desktop_width) / 2 * (static_cast<double>(QCursor::pos().x() * 2) / QApplication::desktop()->width() - 1) * (path_list[Path_List_Index].k_mouse_move_width >= 0 ? 1 : -1));
            move_y = static_cast<int>((save_final_size.height() * (1 + abs(path_list[Path_List_Index].k_mouse_move_height)) - desktop_height) / 2 * (static_cast<double>(QCursor::pos().y() * 2) / QApplication::desktop()->height() - 1) * (path_list[Path_List_Index].k_mouse_move_height >= 0 ? 1 : -1));
            break;
        }
        }
        if (path_list[Path_List_Index].is_image)
        {
            image_background->move(base_point + QPoint(move_x, move_y));
        }
        else
        {
            graphicsView->move(base_point + QPoint(move_x, move_y));
        }
        btnCheck->doing = true;
    }
    updating = false;
    if (path_list[Path_List_Index].is_image)
    {
        image_movie->start();
    }
    else
    {
        media_player->play();
    }
}
void Desktop_Background::Play()
{
    if (use_plugin)
    {
        if (plugin_interface)
        {
            if (plugin_interface->Plugin_Version >= P_Version{0, 0, 1})
            {
                if (plugin_interface->inited)
                {
                    plugin_interface->play();
                }
            }
        }
    }
    if (image_background->isVisible())
    {
        if (image_movie->state() != QMovie::MovieState::Running)
        {
            if (image_movie->state() == QMovie::MovieState::Paused)
            {
                image_movie->setPaused(false);
            }
            else
            {
                image_movie->start();
            }
        }
    }
    if (graphicsView->isVisible())
    {
        if (media_player->state() != QMediaPlayer::State::PlayingState)
        {
            media_player->play();
        }
    }
}
void Desktop_Background::Pause()
{
    if (use_plugin)
    {
        if (plugin_interface)
        {
            if (plugin_interface->Plugin_Version >= P_Version{0, 0, 1})
            {
                if (plugin_interface->inited)
                {
                    plugin_interface->pause();
                }
            }
        }
    }
    if (image_background->isVisible())
    {
        if (image_movie->state() == QMovie::MovieState::Running)
        {
            image_movie->setPaused(true);
        }
    }
    if (graphicsView->isVisible())
    {
        if (media_player->state() == QMediaPlayer::State::PlayingState)
        {
            media_player->pause();
        }
    }
}
void Desktop_Background::Mouse_Play()
{
    if (!btnCheck->doing)
    {
        btnCheck->doing = true;
        Desktop_Background::mouse_move_event(QCursor::pos().x(), QCursor::pos().y());
    }
}
bool Desktop_Background::Get_Playing_State()
{
    if (image_background->isVisible())
    {
        return image_movie->state() == QMovie::MovieState::Running;
    }
    else
    {
        return media_player->state() == QMediaPlayer::State::PlayingState;
    }
}
bool Desktop_Background::Get_Mouse_Playing_State()
{
    return btnCheck->doing;
}
void Desktop_Background::Mouse_Pause()
{
    btnCheck->doing = false;
}
Desktop_Background::Desktop_Background(QWidget *parent)
    :QWidget(parent)
{
    image_movie->stop();
    media_player->stop();
    image_background->hide();
    graphicsView->hide();
    scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    setAttribute(Qt::WA_StaticContents);
    graphicsView->setRenderHint(QPainter::RenderHint::SmoothPixmapTransform, true);
    move(0, 0);
    image_background->setMovie(image_movie);
    media_player->setVideoOutput(video_item);
    scene->addItem(video_item);
    image_movie->setCacheMode(QMovie::CacheAll);
    graphicsView->setCacheMode(QGraphicsView::CacheBackground);
    holding_pos_timer->setInterval(50);
    connect(holding_pos_timer, &QTimer::timeout, this, [=]
    {
        if (holding_time < holding_max_time)
        {
            holding_time++;//可能不优雅,但想不到更好的方法.positionChanged(qint64 position)并不稳定,load时无效
            if (media_player->duration() != 0)
            {
                media_player->setPosition(media_player->duration() * holding_value / 100);
                holding_pos_timer->stop();
                holding_time = 0;
            }
        }
        else
        {
            holding_pos_timer->stop();
            holding_time = 0;
        }
    });
    connect(media_player, &QMediaPlayer::stateChanged,this,[=](QMediaPlayer::State new_state)
    {
        if (sending_info)
        {
            return;
        }
        if (!updating)
        {
            if (new_state == QMediaPlayer::State::StoppedState)
            {
                graphicsView->setUpdatesEnabled(false);
                media_player->setMedia(QUrl::fromLocalFile(path_list[Path_List_Index].path));
                media_player->play();
            }
        }
    });
    connect(video_item,&QGraphicsVideoItem::nativeSizeChanged,this,[=](const QSizeF &size)
    {
        if (call_video_update && size.width() > 0 && size.height() > 0)
        {
            call_video_update = false;
            media_player->pause();
            video_item->setSize(size);
            Desktop_Background::Second_Update_Widget();
        }
        else if (size.width() > 0 && size.height() > 0)
        {
            if (!graphicsView->updatesEnabled())
            {
                QTimer::singleShot(25, this, [=]{graphicsView->setUpdatesEnabled(true);});//MediaPlayer播放完后会有短暂黑屏
            }
        }
    });
    connect(btnCheck,&keyScan::mouse_move,this,&Desktop_Background::mouse_move_event);//高耗能
    btnCheck->start();
    connect(image_movie, &QMovie::frameChanged, this, [=](int frame)
    {
        if (!image_background->isVisible())
        {
            return;
        }
        int all_frame = image_movie->frameCount();
        if (all_frame <= 0)
        {
            return;
        }
        int result = frame * 100 / all_frame;
        emit send_position(result, QString::number(frame));
    });
    connect(media_player, &QMediaPlayer::positionChanged, this, [=](qint64 position)
    {
        if (!graphicsView->isVisible())
        {
            return;
        }
        qint64 all_position = media_player->duration();
        if (all_position <= 0)
        {
            return;
        }
        int result = static_cast<int>(position * 100 / all_position);
        position /= 1000;//mSec
        int sec = position % 60;
        int min = static_cast<int>(position / 60);
        QString text;
        if (min < 10)
        {
            text = "0";
        }
        text += QString::number(min);
        text += ":";
        if (sec < 10)
        {
            text += "0";
        }
        text += QString::number(sec);
        emit send_position(result, text);
    });
    connect(send_position_sender, &P_Sender::sig_Send_Data, this, [=](QVariant var)
    {
        QList<QVariant> item_list = var.toList();
        if (item_list.count() == 2)
        {
            emit send_position(item_list[0].toInt(), item_list[1].toString());
        }
    });
}
void Desktop_Background::Set_Speed(int value)
{
    qreal result = static_cast<qreal>(value) / 100;
    media_player->setPlaybackRate(result);
    image_movie->setSpeed(value);
    if (use_plugin)
    {
        if (plugin_interface)
        {
            if (plugin_interface->Plugin_Version >= P_Version{0, 0, 1})
            {
                if (plugin_interface->inited)
                {
                    plugin_interface->setSpeed(value);
                }
            }
        }
    }
}
void Desktop_Background::Set_Volume(int value)
{
    media_player->setVolume(value);
    if (use_plugin)
    {
        if (plugin_interface)
        {
            if (plugin_interface->Plugin_Version >= P_Version{0, 0, 1})
            {
                if (plugin_interface->inited)
                {
                    plugin_interface->setVolume(value);
                }
            }
        }
    }
}
void Desktop_Background::Set_Position(int value)
{
    holding_value = value;
    holding_time = 0;
    holding_pos_timer->start();
    image_movie->jumpToFrame(static_cast<int>(static_cast<double>(image_movie->frameCount() * value) / 100));
    if (use_plugin)
    {
        if (plugin_interface)
        {
            if (plugin_interface->Plugin_Version >= P_Version{0, 0, 1})
            {
                if (plugin_interface->inited)
                {
                    plugin_interface->setPosition(value);
                }
            }
        }
    }
}
static bool sort_data(Path_Info list1, Path_Info list2)
{
    uint id_1 = list1.id;
    uint id_2 = list2.id;
    return id_1 < id_2;
}
void Desktop_Background::save(QSettings *settings)
{
    settings->beginGroup("Background");
    settings->setValue("Path_List_Index", Path_List_Index);
    settings->setValue("Path_List_Count", path_list.count());
    settings->endGroup();
    for (int i = 0; i < path_list.count(); i++)
    {
        settings->beginGroup(QString("Wallpaper%1").arg(i));
        settings->setValue("Empty", path_list[i].Empty);
        settings->setValue("id", path_list[i].id);
        settings->setValue("name", path_list[i].name);
        settings->setValue("is_image", path_list[i].is_image);
        settings->setValue("path", path_list[i].path);
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
        settings->setValue("scale_type", index);
        settings->setValue("center", path_list[i].center);
        settings->setValue("mouse_effect", path_list[i].mouse_effect);
        settings->setValue("k_mouse_move_width", path_list[i].k_mouse_move_width);
        settings->setValue("k_mouse_move_height", path_list[i].k_mouse_move_height);
        settings->setValue("delta_x", path_list[i].delta_x);
        settings->setValue("delta_y", path_list[i].delta_y);
        settings->setValue("on_Antialiasing", path_list[i].on_Antialiasing);
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
        settings->setValue("mouse_control_type", index);
        settings->setValue("wallpaper_width", path_list[i].wallpaper_width);
        settings->setValue("wallpaper_height", path_list[i].wallpaper_height);
        settings->endGroup();
    }
}
void Desktop_Background::load(QSettings *settings)
{
    sending_info = true;
    settings->beginGroup("Background");
    path_list.clear();
    Path_List_Index = settings->value("Path_List_Index", 0).toInt();
    int path_list_count = settings->value("Path_List_Count", 0).toInt();
    settings->endGroup();
    for (int i = 0; i < path_list_count; i++)
    {
        settings->beginGroup(QString("Wallpaper%1").arg(i));
        bool Empty = settings->value("Empty", true).toBool();
        if (Empty)
        {
            path_list<<Path_Info(0,"",true,"",Scale_Type::Full,true,false,0,0,0,0,true);
        }
        else
        {
            path_list<<Path_Info(
                           static_cast<uint>(settings->value("id", 0).toInt()),
                           settings->value("name", "").toString(),
                           settings->value("is_image", true).toBool(),
                           settings->value("path", "").toString(),
                           (QList<Scale_Type>()
                            <<Scale_Type::No
                            <<Scale_Type::Each
                            <<Scale_Type::Width
                            <<Scale_Type::Height
                            <<Scale_Type::Short
                            <<Scale_Type::Long
                            <<Scale_Type::Full
                            <<Scale_Type::User
                            )[settings->value("scale_type", 6).toInt()],
                           settings->value("center", true).toBool(),
                           settings->value("mouse_effect", false).toBool(),
                           settings->value("k_mouse_move_width", 0).toDouble(),
                           settings->value("k_mouse_move_height", 0).toDouble(),
                           settings->value("delta_x", 0).toInt(),
                           settings->value("delta_y", 0).toInt(),
                           settings->value("on_Antialiasing", true).toBool(),
                           (QList<Mouse_Control_Type>()
                            <<Mouse_Control_Type::Follow_Desktop
                            <<Mouse_Control_Type::Follow_Wallpaper
                            )[settings->value("mouse_control_type", 0).toInt()],
                           settings->value("wallpaper_width", 0).toInt(),
                           settings->value("wallpaper_height", 0).toInt()
                           );
        }
        settings->endGroup();
    }
    if (path_list.count() == 0)
    {
        path_list<<Path_Info(0,"",true,"",Scale_Type::Full,true,false,0,0,0,0,true);
    }
    sending_info = false;
    Update_Widget();
}
