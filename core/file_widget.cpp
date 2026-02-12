#include "file_widget.h"
void File_Widget::Set_Base_Icon()
{
    movie_size = get_Image_Size(":/base/folder.svg");
    if (movie_size.isValid())
    {
        movie->setFileName(":/base/folder.svg");
        emit Basic_Widget::size_changed(Carrier->size());
    }
}
File_Widget::File_Widget(QWidget *parent)
    :Process_Widget(parent)
{
    is_file_widget = true;
    movie_size = get_Image_Size(":/base/folder.svg");
    if (movie_size.isValid())
    {
        movie->setFileName(":/base/folder.svg");
        emit Basic_Widget::size_changed(Carrier->size());
    }
    this->process_name_label->setText("文件");
    if (set_auto_resize->isChecked())
    {
        auto_set_font_size();
    }
    menu->insertAction(break_out, open_way);
    menu->insertAction(break_out, open_path_way);
    menu->insertSeparator(break_out);
    set_file_process->addAction(set_file_as_file);
    set_file_process->addAction(set_file_as_dir);
    setting_menu->insertMenu(set_process, set_file_process);
    menu->addSeparator();
    menu->addAction(show_info);
    process_string = "dde-file-manager --show-item " + file_path;
    Extra_Pressed_Do();
}
File_Widget::~File_Widget()
{
    if (file_widget_list)
    {
        file_widget_list->removeOne(this);
    }
}
void File_Widget::Extra_Pressed_Do()
{
    update_running_path();
}
void File_Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == open_way)
    {
        if (file_path.isEmpty())
        {
            return;
        }
        QProcess process;
        process.setProgram("/bin/bash");
        process.setWorkingDirectory(running_path);
        QString m_process_str = "dde-file-manager -d -o";
        if (file_open_way_process)
        {
            m_process_str = *file_open_way_process;
        }
        m_process_str += " ";
        m_process_str += file_path;
        process.setArguments(QStringList() << "-c" << m_process_str);
        process.setStandardOutputFile("/dev/null");
        process.setStandardErrorFile("/dev/null");
        process.startDetached();
    }
    else if (know_what == open_path_way)
    {
        if (file_path.isEmpty())
        {
            return;
        }
        QProcess process;
        process.setProgram("/bin/bash");
        process.setWorkingDirectory(running_path);
        QString m_process_str = "dde-file-manager --show-item";
        if (file_open_path_process)
        {
            m_process_str = *file_open_path_process;
        }
        m_process_str += " ";
        m_process_str += file_path;
        process.setArguments(QStringList() << "-c" << m_process_str);
        process.setStandardOutputFile("/dev/null");
        process.setStandardErrorFile("/dev/null");
        process.startDetached();
    }
    else if (know_what == set_file_as_file || know_what == set_file_as_dir)
    {
        update_running_path();
        QString filepath;
        QString name;
        if (know_what == set_file_as_file)
        {
            filepath = QFileDialog::getOpenFileName(nullptr, "获取文件", running_path);
            name = QFileInfo(filepath).fileName();
        }
        else
        {
            filepath = QFileDialog::getExistingDirectory(nullptr, "获取文件夹", running_path);
            name = QDir(filepath).dirName();
        }
        X11_Rasie();
        if (filepath.isEmpty())
        {
            return;
        }
        file_path = filepath;
        QMessageBox::StandardButton name_ans = QMessageBox::question(nullptr, "确认名称", QString("将设为名称:%1\n请确认是否应用").arg(name));
        if (name_ans == QMessageBox::Yes)
        {
            this->process_name_label->setText(name);
            if (set_auto_resize->isChecked())
            {
                auto_set_font_size();
            }
        }
        QMessageBox::StandardButton image_ans = QMessageBox::question(nullptr, "确认图像", QString("是否使用推测图像"));
        if (image_ans == QMessageBox::Yes)
        {
            if (know_what == set_file_as_file)
            {
                QFileInfo fileInfo(filepath);
                QMimeDatabase mimeDb;
                QMimeType mimeType;
                mimeType = mimeDb.mimeTypeForFile(fileInfo);
                QString mimeName = mimeType.name();
                if (mimeName.startsWith("image/"))
                {
                    movie_size = get_Image_Size(file_path);
                    if (movie_size.isValid())
                    {
                        theme_image = false;
                        movie->setFileName(file_path);
                        emit Basic_Widget::size_changed(Carrier->size());
                    }
                }
                else if (mimeName == "application/x-desktop")
                {
                    theme_image = true;
                    QSettings desktopSettings(file_path, QSettings::IniFormat);
                    desktopSettings.setIniCodec("UTF-8");
                    desktopSettings.beginGroup("Desktop Entry");
                    theme_name = desktopSettings.value("Icon", "application").toString();
                    desktopSettings.endGroup();
                    just_show_image->setIcon(QIcon::fromTheme(theme_name));
                    just_show_image->setIconSize(QSize(128, 128));
                    just_show_image->show();
                    movie->setFileName("");
                    movie_size = QSize(128, 128);
                    emit Basic_Widget::size_changed(Carrier->size());
                }
                else
                {
                    theme_image = true;
                    QIcon icon = QIcon::fromTheme(mimeType.iconName());
                    theme_name = mimeType.iconName();
                    if (icon.isNull())
                    {
                        icon = QIcon::fromTheme(mimeType.genericIconName());
                        theme_name = mimeType.genericIconName();
                    }
                    if (icon.isNull())
                    {
                        theme_name = "unknown";
                    }
                    just_show_image->setIcon(QIcon::fromTheme(theme_name));
                    just_show_image->setIconSize(QSize(128, 128));
                    just_show_image->show();
                    movie->setFileName("");
                    movie_size = QSize(128, 128);
                    emit Basic_Widget::size_changed(Carrier->size());
                }
            }
            else
            {
                theme_image = true;
                theme_name = "folder";
                just_show_image->setIcon(QIcon::fromTheme(theme_name));
                just_show_image->setIconSize(QSize(128, 128));
                just_show_image->show();
                movie->setFileName("");
                movie_size = QSize(128, 128);
                emit Basic_Widget::size_changed(Carrier->size());
            }
        }
        process_string = get_running_process(file_path);
        QMessageBox::StandardButton final_ans = QMessageBox::question(nullptr, "确认进程", QString("默认进程:%1\n请确认是否应用").arg(process_string));
        if (final_ans != QMessageBox::Yes)
        {
            bool ok = false;
            QString final_string = QInputDialog::getText(nullptr, "设置运行进程", "设置运行进程:",QLineEdit::Normal, process_string, &ok);
            if (!ok || final_string.isEmpty())
            {
                process_string = "";
            }
            else
            {
                process_string = final_string;
            }
        }
    }
    else if (know_what == show_info)
    {
        if (file_path.isEmpty())
        {
            return;
        }
        QProcess process;
        process.setProgram("/bin/bash");
        process.setWorkingDirectory(running_path);
        QString m_process_str = "dde-file-manager -p";
        if (file_open_info_process)
        {
            m_process_str = *file_open_info_process;
        }
        m_process_str += " ";
        m_process_str += file_path;
        process.setArguments(QStringList() << "-c" << m_process_str);
        process.setStandardOutputFile("/dev/null");
        process.setStandardErrorFile("/dev/null");
        process.startDetached();
    }
    else
    {
        context_solution(know_what);
    }
}
void File_Widget::quickly_set(QString filepath)
{
    if (filepath.isEmpty())
    {
        return;
    }
    if (!QUrl(filepath).isValid())
    {
        return;
    }
    QFileInfo fileInfo(filepath);
    this->process_name_label->setText(fileInfo.fileName());
    file_path = filepath;
    if (!fileInfo.isDir())
    {
        QMimeDatabase mimeDb;
        QMimeType mimeType;
        mimeType = mimeDb.mimeTypeForFile(fileInfo);
        QString mimeName = mimeType.name();
        if (mimeName.startsWith("image/"))
        {
            movie_size = get_Image_Size(file_path);
            if (movie_size.isValid())
            {
                theme_image = false;
                movie->setFileName(file_path);
                emit Basic_Widget::size_changed(Carrier->size());
            }
        }
        else if (mimeName == "application/x-desktop")
        {
            theme_image = true;
            QSettings desktopSettings(file_path, QSettings::IniFormat);
            desktopSettings.setIniCodec("UTF-8");
            desktopSettings.beginGroup("Desktop Entry");
            theme_name = desktopSettings.value("Icon", "application").toString();
            desktopSettings.endGroup();
            just_show_image->setIcon(QIcon::fromTheme(theme_name));
            just_show_image->setIconSize(QSize(128, 128));
            just_show_image->show();
            movie->setFileName("");
            movie_size = QSize(128, 128);
            emit Basic_Widget::size_changed(Carrier->size());
        }
        else
        {
            theme_image = true;
            QIcon icon = QIcon::fromTheme(mimeType.iconName());
            theme_name = mimeType.iconName();
            if (icon.isNull())
            {
                icon = QIcon::fromTheme(mimeType.genericIconName());
                theme_name = mimeType.genericIconName();
            }
            if (icon.isNull())
            {
                theme_name = "unknown";
            }
            just_show_image->setIcon(QIcon::fromTheme(theme_name));
            just_show_image->setIconSize(QSize(128, 128));
            just_show_image->show();
            movie->setFileName("");
            movie_size = QSize(128, 128);
            emit Basic_Widget::size_changed(Carrier->size());
        }
    }
    else
    {
        theme_image = true;
        theme_name = "folder";
        just_show_image->setIcon(QIcon::fromTheme(theme_name));
        just_show_image->setIconSize(QSize(128, 128));
        just_show_image->show();
        movie->setFileName("");
        movie_size = QSize(128, 128);
        emit Basic_Widget::size_changed(Carrier->size());
    }
    process_string = get_running_process(file_path);
}
QString File_Widget::get_running_process(QString m_file_path)
{
    QProcess process;
    process.start("xdg-mime", QStringList() << "query" << "filetype" << m_file_path);
    QString mime_type = "";
    if (process.waitForFinished(5000) && process.exitCode() == 0)
    {
        mime_type = process.readAllStandardOutput().trimmed();
    }
    process.start("xdg-mime", QStringList() << "query" << "default" << mime_type);
    QString final_process = "";
    if (process.waitForFinished(5000) && process.exitCode() == 0)
    {
        final_process = process.readAllStandardOutput().trimmed();
    }
    QString m_running_process = "";
    if (!final_process.isEmpty())
    {
        if (!final_process.endsWith(".desktop"))
        {
            final_process += ".desktop";
        }
        QStringList searchPaths = {QDir::home().filePath(".local/share/applications"),
                                  "/usr/local/share/applications",
                                  "/usr/share/applications"};
        for (const QString &path : searchPaths)
        {
            QString m_filepath = QDir(path).filePath(final_process);
            if (QFile::exists(m_filepath))
            {
                QSettings desktopSettings(m_filepath, QSettings::IniFormat);
                desktopSettings.beginGroup("Desktop Entry");
                QString exec = desktopSettings.value("Exec").toString();
                desktopSettings.endGroup();
                m_running_process = exec;
            }
        }
    }
    if (m_running_process.isEmpty())
    {
        m_running_process = file_path;
    }
    else
    {
        m_running_process.replace("%f", QDir::toNativeSeparators(file_path));
        m_running_process.replace("%F", QDir::toNativeSeparators(file_path));
        m_running_process.replace("%u", QDir::toNativeSeparators(file_path));
        m_running_process.replace("%U", QDir::toNativeSeparators(file_path));
        QRegularExpression placeholderRegex("%[a-zA-Z]");
        m_running_process.replace(placeholderRegex, "");
    }
    return m_running_process;
}
void File_Widget::update_running_path()
{

    QFileInfo fileInfo = QFileInfo(file_path);
    if (fileInfo.isDir())
    {
        QDir file_dir = QDir(file_path);
        file_dir.cdUp();
        running_path = file_dir.path();
    }
    else
    {
        running_path = fileInfo.absolutePath();
    }
    QDir file_dir = QDir(file_path);
    if (!file_dir.exists())
    {
        running_path = QDir::homePath();
    }
}
void File_Widget::set_file_or_dir(bool file)
{
    update_running_path();
    QString filepath;
    if (file)
    {
        filepath = QFileDialog::getOpenFileName(nullptr, "获取文件", running_path);
    }
    else
    {
        filepath = QFileDialog::getExistingDirectory(nullptr, "获取文件夹", running_path);
    }
    X11_Rasie();
    if (filepath.isEmpty())
    {
        return;
    }
    file_path = filepath;
    process_string = get_running_process(file_path);
    QMessageBox::StandardButton final_ans = QMessageBox::question(nullptr, "确认进程", QString("默认进程:%1\n请确认是否应用").arg(process_string));
    if (final_ans != QMessageBox::Yes)
    {
        bool ok = false;
        QString final_string = QInputDialog::getText(nullptr, "设置运行进程", "设置运行进程:",QLineEdit::Normal, process_string, &ok);
        if (!ok || final_string.isEmpty())
        {
            process_string = "";
        }
        else
        {
            process_string = final_string;
        }
    }
}
void File_Widget::save(QSettings *settings)
{
    Process_Widget::save(settings);
    settings->setValue("file_path", file_path);
}
void File_Widget::load(QSettings *settings)
{
    Process_Widget::load(settings);
    QStringList process_string_list = process_string.split(" ");
    file_path = process_string_list.last();
    if (file_path.isEmpty())
    {
        file_path = QDir::homePath();
    }
    file_path = settings->value("file_path", file_path).toString();
}
