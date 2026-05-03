#include "process_widget.h"
QSize Process_Widget::get_Image_Size(QString path)
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
void Process_Widget::Extra_Pressed_Do()
{}
void Process_Widget::Set_Base_Icon()
{
    movie_size = get_Image_Size(":/base/deepin-launcher.svg");
    if (movie_size.isValid())
    {
        movie->setFileName(":/base/deepin-launcher.svg");
        emit Basic_Widget::size_changed(Carrier->size());
    }
}
void Process_Widget::set_icon(QString checked_icon_path)
{
    set_auto_resize->setIcon(QIcon(checked_icon_path));
    single_press_mode_action->setIcon(QIcon(checked_icon_path));
    Basic_Widget::set_icon(checked_icon_path);
}
Process_Widget::Process_Widget(QWidget *parent)
    :Basic_Widget(parent)
{
    show();//先show大法
    this->background_color = QColor(255,255,255,0);
    Update_Background();
    Carrier->setMouseTracking(true);
    this->Carrier->setStyleSheet("background:rgba(255,255,255,50)");
    just_show_image->setStyleSheet("background:rgba(0,0,0,0)");
    just_show_image->setFocusPolicy(Qt::NoFocus);
    just_show_image->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    just_show_image->setMouseTracking(true);
    movie->setFileName(":/base/deepin-launcher.svg");
    movie_size = get_Image_Size(movie->fileName());
    image_label->setMovie(movie);
    image_label->setAlignment(Qt::AlignCenter);
    image_label->setMouseTracking(true);
    image_label->setStyleSheet("background:rgba(0,0,0,0)");
    image_label->show();
    movie->start();
    process_name_label->setMouseTracking(true);
    process_name_label->setAlignment(Qt::AlignCenter);
    process_name_label->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    process_name_label->setText(tr("进程"));
    process_name_label->setStyleSheet("background:rgba(0,0,0,0)");
    process_name_label->text_color = QColor(0, 0, 0, 255);
    process_name_label->outlineWidth = 2;
    process_name_label->outlineColor = QColor(255, 255, 255, 255);
    menu->addAction(run_action);
    single_press_mode_action->setIcon(QIcon(":/base/this.svg"));
    single_press_mode_action->setIconVisibleInMenu(true);
    setting_menu->addAction(single_press_mode_action);
    setting_menu->addAction(set_image);
    setting_menu->addAction(reset_image);
    setting_menu->addAction(set_name);
    setting_menu->addAction(set_font);
    setting_menu->addAction(set_auto_resize);
    set_auto_resize->setIcon(QIcon(":/base/this.svg"));
    set_auto_resize->setIconVisibleInMenu(false);
    setting_menu->addAction(set_text_color);
    name_out_line_menu->addAction(set_out_line_width);
    name_out_line_menu->addAction(set_out_line_color);
    setting_menu->addMenu(name_out_line_menu);
    setting_menu->addSeparator();
    setting_menu->addAction(set_process);
    setting_menu->addAction(set_by_desktop);
    menu->addAction(break_out);
    menu->addMenu(setting_menu);
    Basic_Widget::basic_context(menu);
    process_string = "ls";
    running_path = QDir::homePath();
    connect(this, &Basic_Widget::size_changed, this, [=](QSize size)
    {
        Carrier->resize(size);
        if (movie_size.isValid())
        {
            int max_width = size.width() - 10;
            int max_height = (size.height() - 15) * 3 / 4;
            float k_width = float(max_width) / movie_size.width();
            float k_height = float(max_height) / movie_size.height();
            QSize final_size;
            if (k_width < k_height)
            {
                final_size = QSize(max_width, int(movie_size.height() * k_width));
            }
            else
            {
                final_size = QSize(int(movie_size.width() * k_height), max_height);
            }
            just_show_image->setIconSize(final_size);
            image_label->resize(size.width(), final_size.height());
            just_show_image->resize(image_label->size());
            image_label->move(0, 5);
            process_name_label->resize(size.width(), size.height() - final_size.height() - 5);
            process_name_label->move(0, 7 + final_size.height());
            movie->setScaledSize(final_size);
            if (theme_image)
            {
                just_show_image->show();
                image_label->setMovie(nullptr);
                image_label->setText("");
            }
            else
            {
                just_show_image->hide();
                image_label->setMovie(movie);
            }
            just_show_image->lower();
            if (movie->fileName() != nullptr)
            {
                movie->jumpToNextFrame();//刷新+配平
                movie->setPaused(true);
                movie->setPaused(false);
            }
        }
        if (set_auto_resize->isIconVisibleInMenu())
        {
            auto_set_font_size();
        }
    });
    connect(this, &Process_Widget::Pressed, this, [=]
    {
        Extra_Pressed_Do();
        if (process_string.isEmpty())
        {
            return;
        }
        if (running_path.isEmpty())
        {
            return;
        }
        QProcess process;
        process.setProgram("/bin/bash");
        process.setWorkingDirectory(running_path);
        process.setArguments(QStringList() << "-c" << process_string);
        process.setStandardOutputFile("/dev/null");
        process.setStandardErrorFile("/dev/null");
        process.startDetached();
    });
    double_click_timer->setInterval(500);
    double_click_timer->setSingleShot(true);
    resize(75, 75);
}
void Process_Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        left_mouse_on_press = true;
        moved = false;
        Carrier->setStyleSheet(QString("background:rgba(%1,%2,%3,%4)").arg(press_color.red()).arg(press_color.green()).arg(press_color.blue()).arg(press_color.alpha()));
        if (!single_press_mode_action->isIconVisibleInMenu())
        {
            if (!double_click_timer->isActive())
            {
                double_click_timer->start();
            }
            else
            {
                double_click_timer->stop();
                emit Pressed();
            }
        }
    }
    Basic_Widget::mousePressEvent(event);
}
void Process_Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (left_mouse_on_press)
    {
        if (event->button() == Qt::LeftButton)
        {
            Carrier->setStyleSheet(QString("background:rgba(%1,%2,%3,%4)").arg(basic_color.red()).arg(basic_color.green()).arg(basic_color.blue()).arg(basic_color.alpha()));
            if (!moved)
            {
                if (single_press_mode_action->isIconVisibleInMenu())
                {
                    emit Pressed();
                }
            }
        }
    }
    left_mouse_on_press = false;
    Basic_Widget::mouseReleaseEvent(event);
}
void Process_Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (Carrier->geometry().contains(event->pos()))
    {
        if (left_mouse_on_press)
        {
            Carrier->setStyleSheet(QString("background:rgba(%1,%2,%3,%4)").arg(press_color.red()).arg(press_color.green()).arg(press_color.blue()).arg(press_color.alpha()));
        }
        else
        {
            Carrier->setStyleSheet(QString("background:rgba(%1,%2,%3,%4)").arg(hover_color.red()).arg(hover_color.green()).arg(hover_color.blue()).arg(hover_color.alpha()));
        }
    }
    else
    {
        Carrier->setStyleSheet(QString("background:rgba(%1,%2,%3,%4)").arg(basic_color.red()).arg(basic_color.green()).arg(basic_color.blue()).arg(basic_color.alpha()));
    }
    moved = true;
    Basic_Widget::mouseMoveEvent(event);
}
void Process_Widget::enterEvent(QEvent *event)
{
    (void)event;
    Carrier->setStyleSheet(QString("background:rgba(%1,%2,%3,%4)").arg(hover_color.red()).arg(hover_color.green()).arg(hover_color.blue()).arg(hover_color.alpha()));
}
void Process_Widget::leaveEvent(QEvent *event)
{
    (void)event;
    Carrier->setStyleSheet(QString("background:rgba(%1,%2,%3,%4)").arg(basic_color.red()).arg(basic_color.green()).arg(basic_color.blue()).arg(basic_color.alpha()));
}
void Process_Widget::context_solution(QAction *know_what)
{
    if (know_what == run_action)
    {
        emit Pressed();
    }
    else if (know_what == single_press_mode_action)
    {
        single_press_mode_action->setIconVisibleInMenu(!single_press_mode_action->isIconVisibleInMenu());
    }
    else if (know_what == set_image)
    {
        QString filename = QFileDialog::getOpenFileName(nullptr, tr("获取图像"), QDir::homePath(), tr("图像文件") + "(*.png *.jpg *.jpeg *.svg *.gif *.bmp);;" + tr("所有文件") + "(*.*)");
        My_X11_Libs::X11_Raise();
        if (filename.isEmpty())
        {
            return;
        }
        movie_size = get_Image_Size(filename);
        if (movie_size.isValid())
        {
            theme_image = false;
            movie->setFileName(filename);
            emit Basic_Widget::size_changed(Carrier->size());
        }
    }
    else if (know_what == set_name)
    {
        QInputDialog dialog;
        dialog.setParent(nullptr);
        dialog.setWindowTitle(tr("获取文字"));
        dialog.setLabelText(tr("输入文字"));
        dialog.setTextValue(this->process_name_label->text());
        dialog.setInputMode(QInputDialog::TextInput);
        dialog.setOption(QInputDialog::UsePlainTextEditForTextInput);
        QFont final_font = this->process_name_label->font();
        final_font.setPointSize(dialog.font().pointSize());
        dialog.setFont(final_font);
        if (dialog.exec() != QDialog::Accepted)
        {
            return;
        }
        this->process_name_label->setText(dialog.textValue());
        if (set_auto_resize->isIconVisibleInMenu())
        {
            auto_set_font_size();
        }
    }
    else if (know_what == set_font)
    {
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, process_name_label->font(), nullptr);
        if (!ok)
        {
            return;
        }
        process_name_label->setFont(font);
    }
    else if (know_what == set_text_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(this->process_name_label->text_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        this->process_name_label->text_color = colorDialog.currentColor();
    }
    else if (know_what == set_out_line_width)
    {
        bool ok = false;
        int out_line_width = QInputDialog::getInt(nullptr, tr("获取数值"), tr("描边大小"), this->process_name_label->outlineWidth, 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        this->process_name_label->outlineWidth = out_line_width;
    }
    else if (know_what == set_out_line_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(this->process_name_label->outlineColor);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        this->process_name_label->outlineColor = colorDialog.currentColor();
    }
    else if (know_what == set_process)
    {
        bool ok = false;
        QString first_string = QInputDialog::getText(nullptr, tr("设置运行进程"), tr("设置运行进程:"), QLineEdit::Normal, process_string, &ok);
        if (!ok || first_string.isEmpty())
        {
            return;
        }
        ok = false;
        QMessageBox::information(nullptr, tr("设置运行路径"), tr("设置运行路径"));
        QString filepath = QFileDialog::getExistingDirectory(nullptr, tr("获取运行路径"), running_path);
        My_X11_Libs::X11_Raise();
        if (filepath.isEmpty())
        {
            filepath = QDir::homePath();
        }
        QMessageBox::StandardButton final_ans = QMessageBox::question(nullptr, tr("确认进程"), tr("最终指令:") + first_string + tr("\n运行路径:") + filepath);
        if (final_ans == QMessageBox::Yes)
        {
            process_string = first_string;
            running_path = filepath;
        }
    }
    else if (know_what == set_by_desktop)
    {
        QString final_process = QFileDialog::getOpenFileName(nullptr, tr("获取.desktop文件"), running_path);
        My_X11_Libs::X11_Raise();
        if (!final_process.isEmpty())
        {
            if (QFile::exists(final_process))
            {
                QSettings desktopSettings(final_process, QSettings::IniFormat);
                desktopSettings.setIniCodec("UTF-8");
                desktopSettings.beginGroup("Desktop Entry");
                QString exec = desktopSettings.value("Exec", "ls").toString();
                exec.replace("%f", "");
                exec.replace("%F", "");
                exec.replace("%u", "");
                exec.replace("%U", "");
                QRegularExpression placeholderRegex("%[a-zA-Z]");
                exec.replace(placeholderRegex, "");
                process_string = exec;
                QDir new_running_path = QDir(final_process);
                new_running_path.cdUp();
                running_path = new_running_path.path();
                QString name;
                if (desktopSettings.contains("Name[zh_CN]"))
                {
                    name = desktopSettings.value("Name[zh_CN]").toString();
                }
                else if (desktopSettings.contains("Name[zh]"))
                {
                    name = desktopSettings.value("Name[zh]").toString();
                }
                else if (desktopSettings.contains("Name"))
                {
                    name = desktopSettings.value("Name").toString();
                }
                else
                {
                    name = "";
                }
                process_name_label->setText(name);
                theme_image = true;
                theme_name = desktopSettings.value("Icon", "application").toString();
                just_show_image->setIcon(QIcon::fromTheme(theme_name));
                just_show_image->setIconSize(QSize(128, 128));
                just_show_image->show();
                movie->setFileName("");
                movie_size = QSize(128, 128);
                emit Basic_Widget::size_changed(Carrier->size());
                desktopSettings.endGroup();
            }
        }
    }
    else if (know_what == set_background_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(basic_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取基础颜色"));
        QMessageBox::information(nullptr, tr("设置基础颜色"), tr("设置基础颜色"));
        if (colorDialog.exec() == QDialog::Accepted)
        {
            basic_color = colorDialog.currentColor();
        }
        colorDialog.setCurrentColor(press_color);
        colorDialog.setWindowTitle(tr("获取按下颜色"));
        QMessageBox::information(nullptr, tr("设置按下颜色"), tr("设置按下颜色"));
        if (colorDialog.exec() == QDialog::Accepted)
        {
            press_color = colorDialog.currentColor();
        }
        colorDialog.setCurrentColor(hover_color);
        colorDialog.setWindowTitle(tr("获取悬停(hover)颜色"));
        QMessageBox::information(nullptr, tr("设置悬停(hover)颜色"), tr("设置悬停(hover)颜色"));
        if (colorDialog.exec() == QDialog::Accepted)
        {
            hover_color = colorDialog.currentColor();
        }
        if (left_mouse_on_press)
        {
            Carrier->setStyleSheet(QString("background:rgba(%1,%2,%3,%4)").arg(press_color.red()).arg(press_color.green()).arg(press_color.blue()).arg(press_color.alpha()));
        }
        else
        {
            Carrier->setStyleSheet(QString("background:rgba(%1,%2,%3,%4)").arg(basic_color.red()).arg(basic_color.green()).arg(basic_color.blue()).arg(basic_color.alpha()));
        }
    }
    else if (know_what == set_auto_resize)
    {
        set_auto_resize->setIconVisibleInMenu(!set_auto_resize->isIconVisibleInMenu());
        if (set_auto_resize->isIconVisibleInMenu())
        {
            auto_set_font_size();
        }
    }
    else if (know_what == break_out)
    {
        if (in_carrier)
        {
            emit move_To_Desktop_Sig(this->mapToGlobal(QPoint(0, 0)));
        }
        else
        {
            emit set_to_Carrier_Sig();
        }
    }
    else if (know_what == reset_image)
    {
        Set_Base_Icon();
    }
    else
    {
        basic_action_func(know_what);
    }
}
Process_Widget::~Process_Widget()
{
    disconnect(this, nullptr, this, nullptr);
    if (!is_file_widget)
    {
        if (process_widget_list)
        {
            process_widget_list->removeOne(this);
        }
    }
}
void Process_Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    context_solution(know_what);
}
void Process_Widget::auto_set_font_size()
{
    if (process_name_label->text().isEmpty())
    {
        return;
    }
    QFont font = process_name_label->font();
    const QRect label_rect = process_name_label->contentsRect();
    int font_size = 1;
    int step = 5;
    while (step > 0 && font_size < 250)
    {
        font.setPointSize(font_size);
        QFontMetrics fm(font);
        QRect text_rect = fm.boundingRect(label_rect, static_cast<int>(process_name_label->alignment() | Qt::TextWordWrap), process_name_label->text());
        if (text_rect.height() <= label_rect.height() && text_rect.width() <= label_rect.width())
        {
            font_size += step;
        }
        else
        {
            font_size -= step;
            step /= 2;
        }
    }
    font.setPointSize(std::max(1, font_size - 1));
    process_name_label->setFont(font);
}
void Process_Widget::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    settings->setValue("in_carrier", in_carrier);
    settings->setValue("carrier_index", carrier_index);
    settings->setValue("process_string", process_string);
    settings->setValue("running_path", running_path);
    settings->setValue("theme_image", theme_image);
    if (theme_image)
    {
        settings->setValue("image_path", theme_name);
    }
    else
    {
        settings->setValue("image_path", movie->fileName());
    }
    settings->setValue("label_name", process_name_label->text());
    settings->setValue("label_font", process_name_label->font());
    settings->setValue("label_auto_resize", set_auto_resize->isIconVisibleInMenu());
    settings->setValue("single_press_mode", single_press_mode_action->isIconVisibleInMenu());
    settings->setValue("label_text_color", process_name_label->text_color.rgba());
    settings->setValue("label_out_line_color", process_name_label->outlineColor.rgba());
    settings->setValue("label_out_line_width", process_name_label->outlineWidth);
    settings->setValue("press_color", press_color.rgba());
    settings->setValue("basic_color", basic_color.rgba());
    settings->setValue("hover_color", hover_color.rgba());
}
void Process_Widget::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    process_string = settings->value("process_string", "").toString();
    running_path = settings->value("running_path", "").toString();
    theme_image = settings->value("theme_image", false).toBool();
    QString filename = settings->value("image_path", "").toString();
    if (theme_image)
    {
        theme_name = filename;
        just_show_image->setIcon(QIcon::fromTheme(theme_name));
        just_show_image->setIconSize(QSize(128, 128));
        just_show_image->show();
        movie->setFileName("");
        movie_size = QSize(128, 128);
        emit Basic_Widget::size_changed(Carrier->size());
    }
    else
    {
        if (!filename.isEmpty())
        {
            movie_size = get_Image_Size(filename);
            if (movie_size.isValid())
            {
                movie->setFileName(filename);
                emit Basic_Widget::size_changed(Carrier->size());
            }
        }
    }
    process_name_label->setText(settings->value("label_name", "").toString());
    process_name_label->setFont(settings->value("label_font", QFontDatabase::systemFont(QFontDatabase::FixedFont)).value<QFont>());
    bool auto_resize = settings->value("label_auto_resize", false).toBool();
    set_auto_resize->setIconVisibleInMenu(auto_resize);
    bool single_press_mode = settings->value("single_press_mode", true).toBool();
    single_press_mode_action->setIconVisibleInMenu(single_press_mode);
    process_name_label->text_color = QColor::fromRgba(settings->value("label_text_color", QColor(255, 255, 255, 255).rgba()).toUInt());
    process_name_label->outlineColor = QColor::fromRgba(settings->value("label_out_line_color", QColor(0, 0, 0, 255).rgba()).toUInt());
    process_name_label->outlineWidth = settings->value("label_out_line_width", 2).toInt();
    press_color = QColor::fromRgba(settings->value("press_color", QColor(255,255,255,75).rgba()).toUInt());
    basic_color = QColor::fromRgba(settings->value("basic_color", QColor(255,255,255,50).rgba()).toUInt());
    hover_color = QColor::fromRgba(settings->value("hover_color", press_color.rgba()).toUInt());
    if (auto_resize)
    {
        auto_set_font_size();
    }
    if (left_mouse_on_press)
    {
        Carrier->setStyleSheet(QString("background:rgba(%1,%2,%3,%4)").arg(press_color.red()).arg(press_color.green()).arg(press_color.blue()).arg(press_color.alpha()));
    }
    else
    {
        Carrier->setStyleSheet(QString("background:rgba(%1,%2,%3,%4)").arg(basic_color.red()).arg(basic_color.green()).arg(basic_color.blue()).arg(basic_color.alpha()));
    }
}
