#include "file_tree.h"
#include <QX11Info>
#include <X11/Xlib.h>
My_Tree_View * My_Tree_View::catch_ptr;
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
    treeView->setMouseTracking(true);
    treeView->setTabletTracking(true);
    treeView->setDragDropMode(QAbstractItemView::DragDrop);
    treeView->F_model = model;
    treeView->proxyModel = proxyModel;
    show();
    this->background_color = QColor(255,255,255,50);
    Update_Background();
    menu->addAction(open_it);
    menu->addAction(open_way);
    menu->addAction(open_path_way);
    menu->addAction(show_info);
    menu->addAction(copy_action);
    menu->addAction(clean_selection_action);
    single_press_mode_action->setIcon(QIcon(":/base/this.svg"));
    single_press_mode_action->setIconVisibleInMenu(false);
    tree_setting->addAction(single_press_mode_action);
    tree_setting->addAction(set_dir_path);
    tree_setting->addAction(refresh_action);
    set_style_menu->addAction(set_icon_size_action);
    set_style_menu->addAction(set_font_action);
    set_style_menu->addAction(set_select_radius);
    set_style_menu->addAction(set_hover_color);
    set_style_menu->addAction(set_select_color);
    tree_setting->addMenu(set_style_menu);
    menu->addMenu(tree_setting);
    basic_context(menu);
    shortcut_copy_action->setShortcut(QKeySequence::Copy);
    shortcut_copy_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_copy_action, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Tree_View::catch_ptr)
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QList<QUrl> urls;
                QByteArray gnomeData;
                gnomeData.append("copy\n");
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                    urls.append(QUrl::fromLocalFile(this_file_path));
                    gnomeData.append(QUrl::fromLocalFile(this_file_path).toEncoded() + "\n");
                }
                QMimeData *mimeData = new QMimeData;
                mimeData->setUrls(urls);
                mimeData->setData("x-special/gnome-copied-files", gnomeData);//添加此项用于激活dde-file-manager的"粘贴"
                QApplication::clipboard()->setMimeData(mimeData, QClipboard::Mode::Clipboard);
            }
        }
    });
    shortcut_show_info->setShortcut(Qt::Key_Space);
    shortcut_show_info->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_show_info, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Tree_View::catch_ptr)
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QString this_file_path = root_path;
                QProcess process;
                process.setProgram("/bin/bash");
                process.setWorkingDirectory(QDir::homePath());
                QString m_process_str = "dde-file-manager -p";
                if (file_open_info_process)
                {
                    m_process_str = *file_open_info_process;
                }
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QString this_file_path = model->filePath(selectedList[i]);
                    m_process_str += " ";
                    m_process_str += '"';
                    m_process_str += this_file_path;
                    m_process_str += '"';
                }
                process.setArguments(QStringList() << "-c" << m_process_str);
                process.setStandardOutputFile("/dev/null");
                process.setStandardErrorFile("/dev/null");
                process.startDetached();
            }
        }
    });
    shortcut_enter->setShortcut(Qt::Key_Return);
    shortcut_enter->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_enter, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Tree_View::catch_ptr)
        {
            Pressed(true);
        }
    });
    treeView->addAction(shortcut_copy_action);
    treeView->addAction(shortcut_show_info);
    treeView->addAction(shortcut_enter);
    carrier_widget->move(10, 10);
    search_edit->move(0, 5);
    treeView->move(0, 50);
    carrier_widget->setStyleSheet("QWidget{background:rgba(0,0,0,0);color:rgb(230,230,230)}");
    search_edit->setStyleSheet("QLineEdit{border: 0px solid rgba(0,170,255,255);border-radius:10px 10px;background:rgba(0,0,0,25);font-size:15px;color:rgb(40,40,40)}"
                               "QLineEdit:hover{border: 1px solid rgba(0,170,255,255)}");
    treeView->setAlternatingRowColors(true);
    treeView->setLayoutDirection(Qt::LeftToRight);
    treeView->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    treeView->setEditTriggers(QListView::NoEditTriggers);
    treeView->setItemDelegate(my_delegate);
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
    proxyModel->setSourceModel(model);
    treeView->setModel(proxyModel);
    treeView->setIconSize(QSize(24, 24));
    treeView->setIndentation(24);
    treeView->setRootIndex(proxyModel->mapFromSource(model->index(QDir::rootPath())));
    treeView->setAnimated(true);
    treeView->setSortingEnabled(true);
    treeView->header()->setSortIndicator(0, Qt::SortOrder::AscendingOrder);
    treeView->setColumnWidth(0, 150);
    carrier_widget->setMouseTracking(true);
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
        if (treeView->selectionModel())
        {
            treeView->selectionModel()->clear();
        }
        if (search_edit->text().isEmpty())
        {
            search_edit->removeAction(search_del_action);
            proxyModel->setSearchPattern("");
        }
        else
        {
            search_edit->addAction(search_del_action,QLineEdit::TrailingPosition);
            proxyModel->setSearchPattern(search_edit->text());
        }
    });
    connect(this, &Basic_Widget::size_changed, this, [=](QSize size)
    {
        carrier_widget->resize(size - QSize(20, 20));
        search_edit->resize(size.width() - 20, 40);
        treeView->resize(size - QSize(20, 70));
    });
    connect(this->treeView, &QTreeView::clicked, this, [=]
    {
        if (single_press_mode_action->isIconVisibleInMenu())
        {
            Pressed();
        }
    });
    connect(this->treeView, &QTreeView::doubleClicked, this, [=]
    {
        if (!single_press_mode_action->isIconVisibleInMenu())
        {
            Pressed();
        }
    });
    resize(600, 300);
    show();
}
void File_Tree::Pressed(bool from_key)
{
    if (treeView->selectionModel())
    {
        QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
        if (!selectedList.isEmpty())
        {
            for (int i = 0; i < selectedList.count(); i += 4)
            {
                if (model->hasChildren(proxyModel->mapToSource(selectedList[i])))
                {
                    if (single_press_mode_action->isIconVisibleInMenu() || from_key)
                    {
                        if (treeView->isExpanded(selectedList[i]))
                        {
                            treeView->setExpanded(selectedList[i], false);
                        }
                        else
                        {
                            treeView->expand(selectedList[i]);
                        }
                    }
                    return;
                }
                QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
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
                        m_running_process.replace("%f", QDir::toNativeSeparators('"' + this_file_path + '"'));
                        m_running_process.replace("%F", QDir::toNativeSeparators('"' + this_file_path + '"'));
                        m_running_process.replace("%u", QDir::toNativeSeparators('"' + this_file_path + '"'));
                        m_running_process.replace("%U", QDir::toNativeSeparators('"' + this_file_path + '"'));
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
        }
    }
}
void File_Tree::set_tree_view_style()
{
    treeView->setStyleSheet(QString("QTreeView{background:rgba(255,255,255,0);color:rgb(60,60,60);selection-background-color:rgba(0,0,0,0);}"
                                    "QTreeView::item:first{color:rgb(0,0,0)}"
                                    "QTreeView::item:selected{color:rgb(255,255,255);border: 0px solid rgba(255,255,255,0)}"));
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
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            QStringList file_list = {};
            if (!selectedList.isEmpty())
            {
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                    file_list << this_file_path;
                }
            }
            else
            {
                file_list << root_path;
            }
            for (int i = 0; i < file_list.count(); i++)
            {
                QString this_file_path = file_list[i];
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
                    m_running_process.replace("%f", QDir::toNativeSeparators('"' + this_file_path + '"'));
                    m_running_process.replace("%F", QDir::toNativeSeparators('"' + this_file_path + '"'));
                    m_running_process.replace("%u", QDir::toNativeSeparators('"' + this_file_path + '"'));
                    m_running_process.replace("%U", QDir::toNativeSeparators('"' + this_file_path + '"'));
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
    }
    else if (know_what == open_way)
    {
        if (treeView->selectionModel())
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                    QProcess process;
                    process.setProgram("/bin/bash");
                    process.setWorkingDirectory(this_file_path);
                    QString m_process_str = "dde-file-manager -d -o";
                    if (file_open_way_process)
                    {
                        m_process_str = *file_open_way_process;
                    }
                    m_process_str += " ";
                    m_process_str += '"';
                    m_process_str += this_file_path;
                    m_process_str += '"';
                    process.setArguments(QStringList() << "-c" << m_process_str);
                    process.setStandardOutputFile("/dev/null");
                    process.setStandardErrorFile("/dev/null");
                    process.startDetached();
                }
            }
            else
            {
                QString this_file_path = root_path;
                QProcess process;
                process.setProgram("/bin/bash");
                process.setWorkingDirectory(this_file_path);
                QString m_process_str = "dde-file-manager -d -o";
                if (file_open_way_process)
                {
                    m_process_str = *file_open_way_process;
                }
                m_process_str += " ";
                m_process_str += '"';
                m_process_str += this_file_path;
                m_process_str += '"';
                process.setArguments(QStringList() << "-c" << m_process_str);
                process.setStandardOutputFile("/dev/null");
                process.setStandardErrorFile("/dev/null");
                process.startDetached();
            }
        }
    }
    else if (know_what == open_path_way)
    {
        if (treeView->selectionModel())
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                    QProcess process;
                    process.setProgram("/bin/bash");
                    process.setWorkingDirectory(this_file_path);
                    QString m_process_str = "dde-file-manager --show-item";
                    if (file_open_path_process)
                    {
                        m_process_str = *file_open_path_process;
                    }
                    m_process_str += " ";
                    m_process_str += '"';
                    m_process_str += this_file_path;
                    m_process_str += '"';
                    process.setArguments(QStringList() << "-c" << m_process_str);
                    process.setStandardOutputFile("/dev/null");
                    process.setStandardErrorFile("/dev/null");
                    process.startDetached();
                }
            }
            else
            {
                QString this_file_path = root_path;
                QProcess process;
                process.setProgram("/bin/bash");
                process.setWorkingDirectory(this_file_path);
                QString m_process_str = "dde-file-manager --show-item";
                if (file_open_path_process)
                {
                    m_process_str = *file_open_path_process;
                }
                m_process_str += " ";
                m_process_str += '"';
                m_process_str += this_file_path;
                m_process_str += '"';
                process.setArguments(QStringList() << "-c" << m_process_str);
                process.setStandardOutputFile("/dev/null");
                process.setStandardErrorFile("/dev/null");
                process.startDetached();
            }
        }
    }
    else if (know_what == show_info)
    {
        if (treeView->selectionModel())
        {
            QString this_file_path = root_path;
            QProcess process;
            process.setProgram("/bin/bash");
            process.setWorkingDirectory(QDir::homePath());
            QString m_process_str = "dde-file-manager -p";
            if (file_open_info_process)
            {
                m_process_str = *file_open_info_process;
            }
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                    m_process_str += " ";
                    m_process_str += '"';
                    m_process_str += this_file_path;
                    m_process_str += '"';
                }
            }
            else
            {
                m_process_str += " ";
                m_process_str += '"';
                m_process_str += this_file_path;
                m_process_str += '"';
            }
            process.setArguments(QStringList() << "-c" << m_process_str);
            process.setStandardOutputFile("/dev/null");
            process.setStandardErrorFile("/dev/null");
            process.startDetached();
        }
    }
    else if (know_what == copy_action)
    {
        if (treeView->selectionModel())
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            QList<QUrl> urls;
            QByteArray gnomeData;
            gnomeData.append("copy\n");
            if (!selectedList.isEmpty())
            {
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                    urls.append(QUrl::fromLocalFile(this_file_path));
                    gnomeData.append(QUrl::fromLocalFile(this_file_path).toEncoded() + "\n");
                }
            }
            else
            {
                urls.append(QUrl::fromLocalFile(root_path));
                gnomeData.append(urls.first().toEncoded() + "\n");
            }
            QMimeData *mimeData = new QMimeData;
            mimeData->setUrls(urls);
            mimeData->setData("x-special/gnome-copied-files", gnomeData);//添加此项用于激活dde-file-manager的"粘贴"
            QApplication::clipboard()->setMimeData(mimeData, QClipboard::Mode::Clipboard);
        }
    }
    else if (know_what == single_press_mode_action)
    {
        single_press_mode_action->setIconVisibleInMenu(!single_press_mode_action->isIconVisibleInMenu());
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
        treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
        if (treeView->selectionModel())
        {
            treeView->selectionModel()->clear();
        }
    }
    else if (know_what == refresh_action)
    {
        treeView->reset();
        treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
        if (treeView->selectionModel())
        {
            treeView->selectionModel()->clear();
        }
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
    else if (know_what == set_font_action)
    {
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, treeView->font(), nullptr);
        if (!ok)
        {
            return;
        }
        treeView->setFont(font);
    }
    else if (know_what == set_select_radius)
    {
        bool ok = false;
        int num = QInputDialog::getInt(nullptr, "获取数值", "大小:", radius, 0, 2147483647, 1, &ok);
        if (ok)
        {
            radius = num;
            set_tree_view_style();
        }
    }
    else if (know_what == set_hover_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
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
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
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
    else if (know_what == clean_selection_action)
    {
        if (treeView->selectionModel())
        {
            treeView->selectionModel()->clear();
        }
    }
    else
    {
        basic_action_func(know_what);
    }
}
void File_Tree::dropEvent(QDropEvent *event)
{
    if (*m_allow_drop && event->mimeData()->hasUrls() && !treeView->move_copying)
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
        treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
        if (treeView->selectionModel())
        {
            treeView->selectionModel()->clear();
        }
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
    if (treeView == My_Tree_View::catch_ptr)
    {
        event->accept();
    }
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
    settings->setValue("text_font", treeView->font());
    settings->setValue("hover_color", hover_color.rgba());
    settings->setValue("select_color", select_color.rgba());
    settings->setValue("single_press_mode", single_press_mode_action->isIconVisibleInMenu());
    settings->setValue("treeview_radius", radius);
}
void File_Tree::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    root_path = settings->value("root_path", QDir::rootPath()).toString();
    treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
    treeView->setColumnWidth(0, settings->value("column_width0", 150).toInt());
    treeView->setColumnWidth(1, settings->value("column_width1", 150).toInt());
    treeView->setColumnWidth(2, settings->value("column_width2", 150).toInt());
    treeView->setColumnWidth(3, settings->value("column_width3", 150).toInt());
    treeView->header()->setSortIndicator(settings->value("sort_section", 0).toInt(), settings->value("sort_order", true).toBool() ? Qt::SortOrder::AscendingOrder : Qt::SortOrder::DescendingOrder);
    int indentation_num = settings->value("indentation", 24).toInt();
    treeView->setIconSize(QSize(indentation_num, indentation_num));
    treeView->setIndentation(indentation_num);
    treeView->setFont(settings->value("text_font", QFontDatabase::systemFont(QFontDatabase::FixedFont)).value<QFont>());
    hover_color = QColor::fromRgba(settings->value("hover_color", QColor(227, 242, 253, 255).rgb()).toUInt());
    select_color = QColor::fromRgba(settings->value("select_color", QColor(0, 170, 255, 255).rgb()).toUInt());
    bool single_press_mode = settings->value("single_press_mode", false).toBool();
    single_press_mode_action->setIconVisibleInMenu(single_press_mode);
    radius = settings->value("treeview_radius", 10).toInt();
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
    QWidget::dropEvent(event);
}
void My_Tree_View::dragEnterEvent(QDragEnterEvent *event)
{
    QWidget::dragEnterEvent(event);
}
void My_Tree_View::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && F_model && proxyModel)
    {
        if (selectionModel())
        {
            QModelIndexList selectedList = selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                move_copying = true;
                QList<QUrl> urls;
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    urls.append(QUrl::fromLocalFile(F_model->filePath(proxyModel->mapToSource(selectedList[i]))));
                }
                QMimeData *mimeData = new QMimeData;
                mimeData->setUrls(urls);
                QDrag *drag=new QDrag(this);
                drag->setMimeData(mimeData);
                drag->setPixmap(F_model->fileIcon(proxyModel->mapToSource(selectedList[0])).pixmap(50, 50));
                drag->setHotSpot(QPoint(13,13));
                drag->exec(Qt::CopyAction, Qt::CopyAction);
            }
        }
    }
    QWidget::mouseMoveEvent(event);
}
void My_Tree_View::enterEvent(QEvent *event)
{
    My_Tree_View::catch_ptr = this;
    QWidget::enterEvent(event);
}
My_TreeView_Delegate::My_TreeView_Delegate(QObject *parent, QColor *m_hover_color, QColor *m_select_color, int *m_radius)
    :QStyledItemDelegate(parent)
    ,hover_color(m_hover_color)
    ,select_color(m_select_color)
    ,radius(m_radius)
{}
void My_TreeView_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    if (opt.state & QStyle::State_MouseOver || opt.state & QStyle::State_Selected)
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        int index_id = index.column();
        if (opt.state & QStyle::State_MouseOver)
        {
            QColor hoverColor(227, 242, 253, 255);
            if (hover_color) hoverColor = *hover_color;
            int Radius = 10;
            if (radius) Radius = *radius;
            painter->setBrush(hoverColor);
            painter->setPen(Qt::NoPen);
            if (index_id == 0)
            {
                QPainterPath path;
                QRectF rect = opt.rect;
                path.moveTo(rect.x() + Radius, rect.y());
                path.lineTo(rect.right(), rect.y());
                path.lineTo(rect.right(), rect.bottom());
                path.lineTo(rect.x() + Radius, rect.bottom());
                path.quadTo(rect.bottomLeft(), QPointF(rect.x(), rect.bottom() - Radius));
                path.lineTo(rect.x(), rect.y() + Radius);
                path.quadTo(rect.topLeft(), QPointF(rect.x() + Radius, rect.y()));
                path.closeSubpath();
                painter->drawPath(path);
            }
            else if (index_id == 3)
            {
                QPainterPath path;
                QRectF rect = opt.rect;
                path.moveTo(rect.topLeft());
                QPointF topRight = rect.topRight();
                path.lineTo(topRight.x() - Radius, topRight.y());
                path.quadTo(topRight, QPointF(topRight.x(), topRight.y() + Radius));
                QPointF bottomRight = rect.bottomRight();
                path.lineTo(bottomRight.x(), bottomRight.y() - Radius);
                path.quadTo(bottomRight, QPointF(bottomRight.x() - Radius, bottomRight.y()));
                path.lineTo(rect.bottomLeft());
                path.closeSubpath();
                painter->drawPath(path);
            }
            else
            {
                painter->drawRect(opt.rect);
            }
        }
        if (opt.state & QStyle::State_Selected)
        {
            QColor selectColor(0, 170, 255, 255);
            if (select_color) selectColor = *select_color;
            int Radius = 10;
            if (radius) Radius = *radius;
            painter->setBrush(selectColor);
            painter->setPen(Qt::NoPen);
            if (index_id == 0)
            {
                QPainterPath path;
                QRectF rect = opt.rect;
                path.moveTo(rect.x() + Radius, rect.y());
                path.lineTo(rect.right(), rect.y());
                path.lineTo(rect.right(), rect.bottom());
                path.lineTo(rect.x() + Radius, rect.bottom());
                path.quadTo(rect.bottomLeft(), QPointF(rect.x(), rect.bottom() - Radius));
                path.lineTo(rect.x(), rect.y() + Radius);
                path.quadTo(rect.topLeft(), QPointF(rect.x() + Radius, rect.y()));
                path.closeSubpath();
                painter->drawPath(path);
            }
            else if (index_id == 3)
            {
                QPainterPath path;
                QRectF rect = opt.rect;
                path.moveTo(rect.topLeft());
                QPointF topRight = rect.topRight();
                path.lineTo(topRight.x() - Radius, topRight.y());
                path.quadTo(topRight, QPointF(topRight.x(), topRight.y() + Radius));
                QPointF bottomRight = rect.bottomRight();
                path.lineTo(bottomRight.x(), bottomRight.y() - Radius);
                path.quadTo(bottomRight, QPointF(bottomRight.x() - Radius, bottomRight.y()));
                path.lineTo(rect.bottomLeft());
                path.closeSubpath();
                painter->drawPath(path);
            }
            else
            {
                painter->drawRect(opt.rect);
            }
        }
        opt.backgroundBrush = Qt::NoBrush;
        painter->restore();
    }
    QStyledItemDelegate::paint(painter, opt, index);
}
My_ProxyModel::My_ProxyModel(QObject *parent)
    :QSortFilterProxyModel(parent)
{}
void My_ProxyModel::setSearchPattern(const QString &pattern)
{
    m_pattern = pattern;
    invalidateFilter();
}
bool My_ProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex sourceIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    if (!sourceIndex.isValid())
    {
        return false;
    }
    QFileSystemModel *fileSystemModel = qobject_cast<QFileSystemModel *>(sourceModel());
    if (fileSystemModel)
    {
        QFileInfo fileInfo = fileSystemModel->fileInfo(sourceIndex);
        if (fileInfo.isDir())
        {
            return true;
        }
    }
    if (m_pattern.isEmpty())
    {
        return true;
    }
    QString fileName = sourceIndex.data().toString();
    return fileName.contains(m_pattern, Qt::CaseInsensitive);
}
void My_ProxyModel::sort(int column, Qt::SortOrder order)
{
    if (QFileSystemModel *fsModel = qobject_cast<QFileSystemModel*>(sourceModel()))
    {
        fsModel->sort(column, order);
    }
    else
    {
        QSortFilterProxyModel::sort(column, order);
    }
    invalidate();
}
