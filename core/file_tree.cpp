#include "file_tree.h"
#include "core/tools/file_control.h"
My_Tree_View * My_Tree_View::catch_ptr;
void File_Tree::set_icon(QString checked_icon_path)
{
    single_press_mode_action->setIcon(QIcon(checked_icon_path));
    show_hidden_action->setIcon(QIcon(checked_icon_path));
    Basic_Widget::set_icon(checked_icon_path);
}
File_Tree::File_Tree(QWidget *parent)
    :Basic_Widget(parent)
{
    setAcceptDrops(true);
    treeView->F_model = model;
    treeView->proxyModel = proxyModel;
    show();
    this->background_color = QColor(255,255,255,50);
    Update_Background();
    menu->addAction(open_it);
    menu->addSeparator();
    menu->addAction(open_way);
    menu->addAction(open_path_way);
    menu->addSeparator();
    menu->addAction(open_in_Terminal_action);
    compress_menu->addAction(compressor_action);
    compress_menu->addAction(save_as_zip_action);
    compress_menu->addAction(save_as_7z_action);
    menu->addMenu(compress_menu);
    menu->addSeparator();
    create_menu->addAction(create_new_file);
    create_menu->addAction(create_new_folder);
    menu->addMenu(create_menu);
    control_menu->addAction(set_as_path_way);
    control_menu->addAction(set_parent_as_path_way);
    control_menu->addSeparator();
    show_hidden_action->setIcon(QIcon(":/base/this.svg"));
    show_hidden_action->setIconVisibleInMenu(false);
    control_menu->addAction(show_hidden_action);
    control_menu->addAction(select_all_action);
    control_menu->addAction(clean_selection_action);
    control_menu->addAction(refresh_action);
    menu->addMenu(control_menu);
    menu->addSeparator();
    menu->addAction(cut_action);
    menu->addAction(copy_action);
    menu->addAction(paste_action);
    menu->addAction(rename_action);
    menu->addAction(delete_action);
    menu->addSeparator();
    menu->addAction(show_info);
    menu->addSeparator();
    single_press_mode_action->setIcon(QIcon(":/base/this.svg"));
    single_press_mode_action->setIconVisibleInMenu(false);
    tree_setting->addAction(single_press_mode_action);
    tree_setting->addAction(set_dir_path);
    set_style_menu->addAction(set_icon_size_action);
    set_style_menu->addAction(set_font_action);
    set_style_menu->addAction(set_select_radius);
    set_style_menu->addAction(set_hover_color);
    set_style_menu->addAction(set_select_color);
    tree_setting->addMenu(set_style_menu);
    menu->addMenu(tree_setting);
    basic_context(menu);
    //shortcut
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
                    QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                    m_process_str += " ";
                    m_process_str += File_Control::FilenameForBash(this_file_path);
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
    shortcut_cut_action->setShortcut(QKeySequence::Cut);
    shortcut_cut_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_cut_action, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Tree_View::catch_ptr)
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QList<QUrl> urls;
                QByteArray gnomeData;
                gnomeData.append("cut\n");
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
    shortcut_paste_action->setShortcut(QKeySequence::Paste);
    shortcut_paste_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_paste_action, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Tree_View::catch_ptr)
        {
            const QMimeData *mimeData = QApplication::clipboard()->mimeData();
            QList<QUrl> urls = mimeData->urls();
            if (urls.isEmpty())
            {
                return;
            }
            bool is_cut = false;
            if (mimeData->hasFormat("x-special/gnome-copied-files"))
            {
                QByteArray gnomeData = mimeData->data("x-special/gnome-copied-files");
                if (!gnomeData.isEmpty())
                {
                    QList<QByteArray> lines = gnomeData.split('\n');
                    if (lines.size() > 0)
                    {
                        if (lines[0] == "cut")
                        {
                            is_cut = true;
                        }
                    }
                }
            }
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            QFileInfo to_file_info(root_path);
            if (!selectedList.isEmpty())
            {
                for (int i = 0; i < selectedList.count(); i++)
                {
                    QFileInfo file_info(model->filePath(proxyModel->mapToSource(selectedList[i])));
                    if (file_info.isDir())
                    {
                        to_file_info = file_info;
                    }
                    else
                    {
                        to_file_info = QFileInfo(file_info.dir().path());
                    }
                    QString targetDir = to_file_info.filePath();
                    for (const QUrl& url : urls)
                    {
                        QString srcPath = url.toLocalFile();
                        if (srcPath.isEmpty())
                        {
                            continue;
                        }
                        QFileInfo srcInfo(srcPath);
                        QString destPath = targetDir + QDir::separator() + srcInfo.fileName();
                        int copy_file_asking = -1;
                        File_Control::Copy_File(srcPath, destPath, is_cut, &copy_file_asking);
                        QModelIndex idx = proxyModel->mapFromSource(model->index(destPath));
                        if (idx.isValid())
                        {
                            model->data(idx, Qt::DisplayRole);
                            treeView->update(idx);
                        }
                    }
                }
            }
        }
    });
    shortcut_find_action->setShortcut(QKeySequence::Find);
    shortcut_find_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_find_action, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Tree_View::catch_ptr)
        {
            search_edit->setFocus();
        }
    });
    shortcut_delete_action->setShortcut(Qt::Key_Delete);
    shortcut_delete_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_delete_action, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Tree_View::catch_ptr)
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QList<QUrl> urls;
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                    urls.append(QUrl::fromLocalFile(this_file_path));
                }
                for (const QUrl& url : urls)
                {
                    QString srcPath = url.toLocalFile();
                    if (srcPath.isEmpty())
                    {
                        continue;
                    }
                    QFile::moveToTrash(srcPath);
                }
            }
        }
    });
    shortcut_force_delete_action->setShortcut(QKeySequence("Shift+Del"));
    shortcut_force_delete_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_force_delete_action, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Tree_View::catch_ptr)
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QList<QUrl> urls;
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                    urls.append(QUrl::fromLocalFile(this_file_path));
                }
                for (const QUrl& url : urls)
                {
                    QString srcPath = url.toLocalFile();
                    if (srcPath.isEmpty())
                    {
                        continue;
                    }
                    QFileInfo srcInfo(srcPath);
                    if (srcInfo.isDir())
                    {
                        QDir(srcPath).removeRecursively();
                    }
                    else
                    {
                        QFile::remove(srcPath);
                    }
                }
            }
        }
    });
    shortcut_show_hidden_action->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_H));
    shortcut_show_hidden_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_show_hidden_action, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Tree_View::catch_ptr)
        {
            show_hidden_action->setIconVisibleInMenu(!show_hidden_action->isIconVisibleInMenu());
            proxyModel->setShowHidden(show_hidden_action->isIconVisibleInMenu());
        }
    });
    shortcut_rename_action->setShortcut(Qt::Key_F2);
    shortcut_rename_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_rename_action, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Tree_View::catch_ptr)
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QString name_list_str = "";
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    if (!name_list_str.isEmpty())
                    {
                        name_list_str += "\n";
                    }
                    name_list_str += model->fileName(proxyModel->mapToSource(selectedList[i]));
                }
                QInputDialog dialog;
                dialog.setParent(nullptr);
                dialog.setWindowTitle("重命名");
                dialog.setLabelText("获取新名称:(请勿添加\\n)");
                dialog.setTextValue(name_list_str);
                dialog.setInputMode(QInputDialog::TextInput);
                dialog.setOption(QInputDialog::UsePlainTextEditForTextInput);
                dialog.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
                QPlainTextEdit *textEdit = dialog.findChild<QPlainTextEdit*>();
                if (textEdit)
                {
                    textEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard | Qt::TextEditable);
                }
                if (dialog.exec() != QDialog::Accepted)
                {
                    return;
                }
                QStringList name_list = name_list_str.split("\n");
                if (name_list.count() != selectedList.count() / 4)
                {
                    return;
                }
                model->setReadOnly(false);
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QModelIndex proxyIndex = selectedList[i];
                    QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
                    model->setData(sourceIndex, name_list[i / 4], Qt::EditRole);
                }
                model->setReadOnly(true);
            }
        }
    });
    treeView->addAction(shortcut_copy_action);
    treeView->addAction(shortcut_show_info);
    treeView->addAction(shortcut_enter);
    treeView->addAction(shortcut_cut_action);
    treeView->addAction(shortcut_paste_action);
    treeView->addAction(shortcut_delete_action);
    treeView->addAction(shortcut_force_delete_action);
    treeView->addAction(shortcut_find_action);
    treeView->addAction(shortcut_show_hidden_action);
    //shortcut
    carrier_widget->move(10, 10);
    search_edit->move(0, 5);
    treeView->move(0, 50);
    carrier_widget->setStyleSheet("QWidget{background:rgba(0,0,0,0);color:rgb(230,230,230)}");
    search_edit->setStyleSheet("QLineEdit{border: 0px solid rgba(0,170,255,255);border-radius:10px 10px;background:rgba(0,0,0,25);font-size:15px;color:rgb(40,40,40)}"
                               "QLineEdit:hover{border: 1px solid rgba(0,170,255,255)}");
    treeView->setAlternatingRowColors(true);
    treeView->setLayoutDirection(Qt::LeftToRight);
    treeView->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
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
    model->setFilter(QDir::AllEntries | QDir::NoDotAndDotDot | QDir::Hidden);
    model->setRootPath(QDir::rootPath());
    model->setIconProvider(icon_provider);
    proxyModel->setSourceModel(model);
    proxyModel->setShowHidden(false);
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
                        m_running_process.replace("%f", QDir::toNativeSeparators(File_Control::FilenameForBash(this_file_path)));
                        m_running_process.replace("%F", QDir::toNativeSeparators(File_Control::FilenameForBash(this_file_path)));
                        m_running_process.replace("%u", QDir::toNativeSeparators(File_Control::FilenameForBash(this_file_path)));
                        m_running_process.replace("%U", QDir::toNativeSeparators(File_Control::FilenameForBash(this_file_path)));
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
    paste_action->setEnabled(!QApplication::clipboard()->mimeData()->urls().isEmpty());
    if (treeView->selectionModel())
    {
        bool has_selected = treeView->selectionModel()->selectedIndexes().isEmpty();
        cut_action->setEnabled(!has_selected);
        clean_selection_action->setEnabled(!has_selected);
        rename_action->setEnabled(!has_selected);
        delete_action->setEnabled(!has_selected);
        set_as_path_way->setEnabled(!has_selected);
        compress_menu->setEnabled(!has_selected);
    }
    set_parent_as_path_way->setEnabled(!QDir(root_path).isRoot());
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
                    m_running_process.replace("%f", QDir::toNativeSeparators(File_Control::FilenameForBash(this_file_path)));
                    m_running_process.replace("%F", QDir::toNativeSeparators(File_Control::FilenameForBash(this_file_path)));
                    m_running_process.replace("%u", QDir::toNativeSeparators(File_Control::FilenameForBash(this_file_path)));
                    m_running_process.replace("%U", QDir::toNativeSeparators(File_Control::FilenameForBash(this_file_path)));
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
                    m_process_str += File_Control::FilenameForBash(this_file_path);
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
                m_process_str += File_Control::FilenameForBash(this_file_path);
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
                    m_process_str += File_Control::FilenameForBash(this_file_path);
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
                m_process_str += File_Control::FilenameForBash(this_file_path);
                process.setArguments(QStringList() << "-c" << m_process_str);
                process.setStandardOutputFile("/dev/null");
                process.setStandardErrorFile("/dev/null");
                process.startDetached();
            }
        }
    }
    else if (know_what == create_new_file)
    {
        if (treeView->selectionModel())
        {
            QInputDialog dialog;
            dialog.setParent(nullptr);
            dialog.setWindowTitle("新建文件");
            dialog.setLabelText("获取文件名:(一行一文件)");
            dialog.setTextValue("new_file");
            dialog.setInputMode(QInputDialog::TextInput);
            dialog.setOption(QInputDialog::UsePlainTextEditForTextInput);
            dialog.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
            QPlainTextEdit *textEdit = dialog.findChild<QPlainTextEdit*>();
            if (textEdit)
            {
                textEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard | Qt::TextEditable);
            }
            if (dialog.exec() != QDialog::Accepted)
            {
                return;
            }
            QStringList name_list = dialog.textValue().split("\n");
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            QString tmp_file_top_path = "";
            if (!selectedList.isEmpty())
            {
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                    if (QFileInfo(this_file_path).isDir())
                    {
                        tmp_file_top_path = this_file_path;
                        break;
                    }
                }
            }
            else
            {
                tmp_file_top_path = root_path;
            }
            QString file_top_path = QFileInfo(tmp_file_top_path).filePath();
            for (int i = 0; i < name_list.count(); i++)
            {
                QString full_path = file_top_path + QDir::separator() + name_list[i];
                QFile file(full_path);
                if (file.exists())
                {
                    full_path += ".copy";
                    file.setFileName(full_path);
                }
                unsigned int copy_add_num = 0;
                QString sec_dst_path = full_path;
                while (file.exists())
                {
                    copy_add_num++;
                    full_path = sec_dst_path + QString::number(copy_add_num);
                    file.setFileName(full_path);
                    if (copy_add_num == 0)
                    {
                        return;
                    }
                }
                file.open(QIODevice::WriteOnly);
                file.close();
            }
        }
    }
    else if (know_what == create_new_folder)
    {
        if (treeView->selectionModel())
        {
            QInputDialog dialog;
            dialog.setParent(nullptr);
            dialog.setWindowTitle("新建文件夹");
            dialog.setLabelText("获取文件夹名:(一行一文件夹)");
            dialog.setTextValue("new_folder");
            dialog.setInputMode(QInputDialog::TextInput);
            dialog.setOption(QInputDialog::UsePlainTextEditForTextInput);
            dialog.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
            QPlainTextEdit *textEdit = dialog.findChild<QPlainTextEdit*>();
            if (textEdit)
            {
                textEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard | Qt::TextEditable);
            }
            if (dialog.exec() != QDialog::Accepted)
            {
                return;
            }
            QStringList name_list = dialog.textValue().split("\n");
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            QString tmp_file_top_path = "";
            if (!selectedList.isEmpty())
            {
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                    if (QFileInfo(this_file_path).isDir())
                    {
                        tmp_file_top_path = this_file_path;
                        break;
                    }
                }
            }
            else
            {
                tmp_file_top_path = root_path;
            }
            QString file_top_path = QFileInfo(tmp_file_top_path).filePath();
            for (int i = 0; i < name_list.count(); i++)
            {
                QString full_path = file_top_path + QDir::separator() + name_list[i];
                QFileInfo file(full_path);
                if (file.exists())
                {
                    full_path += ".copy";
                    file.setFile(full_path);
                }
                unsigned int copy_add_num = 0;
                QString sec_dst_path = full_path;
                while (file.exists())
                {
                    copy_add_num++;
                    full_path = sec_dst_path + QString::number(copy_add_num);
                    file.setFile(full_path);
                    if (copy_add_num == 0)
                    {
                        return;
                    }
                }
                QDir dir;
                dir.mkdir(full_path);
            }
        }
    }
    else if (know_what == set_as_path_way)
    {
        if (treeView->selectionModel())
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList.first()));
                QFileInfo fileinfo(this_file_path);
                if (!fileinfo.isDir())
                {
                    fileinfo = QFileInfo(fileinfo.dir().path());
                }
                root_path = fileinfo.filePath();
                treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
                if (treeView->selectionModel())
                {
                    treeView->selectionModel()->clear();
                }
            }
        }
    }
    else if (know_what == set_parent_as_path_way)
    {
        if (treeView->selectionModel())
        {
            QDir m_dir(root_path);
            if (m_dir.cdUp())
            {
                root_path = m_dir.absolutePath();
                treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
                if (treeView->selectionModel())
                {
                    treeView->selectionModel()->clear();
                }
            }
        }
    }
    else if (know_what == open_in_Terminal_action)
    {
        if (treeView->selectionModel())
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList.first()));
                QFileInfo file_path_info(this_file_path);
                if (file_path_info.isFile())
                {
                    this_file_path = file_path_info.dir().path();
                }
                QProcess process;
                process.setProgram("/bin/bash");
                process.setWorkingDirectory(this_file_path);
                QString m_process_str = "deepin-terminal -w";
                if (terminal_process)
                {
                    m_process_str = *terminal_process;
                }
                m_process_str += " ";
                m_process_str += File_Control::FilenameForBash(this_file_path);
                process.setArguments(QStringList() << "-c" << m_process_str);
                process.setStandardOutputFile("/dev/null");
                process.setStandardErrorFile("/dev/null");
                process.startDetached();
            }
            else
            {
                QString this_file_path = root_path;
                QProcess process;
                process.setProgram("/bin/bash");
                process.setWorkingDirectory(this_file_path);
                QString m_process_str = "deepin-terminal -w";
                if (terminal_process)
                {
                    m_process_str = *terminal_process;
                }
                m_process_str += " ";
                m_process_str += File_Control::FilenameForBash(this_file_path);
                process.setArguments(QStringList() << "-c" << m_process_str);
                process.setStandardOutputFile("/dev/null");
                process.setStandardErrorFile("/dev/null");
                process.startDetached();
            }
        }
    }
    else if (know_what == compressor_action)
    {
        if (treeView->selectionModel())
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList.first()));
                QFileInfo file_path_info(this_file_path);
                if (file_path_info.isFile())
                {
                    this_file_path = file_path_info.dir().path();
                }
                QProcess process;
                process.setProgram("/bin/bash");
                process.setWorkingDirectory(this_file_path);
                QString m_process_str = "deepin-compressor %F compress";
                if (compressor_process)
                {
                    m_process_str = *compressor_process;
                }
                QString files_str = "";
                for (int i = 0; i < selectedList.count(); i++)
                {
                    files_str += " ";
                    files_str += File_Control::FilenameForBash(model->filePath(proxyModel->mapToSource(selectedList[i])));
                }
                m_process_str.replace("%F", files_str);
                process.setArguments(QStringList() << "-c" << m_process_str);
                process.setStandardOutputFile("/dev/null");
                process.setStandardErrorFile("/dev/null");
                process.startDetached();
            }
            else
            {
                QString this_file_path = root_path;
                QProcess process;
                process.setProgram("/bin/bash");
                process.setWorkingDirectory(this_file_path);
                QString m_process_str = "deepin-compressor %F compress";
                if (compressor_process)
                {
                    m_process_str = *compressor_process;
                }
                m_process_str.replace("%F", File_Control::FilenameForBash(this_file_path));
                process.setArguments(QStringList() << "-c" << m_process_str);
                process.setStandardOutputFile("/dev/null");
                process.setStandardErrorFile("/dev/null");
                process.startDetached();
            }
        }
    }
    else if (know_what == save_as_zip_action)
    {
        if (treeView->selectionModel())
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList.first()));
                QFileInfo file_path_info(this_file_path);
                if (file_path_info.isFile())
                {
                    this_file_path = file_path_info.dir().path();
                }
                QProcess process;
                process.setProgram("/bin/bash");
                process.setWorkingDirectory(this_file_path);
                QString m_process_str = "deepin-compressor %F compress_to_zip";
                if (compressor_zip_process)
                {
                    m_process_str = *compressor_zip_process;
                }
                QString files_str = "";
                for (int i = 0; i < selectedList.count(); i++)
                {
                    files_str += " ";
                    files_str += File_Control::FilenameForBash(model->filePath(proxyModel->mapToSource(selectedList[i])));
                }
                m_process_str.replace("%F", files_str);
                process.setArguments(QStringList() << "-c" << m_process_str);
                process.setStandardOutputFile("/dev/null");
                process.setStandardErrorFile("/dev/null");
                process.startDetached();
            }
            else
            {
                QString this_file_path = root_path;
                QProcess process;
                process.setProgram("/bin/bash");
                process.setWorkingDirectory(this_file_path);
                QString m_process_str = "deepin-compressor %F compress_to_zip";
                if (compressor_zip_process)
                {
                    m_process_str = *compressor_zip_process;
                }
                m_process_str.replace("%F", File_Control::FilenameForBash(this_file_path));
                process.setArguments(QStringList() << "-c" << m_process_str);
                process.setStandardOutputFile("/dev/null");
                process.setStandardErrorFile("/dev/null");
                process.startDetached();
            }
        }
    }
    else if (know_what == save_as_7z_action)
    {
        if (treeView->selectionModel())
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList.first()));
                QFileInfo file_path_info(this_file_path);
                if (file_path_info.isFile())
                {
                    this_file_path = file_path_info.dir().path();
                }
                QProcess process;
                process.setProgram("/bin/bash");
                process.setWorkingDirectory(this_file_path);
                QString m_process_str = "deepin-compressor %F compress_to_7z";
                if (compressor_7z_process)
                {
                    m_process_str = *compressor_7z_process;
                }
                QString files_str = "";
                for (int i = 0; i < selectedList.count(); i++)
                {
                    files_str += " ";
                    files_str += File_Control::FilenameForBash(model->filePath(proxyModel->mapToSource(selectedList[i])));
                }
                m_process_str.replace("%F", files_str);
                process.setArguments(QStringList() << "-c" << m_process_str);
                process.setStandardOutputFile("/dev/null");
                process.setStandardErrorFile("/dev/null");
                process.startDetached();
            }
            else
            {
                QString this_file_path = root_path;
                QProcess process;
                process.setProgram("/bin/bash");
                process.setWorkingDirectory(this_file_path);
                QString m_process_str = "deepin-compressor %F compress_to_7z";
                if (compressor_7z_process)
                {
                    m_process_str = *compressor_7z_process;
                }
                m_process_str.replace("%F", File_Control::FilenameForBash(this_file_path));
                process.setArguments(QStringList() << "-c" << m_process_str);
                process.setStandardOutputFile("/dev/null");
                process.setStandardErrorFile("/dev/null");
                process.startDetached();
            }
        }
    }
    else if (know_what == show_hidden_action)
    {
        if (treeView->selectionModel())
        {
            show_hidden_action->setIconVisibleInMenu(!show_hidden_action->isIconVisibleInMenu());
            proxyModel->setShowHidden(show_hidden_action->isIconVisibleInMenu());
        }
    }
    else if (know_what == select_all_action)
    {
        if (treeView->selectionModel())
        {
            treeView->selectAll();
        }
    }
    else if (know_what == clean_selection_action)
    {
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
        QModelIndex idx = proxyModel->mapFromSource(model->index(root_path));
        if (idx.isValid())
        {
            model->data(idx, Qt::DisplayRole);
            treeView->update(idx);
        }
    }
    else if (know_what == cut_action)
    {
        if (treeView->selectionModel())
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            QList<QUrl> urls;
            QByteArray gnomeData;
            gnomeData.append("cut\n");
            if (!selectedList.isEmpty())
            {
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
    else if (know_what == paste_action)
    {
        const QMimeData *mimeData = QApplication::clipboard()->mimeData();
        QList<QUrl> urls = mimeData->urls();
        if (urls.isEmpty())
        {
            return;
        }
        bool is_cut = false;
        if (mimeData->hasFormat("x-special/gnome-copied-files"))
        {
            QByteArray gnomeData = mimeData->data("x-special/gnome-copied-files");
            if (!gnomeData.isEmpty())
            {
                QList<QByteArray> lines = gnomeData.split('\n');
                if (lines.size() > 0)
                {
                    if (lines[0] == "cut")
                    {
                        is_cut = true;
                    }
                }
            }
        }
        QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
        QFileInfo to_file_info(root_path);
        if (!selectedList.isEmpty())
        {
            for (int i = 0; i < selectedList.count(); i++)
            {
                QFileInfo file_info(model->filePath(proxyModel->mapToSource(selectedList[i])));
                if (file_info.isDir())
                {
                    to_file_info = file_info;
                }
                else
                {
                    to_file_info = QFileInfo(file_info.dir().path());
                }
                QString targetDir = to_file_info.filePath();
                for (const QUrl& url : urls)
                {
                    QString srcPath = url.toLocalFile();
                    if (srcPath.isEmpty())
                    {
                        continue;
                    }
                    QFileInfo srcInfo(srcPath);
                    QString destPath = targetDir + QDir::separator() + srcInfo.fileName();
                    int copy_file_asking = -1;
                    File_Control::Copy_File(srcPath, destPath, is_cut, &copy_file_asking);
                    QModelIndex idx = proxyModel->mapFromSource(model->index(destPath));
                    if (idx.isValid())
                    {
                        model->data(idx, Qt::DisplayRole);
                        treeView->update(idx);
                    }
                }
            }
        }
    }
    else if (know_what == rename_action)
    {
        if (treeView->selectionModel())
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QString name_list_str = "";
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    if (!name_list_str.isEmpty())
                    {
                        name_list_str += "\n";
                    }
                    name_list_str += model->fileName(proxyModel->mapToSource(selectedList[i]));
                }
                QInputDialog dialog;
                dialog.setParent(nullptr);
                dialog.setWindowTitle("重命名");
                dialog.setLabelText("获取新名称:(请勿添加\\n)");
                dialog.setTextValue(name_list_str);
                dialog.setInputMode(QInputDialog::TextInput);
                dialog.setOption(QInputDialog::UsePlainTextEditForTextInput);
                dialog.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
                QPlainTextEdit *textEdit = dialog.findChild<QPlainTextEdit*>();
                if (textEdit)
                {
                    textEdit->setTextInteractionFlags(Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard | Qt::TextEditable);
                }
                if (dialog.exec() != QDialog::Accepted)
                {
                    return;
                }
                name_list_str = dialog.textValue();
                QStringList name_list = name_list_str.split("\n");
                if (name_list.count() != selectedList.count() / 4)
                {
                    return;
                }
                model->setReadOnly(false);
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QModelIndex proxyIndex = selectedList[i];
                    QModelIndex sourceIndex = proxyModel->mapToSource(proxyIndex);
                    model->setData(sourceIndex, name_list[i / 4], Qt::EditRole);
                }
                model->setReadOnly(true);
            }
        }
    }
    else if (know_what == delete_action)
    {
        if (treeView->selectionModel())
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QList<QUrl> urls;
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    QString this_file_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                    urls.append(QUrl::fromLocalFile(this_file_path));
                }
                for (const QUrl& url : urls)
                {
                    QString srcPath = url.toLocalFile();
                    if (srcPath.isEmpty())
                    {
                        continue;
                    }
                    QFile::moveToTrash(srcPath);
                }
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
                    m_process_str += File_Control::FilenameForBash(this_file_path);
                }
            }
            else
            {
                m_process_str += " ";
                m_process_str += File_Control::FilenameForBash(this_file_path);
            }
            process.setArguments(QStringList() << "-c" << m_process_str);
            process.setStandardOutputFile("/dev/null");
            process.setStandardErrorFile("/dev/null");
            process.startDetached();
        }
    }
    else if (know_what == single_press_mode_action)
    {
        single_press_mode_action->setIconVisibleInMenu(!single_press_mode_action->isIconVisibleInMenu());
    }
    else if (know_what == set_dir_path)
    {
        QString filename = QFileDialog::getExistingDirectory(nullptr, "获取文件夹", root_path);
        My_X11_Libs::X11_Raise();
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
    else
    {
        basic_action_func(know_what);
    }
}
void File_Tree::dropEvent(QDropEvent *event)
{
    proposed_action_index = QModelIndex();
    QPoint pos = event->pos() - treeView->pos() - carrier_widget->pos() - this->get_self()->pos() - QPoint(0, 25);
    if (*m_allow_drop && event->mimeData()->hasUrls())
    {
        if (pos.y() <= 0)
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
        else
        {
            QFileInfo to_file_info(root_path);
            QModelIndex proxyIndex = treeView->indexAt(pos);
            if (proxyIndex.isValid())
            {
                QFileInfo file_info(model->filePath(proxyModel->mapToSource(proxyIndex)));
                if (file_info.isDir())
                {
                    to_file_info = file_info;
                }
                else
                {
                    to_file_info = QFileInfo(file_info.dir().path());
                }
            }
            QString targetDir = to_file_info.filePath();
            for (const QUrl& url : event->mimeData()->urls())
            {
                QString srcPath = url.toLocalFile();
                if (srcPath.isEmpty())
                {
                    continue;
                }
                QFileInfo srcInfo(srcPath);
                QString destPath = targetDir + QDir::separator() + srcInfo.fileName();
                if (srcPath == destPath)
                {
                    continue;
                }
                int copy_file_asking = -1;
                File_Control::Copy_File(srcPath, destPath, event->source() == this->treeView, &copy_file_asking);
                QModelIndex idx = proxyModel->mapFromSource(model->index(destPath));
                if (idx.isValid())
                {
                    model->data(idx, Qt::DisplayRole);
                    treeView->update(idx);
                }
            }
        }
    }
}
void File_Tree::dragMoveEvent(QDragMoveEvent *event)
{
    proposed_action_index = QModelIndex();
    if (*m_allow_drop && event->mimeData()->hasUrls())
    {
        QPoint pos = event->pos() - treeView->pos() - carrier_widget->pos() - this->get_self()->pos() - QPoint(0, 25);
        proposed_action_index = treeView->indexAt(pos);
        event->accept();
    }
}
void File_Tree::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->source() == this->treeView)
    {
        event->setDropAction(Qt::MoveAction);
    }
    else
    {
        event->setDropAction(Qt::CopyAction);
    }
    proposed_action_index = QModelIndex();
    if (*m_allow_drop && event->mimeData()->hasUrls())
    {
        QPoint pos = event->pos() - treeView->pos() - carrier_widget->pos() - this->get_self()->pos() - QPoint(0, 25);
        proposed_action_index = treeView->indexAt(pos);
        event->accept();
    }
}
void File_Tree::dragLeaveEvent(QDragLeaveEvent *event)
{
    proposed_action_index = QModelIndex();
    (void) event;
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
    settings->setValue("show_hidden_action", show_hidden_action->isIconVisibleInMenu());
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
    show_hidden_action->setIconVisibleInMenu(settings->value("show_hidden_action", false).toBool());
    proxyModel->setShowHidden(show_hidden_action->isIconVisibleInMenu());
    radius = settings->value("treeview_radius", 10).toInt();
    set_tree_view_style();
}
QIcon My_Icon_Provider::icon(QFileIconProvider::IconType type) const
{
    return QFileIconProvider::icon(type);
}
QSize My_Icon_Provider::get_Image_Size(QString path) const
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
            QIcon icon = QIcon::fromTheme(info.filePath());
            if (!icon.isNull() && My_Icon_Provider::get_Image_Size(info.filePath()) != QSize(0, 0))
            {
                return icon;//应使用filePath而不是filename;
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
    setDragDropMode(QAbstractItemView::NoDragDrop);
    setMouseTracking(true);
    setTabletTracking(true);
}
void My_Tree_View::dropEvent(QDropEvent *event)
{
    QWidget::dropEvent(event);
}
void My_Tree_View::dragMoveEvent(QDragMoveEvent *event)
{
    QWidget::dragMoveEvent(event);
}
void My_Tree_View::dragEnterEvent(QDragEnterEvent *event)
{
    QWidget::dragEnterEvent(event);
}
void My_Tree_View::dragLeaveEvent(QDragLeaveEvent *event)
{
    QWidget::dragLeaveEvent(event);
}
void My_Tree_View::mousePressEvent(QMouseEvent *event)
{
    if (!indexAt(event->pos()).isValid() && !(QApplication::keyboardModifiers() & Qt::ControlModifier || QApplication::keyboardModifiers() & Qt::ShiftModifier))
    {
        this->selectionModel()->clear();
    }
    QTreeView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        if (indexAt(event->pos()) != indexAt(event->pos() + QPoint(0, 3)) || indexAt(event->pos()) != indexAt(event->pos() - QPoint(0, 3))|| selectionModel()->selectedIndexes().isEmpty() || (QApplication::keyboardModifiers() & Qt::ControlModifier || QApplication::keyboardModifiers() & Qt::ShiftModifier))
        {
            setup_rubber = true;
            origin_pos = event->pos();
            m_rubberBand->setGeometry(QRect(origin_pos + QPoint(0, 25), QSize()));
            m_rubberBand->show();
        }
        else
        {
            setup_rubber = false;
            m_rubberBand->hide();
        }
    }
}
void My_Tree_View::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton && setup_rubber)
    {
        setup_rubber = false;
        m_rubberBand->hide();
    }
}
void My_Tree_View::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && F_model && proxyModel && selectionModel())
    {
        if (setup_rubber && m_rubberBand->isVisible())
        {
            QRect rect = QRect(origin_pos + QPoint(0, 25), event->pos() + QPoint(0, 25)).normalized();
            m_rubberBand->setGeometry(rect);
            QItemSelection selection;
            QModelIndex topLeft = indexAt(rect.topLeft());
            QModelIndex bottomRight = indexAt(rect.bottomRight());
            if (topLeft.isValid() && bottomRight.isValid())
            {
                selection.select(topLeft, bottomRight);
            }
            QItemSelectionModel::SelectionFlags flags = QItemSelectionModel::ClearAndSelect;
            if (QApplication::keyboardModifiers() & Qt::ControlModifier)
            {
                flags = QItemSelectionModel::Select;
            }
            else if (QApplication::keyboardModifiers() & Qt::ShiftModifier)
            {
                flags = QItemSelectionModel::SelectCurrent;
            }
            else
            {
                flags = QItemSelectionModel::ClearAndSelect;
            }
            this->selectionModel()->select(selection, flags);
            QTreeView::mouseMoveEvent(event);
        }
        else
        {
            QModelIndexList selectedList = selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QList<QUrl> urls;
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    urls.append(QUrl::fromLocalFile(F_model->filePath(proxyModel->mapToSource(selectedList[i]))));
                }
                QMimeData *mimeData = new QMimeData;
                mimeData->setUrls(urls);
                QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->setPixmap(F_model->fileIcon(proxyModel->mapToSource(selectedList[0])).pixmap(50, 50));
                drag->setHotSpot(QPoint(25,25));
                drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
            }
        }
    }
    QWidget::mouseMoveEvent(event);//不喜欢QTreeView自实现的移动
}
void My_Tree_View::enterEvent(QEvent *event)
{
    My_Tree_View::catch_ptr = this;
    QWidget::enterEvent(event);
}
void My_Tree_View::wheelEvent(QWheelEvent *event)
{
    if (QGuiApplication::queryKeyboardModifiers() & Qt::ControlModifier)
    {
        if (event->angleDelta().y() != 0)
        {
            int angle = event->angleDelta().ry();
            if (angle < 0)
            {
                if (this->indentation() > 4 && this->font().pointSize() > 4)
                {
                    this->setIndentation(this->indentation() - 4);
                    this->setIconSize(this->iconSize() - QSize(4, 4));
                    QFont new_font = this->font();
                    new_font.setPointSize(new_font.pointSize() - 4);
                    this->setFont(new_font);
                }
            }
            else
            {
                this->setIndentation(this->indentation() + 4);
                this->setIconSize(this->iconSize() + QSize(4, 4));
                QFont new_font = this->font();
                new_font.setPointSize(new_font.pointSize() + 4);
                this->setFont(new_font);
            }
        }
    }
    QTreeView::wheelEvent(event);
}
My_TreeView_Delegate::My_TreeView_Delegate(QObject *parent, QColor *m_hover_color, QColor *m_select_color, int *m_radius, QModelIndex *m_proposed_action_index)
    :QStyledItemDelegate(parent)
    ,hover_color(m_hover_color)
    ,select_color(m_select_color)
    ,radius(m_radius)
    ,proposed_action_index(m_proposed_action_index)
{}
void My_TreeView_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    if (opt.state & QStyle::State_MouseOver || opt.state & QStyle::State_Selected || (proposed_action_index && proposed_action_index->isValid()))
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);
        int index_id = index.column();
        if (opt.state & QStyle::State_MouseOver || (proposed_action_index->row() == index.row() && proposed_action_index->parent() == index.parent()))
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
My_ProxyModel::My_ProxyModel(QObject *parent, My_Tree_View *m_root)
    :QSortFilterProxyModel(parent)
    ,root(m_root)
{}
void My_ProxyModel::setSearchPattern(const QString &pattern)
{
    m_pattern = pattern;
    invalidateFilter();
}
void My_ProxyModel::setShowHidden(bool show)
{
    if (m_showHidden != show)
    {
        m_showHidden = show;
        invalidateFilter();
    }
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
        if (fileInfo.isHidden() && !m_showHidden && !root->isExpanded(sourceIndex))
        {
            return false;
        }
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
