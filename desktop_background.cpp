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
Path_Info::Path_Info(uint m_id)
{
    id = m_id;
    Empty = true;
}
Path_Info::Path_Info(uint m_id, QString m_name, bool m_is_image, QString m_path, Scale_Type m_scale_type, bool m_center, bool m_mouse_effect,
                     qreal m_k_mouse_move_width, qreal m_k_mouse_move_height, int m_delta_x, int m_delta_y,
                     bool m_on_Antialiasing)
{
    Empty = false;
    id = m_id;
    name = m_name;
    is_image = m_is_image;
    path = m_path;
    scale_type = m_scale_type;
    center = m_center;
    mouse_effect = m_mouse_effect;
    k_mouse_move_width = m_k_mouse_move_width;
    k_mouse_move_height = m_k_mouse_move_height;
    delta_x = m_delta_x;
    delta_y = m_delta_y;
    on_Antialiasing = m_on_Antialiasing;
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
    int move_x = int(path_list[Path_List_Index].k_mouse_move_width * (mouse_x - (QApplication::desktop()->width() / 2)) * (static_cast<double>(desktop_width) / QApplication::desktop()->width()));
    int move_y = int(path_list[Path_List_Index].k_mouse_move_height * (mouse_y - (QApplication::desktop()->height() / 2)) */*我真是天才*/ (static_cast<double>(desktop_height) / QApplication::desktop()->height()));
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
void Desktop_Background::Second_Update_Widget()
{
    if (sending_info)
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
    }
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
        int move_x = static_cast<int>(path_list[Path_List_Index].k_mouse_move_width * (QCursor::pos().x() - (QApplication::desktop()->width() / 2)) * (static_cast<double>(desktop_width) / QApplication::desktop()->width()));
        int move_y = static_cast<int>(path_list[Path_List_Index].k_mouse_move_height * (QCursor::pos().y() - (QApplication::desktop()->height() / 2)) * (static_cast<double>(desktop_height) / QApplication::desktop()->height()));
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
}
void Desktop_Background::Set_Speed(int value)
{
    qreal result = static_cast<qreal>(value) / 100;
    media_player->setPlaybackRate(result);
    image_movie->setSpeed(value);
}
void Desktop_Background::Set_Volume(int value)
{
    media_player->setVolume(value);
}
void Desktop_Background::Set_Position(int value)
{
    media_player->setPosition(media_player->duration() * value / 100);
    image_movie->jumpToFrame(image_movie->frameCount() * value / 100);
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
        }
        settings->setValue("scale_type", index);
        settings->setValue("center", path_list[i].center);
        settings->setValue("mouse_effect", path_list[i].mouse_effect);
        settings->setValue("k_mouse_move_width", path_list[i].k_mouse_move_width);
        settings->setValue("k_mouse_move_height", path_list[i].k_mouse_move_height);
        settings->setValue("delta_x", path_list[i].delta_x);
        settings->setValue("delta_y", path_list[i].delta_y);
        settings->setValue("on_Antialiasing", path_list[i].on_Antialiasing);
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
                            )[settings->value("scale_type", 6).toInt()],
                           settings->value("center", true).toBool(),
                           settings->value("mouse_effect", false).toBool(),
                           settings->value("k_mouse_move_width", 0).toDouble(),
                           settings->value("k_mouse_move_height", 0).toDouble(),
                           settings->value("delta_x", 0).toInt(),
                           settings->value("delta_y", 0).toInt(),
                           settings->value("on_Antialiasing", true).toBool()
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
