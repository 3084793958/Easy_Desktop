#include "file_tree.h"
#include <QX11Info>
#include <X11/Xlib.h>
void File_Tree::X11_Raise()
{
    Window win_Id = static_cast<Window>(m_WinId);
    Display *display = QX11Info::display();
    XRaiseWindow(display, win_Id);
    XFlush(display);
}
#undef CursorShape
File_Tree::File_Tree(QWidget *parent)
    :Basic_Widget(parent)
{
    menu->addAction(open_it);
    menu->addAction(open_way);
    menu->addAction(open_path_way);
    menu->addAction(show_info);
    tree_setting->addAction(set_dir_path);
    tree_setting->addAction(refresh_action);
    menu->addMenu(tree_setting);
    basic_context(menu);
    carrier_widget->move(10, 10);
    search_edit->move(0, 5);
    treeView->move(0, 50);
    carrier_widget->setStyleSheet("QWidget{background:rgba(0,0,0,0);color:rgba(0,0,0,255)}");
    search_edit->setStyleSheet("QLineEdit{border-radius:10px 10px;background:rgba(0,0,0,25);font-size:15px}");
    treeView->setAlternatingRowColors(true);
    treeView->setLayoutDirection(Qt::LeftToRight);
    treeView->setEditTriggers(QListView::NoEditTriggers);
    treeView->setStyleSheet("QTreeView{background:rgba(255,255,255,0);color:rgb(0,0,0)}");
    treeView->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{border:none;background:rgba(0,0,0,0);width:8px;margin:0px0px0px0px;}"
                                                 "QScrollBar::handle:vertical{background:rgba(0,0,0,75);border-radius:4px;min-height:20px;}"
                                                 "QScrollBar::handle:vertical:hover{background:rgba(0,0,0,125);}"
                                                 "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0px;}"
                                                 "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}");
    treeView->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{border:none;background:rgba(0,0,0,0);height:8px;margin:0px0px0px0px;}"
                                                   "QScrollBar::handle:horizontal{background:rgba(0,0,0,75);border-radius:4px;min-width:20px;}"
                                                   "QScrollBar::handle:horizontal:hover{background:rgba(0,0,0,125);}"
                                                   "QScrollBar::add-line:horizontal,QScrollBar::sub-line:horizontal{width:0px;}"
                                                   "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:none;}");
    search_edit->setPlaceholderText("搜索");
    search_img_action->setIcon(QIcon(":/base/search.svg"));
    search_edit->addAction(search_img_action, QLineEdit::LeadingPosition);
    search_del_action->setIcon(QIcon(":/base/del.svg"));
    model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot);
    model->setRootPath(QDir::rootPath());
    treeView->setModel(model);
    treeView->setRootIndex(model->index(QDir::rootPath()));
    treeView->setAnimated(true);
    treeView->setSortingEnabled(true);
    treeView->setColumnWidth(0, 150);
    carrier_widget->setMouseTracking(true);
    treeView->setMouseTracking(true);
    search_edit->setMouseTracking(true);
    installEventFilter(treeView);
    connect(search_del_action, &QAction::triggered, this, [=]
    {
        search_edit->removeAction(search_del_action);
        search_edit->clear();
    });
    connect(search_edit, &QLineEdit::textChanged, this, [=]
    {
        if (search_edit->text().isEmpty())
        {
            search_edit->removeAction(search_del_action);
            model->setNameFilters(QStringList());
        }
        else
        {
            search_edit->addAction(search_del_action,QLineEdit::TrailingPosition);
            QStringList filters;
            filters << QString("*%1*").arg(search_edit->text());
            model->setNameFilters(filters);
            model->setNameFilterDisables(false);
        }
    });
    connect(this, &Basic_Widget::size_changed, this, [=](QSize size)
    {
        carrier_widget->resize(size - QSize(20, 20));
        search_edit->resize(size.width() - 20, 40);
        treeView->resize(size - QSize(20, 70));
    });
    connect(this->treeView, &QTreeView::doubleClicked, this, [=]
    {
        if (treeView->selectionModel())
        {
            if (model->hasChildren(treeView->selectionModel()->currentIndex())) return;
            QString this_file_path = model->filePath(treeView->selectionModel()->currentIndex());
            QFileInfo fileinfo(this_file_path);
            QString running_path = fileinfo.dir().path();
            QProcess process;
            process.start("xdg-mime", QStringList() << "query" << "filetype" << this_file_path);
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
                m_running_process = this_file_path;
            }
            else
            {
                m_running_process.replace("%f", QDir::toNativeSeparators(this_file_path));
                m_running_process.replace("%F", QDir::toNativeSeparators(this_file_path));
                m_running_process.replace("%u", QDir::toNativeSeparators(this_file_path));
                m_running_process.replace("%U", QDir::toNativeSeparators(this_file_path));
                QRegularExpression placeholderRegex("%[a-zA-Z]");
                m_running_process.replace(placeholderRegex, "");
            }
            ;
            QProcess process2;
            process2.setProgram("/bin/bash");
            process2.setWorkingDirectory(running_path);
            process2.setArguments(QStringList() << "-c" << m_running_process);
            process2.setStandardOutputFile("/dev/null");
            process2.setStandardErrorFile("/dev/null");
            process2.startDetached();
        }
    });
    resize(600, 300);
    show();
}
File_Tree::~File_Tree()
{
    if (file_tree_list)
    {
        file_tree_list->removeOne(this);
    }
}
void File_Tree::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == open_it)
    {
        if (treeView->selectionModel())
        {
            QString this_file_path = model->filePath(treeView->selectionModel()->currentIndex());
            QFileInfo fileinfo(this_file_path);
            QString running_path = fileinfo.dir().path();
            QProcess process;
            process.start("xdg-mime", QStringList() << "query" << "filetype" << this_file_path);
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
                m_running_process = this_file_path;
            }
            else
            {
                m_running_process.replace("%f", QDir::toNativeSeparators(this_file_path));
                m_running_process.replace("%F", QDir::toNativeSeparators(this_file_path));
                m_running_process.replace("%u", QDir::toNativeSeparators(this_file_path));
                m_running_process.replace("%U", QDir::toNativeSeparators(this_file_path));
                QRegularExpression placeholderRegex("%[a-zA-Z]");
                m_running_process.replace(placeholderRegex, "");
            }
            ;
            QProcess process2;
            process2.setProgram("/bin/bash");
            process2.setWorkingDirectory(running_path);
            process2.setArguments(QStringList() << "-c" << m_running_process);
            process2.setStandardOutputFile("/dev/null");
            process2.setStandardErrorFile("/dev/null");
            process2.startDetached();
        }
    }
    else if (know_what == open_way)
    {
        if (treeView->selectionModel())
        {
            QString this_file_path = model->filePath(treeView->selectionModel()->currentIndex());
            QProcess process;
            process.setProgram("/bin/bash");
            process.setWorkingDirectory(this_file_path);
            QString m_process_str = "dde-file-manager -d -o";
            if (file_open_way_process)
            {
                m_process_str = *file_open_way_process;
            }
            m_process_str += " ";
            m_process_str += this_file_path;
            process.setArguments(QStringList() << "-c" << m_process_str);
            process.setStandardOutputFile("/dev/null");
            process.setStandardErrorFile("/dev/null");
            process.startDetached();
        }
    }
    else if (know_what == open_path_way)
    {
        if (treeView->selectionModel())
        {
            QString this_file_path = model->filePath(treeView->selectionModel()->currentIndex());
            QProcess process;
            process.setProgram("/bin/bash");
            process.setWorkingDirectory(this_file_path);
            QString m_process_str = "dde-file-manager --show-item";
            if (file_open_path_process)
            {
                m_process_str = *file_open_path_process;
            }
            m_process_str += " ";
            m_process_str += this_file_path;
            process.setArguments(QStringList() << "-c" << m_process_str);
            process.setStandardOutputFile("/dev/null");
            process.setStandardErrorFile("/dev/null");
            process.startDetached();
        }
    }
    else if (know_what == show_info)
    {
        if (treeView->selectionModel())
        {
            QString this_file_path = model->filePath(treeView->selectionModel()->currentIndex());
            QProcess process;
            process.setProgram("/bin/bash");
            process.setWorkingDirectory(this_file_path);
            QString m_process_str = "dde-file-manager -p";
            if (file_open_info_process)
            {
                m_process_str = *file_open_info_process;
            }
            m_process_str += " ";
            m_process_str += this_file_path;
            process.setArguments(QStringList() << "-c" << m_process_str);
            process.setStandardOutputFile("/dev/null");
            process.setStandardErrorFile("/dev/null");
            process.startDetached();
        }
    }
    else if (know_what == set_dir_path)
    {
        QString filename = QFileDialog::getExistingDirectory(nullptr, "获取文件夹", root_path);
        X11_Raise();
        if (filename.isEmpty() || filename.isNull())
        {
            return;
        }
        root_path = filename;
        treeView->setRootIndex(model->index(root_path));
    }
    else if (know_what == refresh_action)
    {
        treeView->setRootIndex(model->index(root_path));
    }
    else
    {
        basic_action_func(know_what);
    }
}
void File_Tree::dropEvent(QDropEvent *event)
{
    if (*m_allow_drop && event->mimeData()->hasUrls())
    {
        QString filename;
        for (QUrl url : event->mimeData()->urls())
        {
            if (url.isValid())
            {
                QFileInfo fileinfo(url.path());
                if (fileinfo.isDir())
                {
                    filename = url.path();
                    break;
                }
            }
        }
        if (filename.isNull() || filename.isEmpty())
        {
            return;
        }
        root_path = filename;
        treeView->setRootIndex(model->index(root_path));
    }
}
void File_Tree::dragEnterEvent(QDragEnterEvent *event)
{
    if (*m_allow_drop && event->mimeData()->hasUrls())
    {
        event->accept();
    }
}
void File_Tree::wheelEvent(QWheelEvent *event)
{
    event->accept();
}
void File_Tree::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    settings->setValue("root_path", root_path);
    settings->setValue("column_width0", treeView->columnWidth(0));
    settings->setValue("column_width1", treeView->columnWidth(1));
    settings->setValue("column_width2", treeView->columnWidth(2));
    settings->setValue("column_width3", treeView->columnWidth(3));
}
void File_Tree::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    root_path = settings->value("root_path", QDir::rootPath()).toString();
    treeView->setRootIndex(model->index(root_path));
    treeView->setColumnWidth(0, settings->value("column_width0", 150).toInt());
    treeView->setColumnWidth(1, settings->value("column_width1", 150).toInt());
    treeView->setColumnWidth(2, settings->value("column_width2", 150).toInt());
    treeView->setColumnWidth(3, settings->value("column_width3", 150).toInt());
}
