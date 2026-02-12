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
    setAcceptDrops(true);
    treeView->setAcceptDrops(true);
    treeView->setDragEnabled(true);
    treeView->setDragDropMode(QAbstractItemView::DragDrop);
    treeView->F_model = model;
    treeView->m_allow_drop = &m_allow_drop;
    show();
    this->background_color = QColor(255,255,255,50);
    Update_Background();
    menu->addAction(open_it);
    menu->addAction(open_way);
    menu->addAction(open_path_way);
    menu->addAction(show_info);
    menu->addAction(copy_action);
    tree_setting->addAction(set_dir_path);
    tree_setting->addAction(refresh_action);
    set_style_menu->addAction(set_icon_size_action);
    set_style_menu->addAction(set_hover_color);
    set_style_menu->addAction(set_select_color);
    tree_setting->addMenu(set_style_menu);
    menu->addMenu(tree_setting);
    basic_context(menu);
    carrier_widget->move(10, 10);
    search_edit->move(0, 5);
    treeView->move(0, 50);
    carrier_widget->setStyleSheet("QWidget{background:rgba(0,0,0,0);color:rgb(230,230,230)}");
    search_edit->setStyleSheet("QLineEdit{border: 0px solid rgba(0,170,255,255);border-radius:10px 10px;background:rgba(0,0,0,25);font-size:15px;color:rgb(40,40,40)}"
                               "QLineEdit:hover{border: 1px solid rgba(0,170,255,255)}");
    treeView->setAlternatingRowColors(true);
    treeView->setLayoutDirection(Qt::LeftToRight);
    treeView->setEditTriggers(QListView::NoEditTriggers);
    set_tree_view_style();
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
    model->setIconProvider(icon_provider);
    treeView->setModel(model);
    treeView->setIconSize(QSize(24, 24));
    treeView->setIndentation(24);
    treeView->setRootIndex(model->index(QDir::rootPath()));
    treeView->setAnimated(true);
    treeView->setSortingEnabled(true);
    treeView->header()->setSortIndicator(0, Qt::SortOrder::AscendingOrder);
    treeView->setColumnWidth(0, 150);
    carrier_widget->setMouseTracking(true);
    treeView->setMouseTracking(true);
    search_edit->setMouseTracking(true);
    treeView->setUniformRowHeights(true);
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
            QString m_running_process = "";
            if (mime_type == "application/x-desktop")
            {
                QSettings desktopSettings(this_file_path, QSettings::IniFormat);
                desktopSettings.setIniCodec("UTF-8");
                desktopSettings.beginGroup("Desktop Entry");
                QString exec = desktopSettings.value("Exec", "ls").toString();
                desktopSettings.endGroup();
                exec.replace("%f", "");
                exec.replace("%F", "");
                exec.replace("%u", "");
                exec.replace("%U", "");
                QRegularExpression placeholderRegex("%[a-zA-Z]");
                exec.replace(placeholderRegex, "");
                m_running_process = exec;
            }
            else
            {
                QString final_process = "";
                process.start("xdg-mime", QStringList() << "query" << "default" << mime_type);
                if (process.waitForFinished(5000) && process.exitCode() == 0)
                {
                    final_process = process.readAllStandardOutput().trimmed();
                }
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
            }
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
void File_Tree::set_tree_view_style()
{
    treeView->setStyleSheet(QString("QTreeView{background:rgba(255,255,255,0);color:rgb(60,60,60)}"
                                    "QTreeView::item:first{color:rgb(0,0,0)}"
                                    "QTreeView::item:last{color:rgb(60,60,60);border-top-left-radius:0px;border-top-right-radius:10px;border-bottom-left-radius:0px;border-bottom-right-radius:10px}"
                                    "QTreeView::item:hover{background-color:rgb(%1,%2,%3)}"
                                    "QTreeView::item:selected{background:rgb(%4,%5,%6);color:rgb(255,255,255);border: 0px solid rgba(255,255,255,0)}").arg(hover_color.red()).arg(hover_color.green()).arg(hover_color.blue()).arg(select_color.red()).arg(select_color.green()).arg(select_color.blue()));
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
        QString this_file_path = model->rootPath();
        if (treeView->selectionModel() && treeView->selectionModel()->currentIndex().isValid())
        {
             this_file_path = model->filePath(treeView->selectionModel()->currentIndex());
        }
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
    else if (know_what == open_way)
    {
        QString this_file_path = model->rootPath();
        if (treeView->selectionModel() && treeView->selectionModel()->currentIndex().isValid())
        {
             this_file_path = model->filePath(treeView->selectionModel()->currentIndex());
        }
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
    else if (know_what == open_path_way)
    {
        QString this_file_path = model->rootPath();
        if (treeView->selectionModel() && treeView->selectionModel()->currentIndex().isValid())
        {
             this_file_path = model->filePath(treeView->selectionModel()->currentIndex());
        }
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
    else if (know_what == show_info)
    {
        QString this_file_path = model->rootPath();
        if (treeView->selectionModel() && treeView->selectionModel()->currentIndex().isValid())
        {
             this_file_path = model->filePath(treeView->selectionModel()->currentIndex());
        }
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
    else if (know_what == copy_action)
    {
        if (treeView->selectionModel() && treeView->selectionModel()->currentIndex().isValid())
        {
            QList<QUrl> urls;
            urls.append(QUrl::fromLocalFile(model->filePath(treeView->selectionModel()->currentIndex())));
            QMimeData *mimeData = new QMimeData;
            mimeData->setUrls(urls);
            QApplication::clipboard()->setMimeData(mimeData);
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
        treeView->reset();
        treeView->setRootIndex(model->index(root_path));
    }
    else if (know_what == set_icon_size_action)
    {
        bool ok = false;
        int num = QInputDialog::getInt(nullptr, "获取数值", "大小:", treeView->indentation(), 10, 2147483647, 1, &ok);
        if (ok)
        {
            treeView->setIconSize(QSize(num, num));
            treeView->setIndentation(num);
        }
    }
    else if (know_what == set_hover_color)
    {
        QColorDialog colorDialog;
        colorDialog.setCurrentColor(hover_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle("获取颜色");
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        hover_color = colorDialog.currentColor();
        set_tree_view_style();
    }
    else if (know_what == set_select_color)
    {
        QColorDialog colorDialog;
        colorDialog.setCurrentColor(select_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle("获取颜色");
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        select_color = colorDialog.currentColor();
        set_tree_view_style();
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
    settings->setValue("sort_section", treeView->header()->sortIndicatorSection());
    settings->setValue("sort_order", treeView->header()->sortIndicatorOrder() == Qt::SortOrder::AscendingOrder);
    settings->setValue("indentation", treeView->indentation());
    settings->setValue("hover_color", hover_color.rgb());
    settings->setValue("select_color", select_color.rgb());
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
    treeView->header()->setSortIndicator(settings->value("sort_section", 0).toInt(), settings->value("sort_order", true).toBool() ? Qt::SortOrder::AscendingOrder : Qt::SortOrder::DescendingOrder);
    int indentation_num = settings->value("indentation", 24).toInt();
    treeView->setIconSize(QSize(indentation_num, indentation_num));
    treeView->setIndentation(indentation_num);
    hover_color = QColor::fromRgb(settings->value("hover_color", QColor(227, 242, 253).rgb()).toUInt());
    select_color = QColor::fromRgb(settings->value("select_color", QColor(0, 170, 255).rgb()).toUInt());
    set_tree_view_style();
}
QIcon My_Icon_Provider::icon(QFileIconProvider::IconType type) const
{
    return QFileIconProvider::icon(type);
}
QIcon My_Icon_Provider::icon(const QFileInfo &info) const
{
    if (info.isFile())
    {
        QMimeDatabase mimeDb;
        QMimeType mimeType;
        mimeType = mimeDb.mimeTypeForFile(info);
        QString mimeName = mimeType.name();
        if (mimeName.startsWith("image/"))
        {
            return QIcon::fromTheme(info.filePath());//应使用filePath而不是filename;
        }
        else if (mimeName == "application/x-desktop")
        {
            QSettings desktopSettings(info.filePath(), QSettings::IniFormat);
            desktopSettings.setIniCodec("UTF-8");
            desktopSettings.beginGroup("Desktop Entry");
            QString theme_name = desktopSettings.value("Icon", "application").toString();
            desktopSettings.endGroup();
            return QIcon::fromTheme(theme_name);
        }
        else
        {
            QIcon icon = QIcon::fromTheme(mimeType.iconName());
            QString theme_name = mimeType.iconName();
            if (icon.isNull())
            {
                icon = QIcon::fromTheme(mimeType.genericIconName());
                theme_name = mimeType.genericIconName();
            }
            if (icon.isNull())
            {
                theme_name = "unknown";
            }
            return QIcon::fromTheme(theme_name);
        }
    }
    else
    {
        QString theme_name = "folder";
        return QIcon::fromTheme(theme_name);
    }
}
My_Tree_View::My_Tree_View(QWidget *parent)
    :QTreeView(parent)
{
    setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragDrop);
}
void My_Tree_View::dropEvent(QDropEvent *event)
{
    if (m_allow_drop && *m_allow_drop && **m_allow_drop && event->mimeData()->hasUrls() && F_model)
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
        this->setRootIndex(this->F_model->index(filename));
    }
}
void My_Tree_View::dragEnterEvent(QDragEnterEvent *event)
{
    if (m_allow_drop && *m_allow_drop && **m_allow_drop && event->mimeData()->hasUrls())
    {
        event->accept();
    }
}
void My_Tree_View::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && F_model)
    {
        if (selectionModel() && selectionModel()->currentIndex().isValid())
        {
            QList<QUrl> urls;
            urls.append(QUrl::fromLocalFile(F_model->filePath(selectionModel()->currentIndex())));
            QMimeData *mimeData = new QMimeData;
            mimeData->setUrls(urls);
            QDrag *drag=new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(F_model->fileIcon(selectionModel()->currentIndex()).pixmap(50, 50));
            drag->setHotSpot(QPoint(13,13));
            drag->exec(Qt::CopyAction, Qt::CopyAction);
        }
    }
}
