#include "table_tree.h"
#include "core/tools/file_control.h"
#include "core/tools/trans_sender.h"
My_Table_View * My_Table_View::catch_ptr;
void File_Table::set_icon(QString checked_icon_path)
{
    single_press_mode_action->setIcon(QIcon(checked_icon_path));
    show_hidden_action->setIcon(QIcon(checked_icon_path));
    m_dialog->set_icon(checked_icon_path);
    preview_file_widget->set_icon(checked_icon_path);
    sort_type_name->setIcon(QIcon(checked_icon_path));
    sort_type_size->setIcon(QIcon(checked_icon_path));
    sort_type_date->setIcon(QIcon(checked_icon_path));
    sort_type_name_Z->setIcon(QIcon(checked_icon_path));
    sort_type_size_Z->setIcon(QIcon(checked_icon_path));
    sort_type_date_Z->setIcon(QIcon(checked_icon_path));
    set_show_status_bar->setIcon(QIcon(checked_icon_path));
    preview_file_action->setIcon(QIcon(checked_icon_path));
    Basic_Widget::set_icon(checked_icon_path);
}
File_Table::File_Table(QWidget *parent)
    :Basic_Widget(parent)
{
    preview_file_widget = new Preview_File_Widget(parent, preview_file_action);
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
    preview_file_action->setIcon(QIcon(":/base/this.svg"));
    preview_file_action->setIconVisibleInMenu(false);
    control_menu->addAction(preview_file_action);
    control_menu->addSeparator();
    control_menu->addAction(select_all_action);
    control_menu->addAction(clean_selection_action);
    control_menu->addAction(refresh_action);
    menu->addMenu(control_menu);
    single_press_mode_action->setIcon(QIcon(":/base/this.svg"));
    single_press_mode_action->setIconVisibleInMenu(false);
    tree_setting->addAction(single_press_mode_action);
    tree_setting->addAction(set_dir_path);
    set_show_status_bar->setIcon(QIcon(":/base/this.svg"));
    set_show_status_bar->setIconVisibleInMenu(false);
    set_style_menu->addAction(set_show_status_bar);
    set_style_menu->addAction(set_show_status_bar_text_color);
    set_style_menu->addSeparator();
    sort_type_name->setIcon(QIcon(":/base/this.svg"));
    sort_type_size->setIcon(QIcon(":/base/this.svg"));
    sort_type_date->setIcon(QIcon(":/base/this.svg"));
    sort_type_name->setIconVisibleInMenu(true);
    sort_type_size->setIconVisibleInMenu(false);
    sort_type_date->setIconVisibleInMenu(false);
    sort_type_menu->addAction(sort_type_name);
    sort_type_menu->addAction(sort_type_size);
    sort_type_menu->addAction(sort_type_date);
    sort_type_name_Z->setIcon(QIcon(":/base/this.svg"));
    sort_type_size_Z->setIcon(QIcon(":/base/this.svg"));
    sort_type_date_Z->setIcon(QIcon(":/base/this.svg"));
    sort_type_name_Z->setIconVisibleInMenu(false);
    sort_type_size_Z->setIconVisibleInMenu(false);
    sort_type_date_Z->setIconVisibleInMenu(false);
    sort_type_menu->addAction(sort_type_name_Z);
    sort_type_menu->addAction(sort_type_size_Z);
    sort_type_menu->addAction(sort_type_date_Z);
    set_style_menu->addMenu(sort_type_menu);
    set_style_menu->addSeparator();
    set_style_menu->addAction(set_icon_size_action);
    set_style_menu->addAction(set_font_action);
    set_style_menu->addAction(set_select_radius);
    set_style_menu->addAction(set_hover_color);
    set_style_menu->addAction(set_select_color);
    tree_setting->addMenu(set_style_menu);
    menu->addMenu(tree_setting);
    menu->addSeparator();
    menu->addAction(cut_action);
    menu->addAction(copy_action);
    menu->addAction(paste_action);
    menu->addAction(rename_action);
    menu->addAction(delete_action);
    menu->addSeparator();
    menu->addAction(show_info);
    menu->addSeparator();
    basic_context(menu);

    treeView->m_statusBar->setFixedHeight(24 * 2);
    treeView->m_statusBar->hide();
    treeView->updateStatusBar_style();
    treeView->m_statusBar->setSizeGripEnabled(false);
    treeView->statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    treeView->m_statusBar->addPermanentWidget(treeView->statusLabel, 1);
    treeView->updateStatusBar();

    //shortcut
    shortcut_copy_action->setShortcut(QKeySequence::Copy);
    shortcut_copy_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_copy_action, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Table_View::catch_ptr)
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QList<QUrl> urls;
                QByteArray gnomeData;
                gnomeData.append("copy\n");
                for (int i = 0; i < selectedList.count(); i += 1)
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
        if (treeView->selectionModel() && treeView == My_Table_View::catch_ptr)
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
                for (int i = 0; i < selectedList.count(); i += 1)
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
        if (treeView->selectionModel() && treeView == My_Table_View::catch_ptr)
        {
            Pressed(true);
        }
    });
    shortcut_cut_action->setShortcut(QKeySequence::Cut);
    shortcut_cut_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_cut_action, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Table_View::catch_ptr)
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QList<QUrl> urls;
                QByteArray gnomeData;
                gnomeData.append("cut\n");
                for (int i = 0; i < selectedList.count(); i += 1)
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
        if (treeView->selectionModel() && treeView == My_Table_View::catch_ptr)
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
                for (int i = 0; i < selectedList.count(); i += 1)
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
        if (treeView->selectionModel() && treeView == My_Table_View::catch_ptr)
        {
            search_edit->setFocus();
        }
    });
    shortcut_delete_action->setShortcut(Qt::Key_Delete);
    shortcut_delete_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_delete_action, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Table_View::catch_ptr)
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QList<QUrl> urls;
                for (int i = 0; i < selectedList.count(); i += 1)
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
        if (treeView->selectionModel() && treeView == My_Table_View::catch_ptr)
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QList<QUrl> urls;
                for (int i = 0; i < selectedList.count(); i += 1)
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
        if (treeView->selectionModel() && treeView == My_Table_View::catch_ptr)
        {
            show_hidden_action->setIconVisibleInMenu(!show_hidden_action->isIconVisibleInMenu());
            proxyModel->setShowHidden(show_hidden_action->isIconVisibleInMenu());
        }
    });
    shortcut_rename_action->setShortcut(Qt::Key_F2);
    shortcut_rename_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_rename_action, &QAction::triggered, this, [=]
    {
        if (treeView->selectionModel() && treeView == My_Table_View::catch_ptr)
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QString name_list_str = "";
                for (int i = 0; i < selectedList.count(); i += 1)
                {
                    if (!name_list_str.isEmpty())
                    {
                        name_list_str += "\n";
                    }
                    name_list_str += model->fileName(proxyModel->mapToSource(selectedList[i]));
                }
                if (m_dialog->Setup(tr("重命名"), tr("获取新名称:(请勿添加\\n)"), name_list_str) != QDialog::Accepted)
                {
                    return;
                }
                QStringList name_list = m_dialog->getLines();
                if (name_list.count() != selectedList.count())
                {
                    return;
                }
                model->setReadOnly(false);
                for (int i = 0; i < selectedList.count(); i += 1)
                {
                    QModelIndex proxyIndex = selectedList[i];
                    proxyModel->Rename_File(proxyIndex, name_list[i]);
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
    deeply_search_button->resize(70, 40);
    deeply_search_button->setStyleSheet("QPushButton{border-radius:10px 10px;background:rgba(0,0,0,25);color:rgb(0,0,0)}"
                                        "QPushButton:hover{border-radius:10px 10px;background:rgba(0,0,0,50)}"
                                        "QPushButton:pressed{border-radius:10px 10px;background:rgba(0,0,0,25)}");
    flat_search_button->resize(70, 40);
    flat_search_button->setStyleSheet("QPushButton{border-radius:10px 10px;background:rgba(0,0,0,25);color:rgb(0,0,0)}"
                                      "QPushButton:hover{border-radius:10px 10px;background:rgba(0,0,0,50)}"
                                      "QPushButton:pressed{border-radius:10px 10px;background:rgba(0,0,0,25)}");
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
    search_edit->setPlaceholderText(tr("搜索"));
    search_img_action->setIcon(QIcon(":/base/search.svg"));
    search_edit->addAction(search_img_action, QLineEdit::LeadingPosition);
    search_del_action->setIcon(QIcon(":/base/del.svg"));
    model->setFilter(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
    model->setRootPath(QDir::rootPath());
    proxyModel->setSourceModel(model);
    proxyModel->setShowHidden(false);
    treeView->setModel(proxyModel);
    treeView->setRootIndex(proxyModel->mapFromSource(model->index(QDir::rootPath())));
    carrier_widget->setMouseTracking(true);
    search_edit->setMouseTracking(true);
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
            proxyModel->setSearchPattern("", 0);
            if (treeView->model() != proxyModel)
            {
                treeView->setModel(proxyModel);
                treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
                setupSelectionConnections();
            }
            search_edit->removeAction(search_del_action);
        }
        else
        {
            search_edit->addAction(search_del_action,QLineEdit::TrailingPosition);
            proxyModel->setSearchPattern(search_edit->text());
        }
    });
    connect(deeply_search_button, &QPushButton::pressed, this, [=]
    {
        if (treeView->selectionModel())
        {
            treeView->selectionModel()->clear();
        }
        if (search_edit->text().isEmpty())
        {
            return;
        }
        proxyModel->setSearchPattern(search_edit->text(), true);
    });
    connect(flat_search_button, &QPushButton::pressed, this, [=]
    {
        if (search_edit->text().isEmpty())
        {
            return;
        }
        proxyModel->setSearchPattern(search_edit->text(), 2);
        FlatFileListModel *flat = proxyModel->flatModel();
        if (flat)
        {
            treeView->setModel(flat);
            setupSelectionConnections();
        }
    });
    connect(this, &Basic_Widget::size_changed, this, [=](QSize size)
    {
        carrier_widget->resize(size - QSize(20, 20));
        search_edit->resize(size.width() - 20 - 160, 40);
        deeply_search_button->move(size.width() - 95, 5);
        flat_search_button->move(size.width() - 170, 5);
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
            Pressed(true);
        }
    });
    setupSelectionConnections();
    connect(Trans_Sender::instance(), &Trans_Sender::Trans_sig, this, [=]
    {
        search_edit->setPlaceholderText(tr("搜索"));
    });
    resize(600, 300);
    show();
}
void File_Table::setupSelectionConnections()
{
    if (m_selectionChangedConn)
    {
        disconnect(m_selectionChangedConn);
        m_selectionChangedConn = QMetaObject::Connection();
    }
    if (m_currentChangedConn)
    {
        disconnect(m_currentChangedConn);
        m_currentChangedConn = QMetaObject::Connection();
    }

    if (!treeView->selectionModel())
    {
        return;
    }

    this->treeView->updateStatusBar();
    preview_file_widget->updatePreview({}, root_path);

    auto updateFunc = [this]()
    {
        this->treeView->updateStatusBar();
        if (treeView->selectionModel())
        {
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            QStringList filelist = {};
            for (int i = 0; i < selectedList.count(); i += 1)
            {
                filelist << model->filePath(proxyModel->mapToSource(selectedList[i]));
            }
            preview_file_widget->updatePreview(filelist, root_path);
        }
    };

    m_selectionChangedConn = connect(treeView->selectionModel(), &QItemSelectionModel::selectionChanged, this, updateFunc);
    m_currentChangedConn = connect(treeView->selectionModel(), &QItemSelectionModel::currentChanged, this, updateFunc);
}
void File_Table::Pressed(bool from_key)
{
    if (treeView->selectionModel() && !(QApplication::keyboardModifiers() & Qt::ControlModifier || QApplication::keyboardModifiers() & Qt::ShiftModifier))
    {
        QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
        if (!selectedList.isEmpty())
        {
            for (int i = 0; i < selectedList.count(); i += 1)
            {
                if (model->hasChildren(proxyModel->mapToSource(selectedList[i])))
                {
                    if (single_press_mode_action->isIconVisibleInMenu() || from_key)
                    {
                        root_path = model->filePath(proxyModel->mapToSource(selectedList[i]));
                        proxyModel->setSearchPattern(search_edit->text());
                        if (treeView->model() != proxyModel)
                        {
                            treeView->setModel(proxyModel);
                        }
                        treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
                        setupSelectionConnections();
                        if (treeView->selectionModel())
                        {
                            treeView->selectionModel()->clear();
                        }
                        treeView->updateStatusBar();
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
void File_Table::first_set_preview_pos()
{
    preview_file_widget->move(this->pos());
    preview_file_widget->setParent(this->parentWidget());
    preview_file_widget->set_now_page(now_page);
    preview_file_widget->set_desktop_number(desktop_number);
    preview_file_widget->set_basic_list(basic_list);
}
void File_Table::set_tree_view_style()
{
    treeView->setStyleSheet(QString("QListView{background:rgba(255,255,255,0);color:rgb(60,60,60);selection-background-color:rgba(0,0,0,0);}"
                                    "QListView::item:selected{color:rgb(255,255,255);border: 0px solid rgba(255,255,255,0)}"
                                    "QToolTip { background: rgba(%1,%2,%3,175); }")
                            .arg(hover_color.red()).arg(hover_color.green()).arg(hover_color.blue()));
}
File_Table::~File_Table()
{
    if (m_dialog)
    {
        m_dialog->deleteLater();
        m_dialog = nullptr;
    }
    delete icon_provider;
    if (preview_file_widget)
    {
        preview_file_widget->deleteLater();
        preview_file_widget = nullptr;
    }
    if (file_table_list)
    {
        file_table_list->removeOne(this);
    }
}
void File_Table::contextMenuEvent(QContextMenuEvent *event)
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
                for (int i = 0; i < selectedList.count(); i += 1)
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
                for (int i = 0; i < selectedList.count(); i += 1)
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
                for (int i = 0; i < selectedList.count(); i += 1)
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
            if (m_dialog->Setup(tr("新建文件"), tr("获取文件名:(一行一文件)"), "new_file") != QDialog::Accepted)
            {
                return;
            }
            QStringList name_list = m_dialog->getLines();
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            QString tmp_file_top_path = "";
            if (!selectedList.isEmpty())
            {
                for (int i = 0; i < selectedList.count(); i += 1)
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
            if (m_dialog->Setup(tr("新建文件夹"), tr("获取文件夹名:(一行一文件夹)"), "new_folder") != QDialog::Accepted)
            {
                return;
            }
            QStringList name_list = m_dialog->getLines();
            QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
            QString tmp_file_top_path = "";
            if (!selectedList.isEmpty())
            {
                for (int i = 0; i < selectedList.count(); i += 1)
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
                proxyModel->setSearchPattern(search_edit->text());
                if (treeView->model() != proxyModel)
                {
                    treeView->setModel(proxyModel);
                }
                treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
                setupSelectionConnections();
                if (treeView->selectionModel())
                {
                    treeView->selectionModel()->clear();
                }
                treeView->updateStatusBar();
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
                proxyModel->setSearchPattern(search_edit->text());
                if (treeView->model() != proxyModel)
                {
                    treeView->setModel(proxyModel);
                }
                treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
                setupSelectionConnections();
                if (treeView->selectionModel())
                {
                    treeView->selectionModel()->clear();
                }
                treeView->updateStatusBar();
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
                for (int i = 0; i < selectedList.count(); i += 1)
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
                for (int i = 0; i < selectedList.count(); i += 1)
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
                for (int i = 0; i < selectedList.count(); i += 1)
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
    else if (know_what == preview_file_action)
    {
        if (preview_file_widget && !preview_file_action->isIconVisibleInMenu())
        {
            preview_file_widget->show();
            preview_file_widget->raise();
            if (treeView->selectionModel())
            {
                QModelIndexList selectedList = treeView->selectionModel()->selectedIndexes();
                if (!selectedList.isEmpty())
                {
                    QStringList filelist = {};
                    for (int i = 0; i < selectedList.count(); i += 1)
                    {
                        filelist << model->filePath(proxyModel->mapToSource(selectedList[i]));
                    }
                    preview_file_widget->updatePreview(filelist, root_path);
                }
                else
                {
                    preview_file_widget->updatePreview({}, root_path);
                }
            }
            preview_file_action->setIconVisibleInMenu(true);
        }
        else
        {
            preview_file_widget->hide();
            preview_file_action->setIconVisibleInMenu(false);
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
        proxyModel->setSearchPattern(search_edit->text());
        if (treeView->model() != proxyModel)
        {
            treeView->setModel(proxyModel);
        }
        treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
        setupSelectionConnections();
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
                for (int i = 0; i < selectedList.count(); i += 1)
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
                for (int i = 0; i < selectedList.count(); i += 1)
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
            for (int i = 0; i < selectedList.count(); i += 1)
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
                for (int i = 0; i < selectedList.count(); i += 1)
                {
                    if (!name_list_str.isEmpty())
                    {
                        name_list_str += "\n";
                    }
                    name_list_str += model->fileName(proxyModel->mapToSource(selectedList[i]));
                }
                if (m_dialog->Setup(tr("重命名"), tr("获取新名称:(请勿添加\\n)"), name_list_str) != QDialog::Accepted)
                {
                    return;
                }
                QStringList name_list = m_dialog->getLines();
                if (name_list.count() != selectedList.count())
                {
                    return;
                }
                model->setReadOnly(false);
                for (int i = 0; i < selectedList.count(); i += 1)
                {
                    QModelIndex proxyIndex = selectedList[i];
                    proxyModel->Rename_File(proxyIndex, name_list[i]);
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
                for (int i = 0; i < selectedList.count(); i += 1)
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
                for (int i = 0; i < selectedList.count(); i += 1)
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
        QString filename = QFileDialog::getExistingDirectory(nullptr, tr("获取文件夹"), root_path);
        My_X11_Libs::X11_Raise();
        if (filename.isEmpty() || filename.isNull())
        {
            return;
        }
        root_path = filename;
        proxyModel->setSearchPattern(search_edit->text());
        if (treeView->model() != proxyModel)
        {
            treeView->setModel(proxyModel);
        }
        treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
        setupSelectionConnections();
        if (treeView->selectionModel())
        {
            treeView->selectionModel()->clear();
        }
        treeView->updateStatusBar();
    }
    else if (know_what == set_icon_size_action)
    {
        bool ok = false;
        int num = QInputDialog::getInt(nullptr, tr("获取数值"), tr("大小:"), treeView->iconSize().width(), 4, 2147483647, 1, &ok);
        if (ok)
        {
            treeView->setIconSize(QSize(num, num));
            treeView->setGridSize(QSize(num + 32, num + 32));
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
        colorDialog.setWindowTitle(tr("获取颜色"));
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
        colorDialog.setWindowTitle(tr("获取颜色"));
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
        int num = QInputDialog::getInt(nullptr, tr("获取数值"), tr("大小:"), radius, 0, 2147483647, 1, &ok);
        if (ok)
        {
            radius = num;
            set_tree_view_style();
        }
    }
    else if (know_what == set_show_status_bar)
    {
        set_show_status_bar->setIconVisibleInMenu(!set_show_status_bar->isIconVisibleInMenu());
        treeView->m_statusBar->setVisible(set_show_status_bar->isIconVisibleInMenu());
        treeView->updateStatusBar();
    }
    else if (know_what == set_show_status_bar_text_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(treeView->statusBar_text_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        treeView->statusBar_text_color = colorDialog.currentColor();
        treeView->updateStatusBar_style();
    }
    else if (know_what == sort_type_name)
    {
        sort_type_name->setIconVisibleInMenu(true);
        sort_type_size->setIconVisibleInMenu(false);
        sort_type_date->setIconVisibleInMenu(false);
        sort_type_name_Z->setIconVisibleInMenu(false);
        sort_type_size_Z->setIconVisibleInMenu(false);
        sort_type_date_Z->setIconVisibleInMenu(false);
        sort_type = 0;
        proxyModel->invalidate();
        proxyModel->sort(0);
    }
    else if (know_what == sort_type_size)
    {
        sort_type_name->setIconVisibleInMenu(false);
        sort_type_size->setIconVisibleInMenu(true);
        sort_type_date->setIconVisibleInMenu(false);
        sort_type_name_Z->setIconVisibleInMenu(false);
        sort_type_size_Z->setIconVisibleInMenu(false);
        sort_type_date_Z->setIconVisibleInMenu(false);
        sort_type = 1;
        proxyModel->invalidate();
        proxyModel->sort(0);
    }
    else if (know_what == sort_type_date)
    {
        sort_type_name->setIconVisibleInMenu(false);
        sort_type_size->setIconVisibleInMenu(false);
        sort_type_date->setIconVisibleInMenu(true);
        sort_type_name_Z->setIconVisibleInMenu(false);
        sort_type_size_Z->setIconVisibleInMenu(false);
        sort_type_date_Z->setIconVisibleInMenu(false);
        sort_type = 2;
        proxyModel->invalidate();
    }
    else if (know_what == sort_type_name_Z)
    {
        sort_type_name->setIconVisibleInMenu(false);
        sort_type_size->setIconVisibleInMenu(false);
        sort_type_date->setIconVisibleInMenu(false);
        sort_type_name_Z->setIconVisibleInMenu(true);
        sort_type_size_Z->setIconVisibleInMenu(false);
        sort_type_date_Z->setIconVisibleInMenu(false);
        sort_type = 3;
        proxyModel->invalidate();
        proxyModel->sort(0);
    }
    else if (know_what == sort_type_size_Z)
    {
        sort_type_name->setIconVisibleInMenu(false);
        sort_type_size->setIconVisibleInMenu(false);
        sort_type_date->setIconVisibleInMenu(false);
        sort_type_name_Z->setIconVisibleInMenu(false);
        sort_type_size_Z->setIconVisibleInMenu(true);
        sort_type_date_Z->setIconVisibleInMenu(false);
        sort_type = 4;
        proxyModel->invalidate();
        proxyModel->sort(0);
    }
    else if (know_what == sort_type_date_Z)
    {
        sort_type_name->setIconVisibleInMenu(false);
        sort_type_size->setIconVisibleInMenu(false);
        sort_type_date->setIconVisibleInMenu(false);
        sort_type_name_Z->setIconVisibleInMenu(false);
        sort_type_size_Z->setIconVisibleInMenu(false);
        sort_type_date_Z->setIconVisibleInMenu(true);
        sort_type = 5;
        proxyModel->invalidate();
        proxyModel->sort(0);
    }
    else
    {
        basic_action_func(know_what);
    }
}
void File_Table::dropEvent(QDropEvent *event)
{
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
            proxyModel->setSearchPattern(search_edit->text());
            if (treeView->model() != proxyModel)
            {
                treeView->setModel(proxyModel);
            }
            treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
            setupSelectionConnections();
            if (treeView->selectionModel())
            {
                treeView->selectionModel()->clear();
            }
            treeView->updateStatusBar();
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
void File_Table::dragMoveEvent(QDragMoveEvent *event)
{
    if (*m_allow_drop && event->mimeData()->hasUrls())
    {
        event->accept();
    }
}
void File_Table::dragEnterEvent(QDragEnterEvent *event)
{
    if (event->source() == this->treeView)
    {
        event->setDropAction(Qt::MoveAction);
    }
    else
    {
        event->setDropAction(Qt::CopyAction);
    }
    if (*m_allow_drop && event->mimeData()->hasUrls())
    {
        event->accept();
    }
}
void File_Table::dragLeaveEvent(QDragLeaveEvent *event)
{
    (void) event;
}
void File_Table::wheelEvent(QWheelEvent *event)
{
    if (treeView == My_Table_View::catch_ptr)
    {
        event->accept();
    }
}
void File_Table::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    settings->setValue("root_path", root_path);
    settings->setValue("icon_size", treeView->iconSize().width());
    settings->setValue("grid_width", treeView->gridSize().width());
    settings->setValue("grid_height", treeView->gridSize().height());
    settings->setValue("text_font", treeView->font());
    settings->setValue("hover_color", hover_color.rgba());
    settings->setValue("select_color", select_color.rgba());
    settings->setValue("single_press_mode", single_press_mode_action->isIconVisibleInMenu());
    settings->setValue("show_hidden_action", show_hidden_action->isIconVisibleInMenu());
    settings->setValue("treeview_radius", radius);
    settings->setValue("set_show_status_bar", set_show_status_bar->isIconVisibleInMenu());
    settings->setValue("preview_file_action", preview_file_action->isIconVisibleInMenu());
    settings->setValue("sort_type", sort_type);
    treeView->p_save(settings);
    m_dialog->p_save(settings, "file_table_dialog_");
    preview_file_widget->save(settings, "file_table_preview_");
}
void File_Table::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    root_path = settings->value("root_path", QDir::rootPath()).toString();
    treeView->setRootIndex(proxyModel->mapFromSource(model->index(root_path)));
    int iconSize = settings->value("icon_size", 64).toInt();
    int gw = settings->value("grid_width", iconSize + 32).toInt();
    int gh = settings->value("grid_height", iconSize + 32).toInt();
    treeView->setIconSize(QSize(iconSize, iconSize));
    treeView->setGridSize(QSize(gw, gh));
    treeView->setFont(settings->value("text_font", QFontDatabase::systemFont(QFontDatabase::FixedFont)).value<QFont>());
    hover_color = QColor::fromRgba(settings->value("hover_color", QColor(227, 242, 253, 255).rgba()).toUInt());
    select_color = QColor::fromRgba(settings->value("select_color", QColor(0, 170, 255, 255).rgba()).toUInt());
    single_press_mode_action->setIconVisibleInMenu(settings->value("single_press_mode", false).toBool());
    show_hidden_action->setIconVisibleInMenu(settings->value("show_hidden_action", false).toBool());
    proxyModel->setShowHidden(show_hidden_action->isIconVisibleInMenu());
    radius = settings->value("treeview_radius", 10).toInt();
    set_tree_view_style();
    sort_type = settings->value("sort_type", 0).toInt();
    if (0 == sort_type)
    {
        sort_type_name->setIconVisibleInMenu(true);
        sort_type_size->setIconVisibleInMenu(false);
        sort_type_date->setIconVisibleInMenu(false);
        sort_type_name_Z->setIconVisibleInMenu(false);
        sort_type_size_Z->setIconVisibleInMenu(false);
        sort_type_date_Z->setIconVisibleInMenu(false);
    }
    else if (1 == sort_type)
    {
        sort_type_name->setIconVisibleInMenu(false);
        sort_type_size->setIconVisibleInMenu(true);
        sort_type_date->setIconVisibleInMenu(false);
        sort_type_name_Z->setIconVisibleInMenu(false);
        sort_type_size_Z->setIconVisibleInMenu(false);
        sort_type_date_Z->setIconVisibleInMenu(false);
    }
    else if (2 == sort_type)
    {
        sort_type_name->setIconVisibleInMenu(false);
        sort_type_size->setIconVisibleInMenu(false);
        sort_type_date->setIconVisibleInMenu(true);
        sort_type_name_Z->setIconVisibleInMenu(false);
        sort_type_size_Z->setIconVisibleInMenu(false);
        sort_type_date_Z->setIconVisibleInMenu(false);
    }
    else if (3 == sort_type)
    {
        sort_type_name->setIconVisibleInMenu(false);
        sort_type_size->setIconVisibleInMenu(false);
        sort_type_date->setIconVisibleInMenu(false);
        sort_type_name_Z->setIconVisibleInMenu(true);
        sort_type_size_Z->setIconVisibleInMenu(false);
        sort_type_date_Z->setIconVisibleInMenu(false);
    }
    else if (4 == sort_type)
    {
        sort_type_name->setIconVisibleInMenu(false);
        sort_type_size->setIconVisibleInMenu(false);
        sort_type_date->setIconVisibleInMenu(false);
        sort_type_name_Z->setIconVisibleInMenu(false);
        sort_type_size_Z->setIconVisibleInMenu(true);
        sort_type_date_Z->setIconVisibleInMenu(false);
    }
    else if (5 == sort_type)
    {
        sort_type_name->setIconVisibleInMenu(false);
        sort_type_size->setIconVisibleInMenu(false);
        sort_type_date->setIconVisibleInMenu(false);
        sort_type_name_Z->setIconVisibleInMenu(false);
        sort_type_size_Z->setIconVisibleInMenu(false);
        sort_type_date_Z->setIconVisibleInMenu(true);
    }
    set_show_status_bar->setIconVisibleInMenu(settings->value("set_show_status_bar", false).toBool());
    treeView->m_statusBar->setVisible(set_show_status_bar->isIconVisibleInMenu());
    treeView->p_load(settings);
    m_dialog->p_load(settings, "file_table_dialog_");
    first_set_preview_pos();
    preview_file_action->setIconVisibleInMenu(settings->value("preview_file_action", false).toBool());
    preview_file_widget->load(settings, "file_table_preview_");
    preview_file_widget->setVisible(preview_file_action->isIconVisibleInMenu());
    preview_file_widget->updatePreview({}, root_path, true);
    treeView->updateStatusBar();
    proxyModel->invalidate();
    proxyModel->sort(0);
}
void My_Table_View::backToPath()
{
    if (this->selectionModel())
    {
        this->setRootIndex(proxyModel->mapFromSource(F_model->index(*root_path_ptr)));
        this->updateStatusBar();
    }
}
My_Table_View::My_Table_View(QWidget *parent, QString *m_root_path_ptr)
    :QListView(parent)
    ,Tree_View_Root_Interface()
{
    root_path_ptr = m_root_path_ptr;
    setViewMode(QListView::IconMode);
    setResizeMode(QListView::Adjust);
    setMovement(QListView::Snap);
    setGridSize(QSize(96, 96));//64 + 32
    setIconSize(QSize(64, 64));
    setWordWrap(true);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setDragEnabled(true);
    setAcceptDrops(true);
    setDropIndicatorShown(true);
    setDefaultDropAction(Qt::CopyAction);
    setMouseTracking(true);
    setTabletTracking(true);
    setSelectionRectVisible(false);//轮子造太多了

    m_sizeUpdateTimer->setInterval(1000);
    connect(m_sizeUpdateTimer, &QTimer::timeout, this, [=]()
    {
        My_Table_View::onSizeCalculated();
    });
    connect(m_futureWatcher, &QFutureWatcher<qint64>::finished, this, &My_Table_View::onSizeCalculated);
}
My_Table_View::~My_Table_View()
{
    m_sizeUpdateTimer->stop();
    if (m_futureWatcher->isRunning())
    {
        m_cancelCalculation = true;
        m_futureWatcher->cancel();
    }
}
void My_Table_View::p_save(QSettings *settings)
{
    settings->setValue("statusBar_text_color", statusBar_text_color.rgba());
}
void My_Table_View::p_load(QSettings *settings)
{
    statusBar_text_color = QColor::fromRgba(settings->value("statusBar_text_color", QColor(50, 50, 50, 255).rgba()).toUInt());
    updateStatusBar_style();
    updateStatusBar();
}
void My_Table_View::dropEvent(QDropEvent *event)
{
    QWidget::dropEvent(event);
}
void My_Table_View::dragMoveEvent(QDragMoveEvent *event)
{
    QWidget::dragMoveEvent(event);
}
void My_Table_View::dragEnterEvent(QDragEnterEvent *event)
{
    QWidget::dragEnterEvent(event);
}
void My_Table_View::dragLeaveEvent(QDragLeaveEvent *event)
{
    QWidget::dragLeaveEvent(event);
}
void My_Table_View::mousePressEvent(QMouseEvent *event)
{
    if (!indexAt(event->pos()).isValid() && !(QApplication::keyboardModifiers() & Qt::ControlModifier || QApplication::keyboardModifiers() & Qt::ShiftModifier))
    {
        this->selectionModel()->clear();
    }
    QListView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)//不管了,先保留吧
    {
        if (indexAt(event->pos()) != indexAt(event->pos() + QPoint(0, 3)) || indexAt(event->pos()) != indexAt(event->pos() - QPoint(0, 3))|| selectionModel()->selectedIndexes().isEmpty() || (QApplication::keyboardModifiers() & Qt::ControlModifier || QApplication::keyboardModifiers() & Qt::ShiftModifier))
        {
            setup_rubber = true;
            origin_pos = event->pos();
            m_rubberBand->setGeometry(QRect(origin_pos, QSize()));
            m_rubberBand->show();
        }
        else
        {
            setup_rubber = false;
            m_rubberBand->hide();
        }
    }
}
void My_Table_View::mouseReleaseEvent(QMouseEvent *event)
{
    QListView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton && setup_rubber)
    {
        setup_rubber = false;
        m_rubberBand->hide();
    }
}
void My_Table_View::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && F_model && proxyModel && selectionModel())
    {
        if (setup_rubber && m_rubberBand->isVisible())
        {
            QRect rect = QRect(origin_pos, event->pos()).normalized();
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
            QListView::mouseMoveEvent(event);
        }
        else
        {
            QModelIndexList selectedList = selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QList<QUrl> urls;
                for (int i = 0; i < selectedList.count(); i += 1)
                {
                    urls.append(QUrl::fromLocalFile(F_model->filePath(proxyModel->mapToSource(selectedList[i]))));
                }
                QMimeData *mimeData = new QMimeData;
                mimeData->setUrls(urls);
                QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->setPixmap(proxyModel->getIcon(selectedList[0]).pixmap(50, 50));
                drag->setHotSpot(QPoint(25,25));
                drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
            }
        }
    }
    QWidget::mouseMoveEvent(event);//不喜欢QTreeView自实现的移动
}
void My_Table_View::enterEvent(QEvent *event)
{
    My_Table_View::catch_ptr = this;
    QWidget::enterEvent(event);
}
void My_Table_View::wheelEvent(QWheelEvent *event)
{
    if (QGuiApplication::queryKeyboardModifiers() & Qt::ControlModifier)
    {
        int delta = event->angleDelta().y();
        if (delta != 0)
        {
            int newSize = iconSize().width() + (delta > 0 ? 4 : -4);
            newSize = qBound(4, newSize, 1024);
            setIconSize(QSize(newSize, newSize));
            setGridSize(QSize(newSize + 32, newSize + 32));
            event->accept();
            return;
        }
    }
    QListView::wheelEvent(event);
}
void My_Table_View::updateStatusBar_style()
{
    m_statusBar->setStyleSheet(QString("QStatusBar{background:rgba(240,240,240,150); color:rgba(%1,%2,%3,%4); border-radius: 7px 7px;}").arg(statusBar_text_color.red()).arg(statusBar_text_color.green()).arg(statusBar_text_color.blue()).arg(statusBar_text_color.alpha()));
    statusLabel->setStyleSheet(QString("QLabel{background:rgba(0,0,0,0); color:rgba(%1,%2,%3,%4);}").arg(statusBar_text_color.red()).arg(statusBar_text_color.green()).arg(statusBar_text_color.blue()).arg(statusBar_text_color.alpha()));
}
void My_Table_View::updateStatusBar()
{
    QMargins margin = viewportMargins();
    margin.setBottom(m_statusBar->height() * m_statusBar->isVisible());
    this->setViewportMargins(margin);
    temp_folder_total_size = 0;
    if (!m_statusBar->isVisible())
    {
        m_sizeUpdateTimer->stop();
        if (m_futureWatcher->isRunning())
        {
            m_cancelCalculation = true;
            m_futureWatcher->setPaused(true);
            m_futureWatcher->cancel();
            m_futureWatcher->future().cancel();
        }
        m_currentDirPath.clear();
    }
    if (!m_statusBar->isVisible() || !F_model || !proxyModel || !selectionModel())
    {
        return;
    }
    QModelIndexList selected = selectionModel()->selectedIndexes();
    QSet<QModelIndex> uniqueRows;
    for (const QModelIndex &idx : selected)
    {
        if (idx.column() == 0)
        {
            uniqueRows.insert(idx);
        }
    }
    qint64 selectedFileCount = 0, selectedFileSize = 0;
    qint64 selectedFolderCount = 0, selectedFolderChildrenCount = 0;
    for (const QModelIndex &idx : uniqueRows)
    {
        QModelIndex srcIdx = proxyModel->mapToSource(idx);
        QFileInfo info(F_model->filePath(srcIdx));
        if (info.isFile())
        {
            ++selectedFileCount;
            selectedFileSize += info.size();
        }
        else if (info.isDir())
        {
            ++selectedFolderCount;
            selectedFolderChildrenCount += QDir(info.absoluteFilePath()).entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden).count();
        }
    }
    QModelIndex rootProxy = rootIndex();
    qint64 totalFileCount = 0, totalFileSize = 0;
    qint64 totalFolderCount = 0;
    recurseStat(rootProxy, totalFileCount, totalFileSize, totalFolderCount);

    QStringList path_list;
    for (int i = 0; i < selected.count(); i += 1)
    {
        path_list << F_model->filePath(proxyModel->mapToSource(selected[i]));
    }
    if (path_list != m_currentDirPath)
    {
        m_currentDirPath = path_list;
        if (!m_currentDirPath.isEmpty())
        {
            if (m_futureWatcher->isRunning())
            {
                m_cancelCalculation = true;
                m_futureWatcher->setPaused(true);
                m_futureWatcher->cancel();
                m_futureWatcher->future().cancel();
                //m_futureWatcher->waitForFinished(); //delay no more [doge]
            }
            m_cancelCalculation = false;
            temp_folder_total_size = 0;
            updateFolderSize();
            m_sizeUpdateTimer->start();
            for_bar_text = tr("选择: %1 个文件 (共 %2)  %3 个文件夹(包含 %4 项) [总选择大小:%8] \n根文件夹: 总文件: %5 个 (共 %6)  总文件夹: %7 个")
                    .arg(selectedFileCount)
                    .arg(formatSize(selectedFileSize))
                    .arg(selectedFolderCount)
                    .arg(selectedFolderChildrenCount)
                    .arg(totalFileCount)
                    .arg(formatSize(totalFileSize))
                    .arg(totalFolderCount);
        }
    }

    QString statusText = tr("选择: %1 个文件 (共 %2)  %3 个文件夹(包含 %4 项) [总选择大小:%5] \n根文件夹: 总文件: %6 个 (共 %7)  总文件夹: %8 个")
            .arg(selectedFileCount)
            .arg(formatSize(selectedFileSize))
            .arg(selectedFolderCount)
            .arg(selectedFolderChildrenCount)
            .arg(temp_folder_total_size)
            .arg(totalFileCount)
            .arg(formatSize(totalFileSize))
            .arg(totalFolderCount);
    statusLabel->setText(statusText);
}
#include <QtConcurrent/QtConcurrent>
void My_Table_View::updateFolderSize()
{
    if (!this->statusLabel->isVisible())
    {
        if (m_futureWatcher->isRunning())
        {
            m_cancelCalculation = true;
            m_futureWatcher->setPaused(true);
            m_futureWatcher->cancel();
            m_futureWatcher->future().cancel();
        }
        return;
    }
    if (m_currentDirPath.isEmpty())
    {
        return;
    }
    m_cancelCalculation = false;
    // 异步计算
    QFuture<qint64> future = QtConcurrent::run([this]() -> qint64
    {
        temp_folder_total_size = 0;
        for (int i = 0; i < m_currentDirPath.count(); ++i)
        {
            QFileInfo fileinfo(m_currentDirPath[i]);
            if (fileinfo.isFile())
            {
                if (m_cancelCalculation.load())
                {
                    return temp_folder_total_size;
                }
                temp_folder_total_size += fileinfo.size();
            }
            else if (fileinfo.isDir())
            {
                QDirIterator it(m_currentDirPath[i], QDir::Files | QDir::Hidden | QDir::NoSymLinks, QDirIterator::Subdirectories);
                while (it.hasNext())
                {
                    if (m_cancelCalculation.load())
                    {
                        return temp_folder_total_size;
                    }
                    it.next();//QDirIterator特性
                    temp_folder_total_size += it.fileInfo().size();
                }
            }
        }
        return temp_folder_total_size;
    });
    m_futureWatcher->setFuture(future);
}
void My_Table_View::onSizeCalculated()
{
    if (!this->statusLabel->isVisible())
    {
        return;
    }
    if (!m_futureWatcher)
    {
        return;
    }
    if (m_currentDirPath.isEmpty())
    {
        return;
    }
    if (!m_futureWatcher->isFinished())
    {
        QString sizeStr = formatSize(temp_folder_total_size);
        statusLabel->setText(for_bar_text.arg(sizeStr + tr("(计算中)")));
        return;
    }
    QFuture<long long> future = m_futureWatcher->future();
    if (!future.isResultReadyAt(0))
    {
        return;
    }
    m_sizeUpdateTimer->stop();
    QString sizeStr = formatSize(future.result());
    statusLabel->setText(for_bar_text.arg(sizeStr));
}
void My_Table_View::resizeEvent(QResizeEvent *event)
{
    QListView::resizeEvent(event);
    updateStatusBar();
    if (m_statusBar->isVisible())
    {
        m_statusBar->setGeometry(0, height() - m_statusBar->height(), width(), m_statusBar->height());
    }
}
QString My_Table_View::formatSize(qint64 bytes)
{
    const char* units[] = {"B", "KiB", "MiB", "GiB"};
    double value = bytes;
    int unitIdx = 0;
    while (unitIdx < 3 && value > 2048.0)
    {
        value /= 1024.0;
        ++unitIdx;
    }
    return QString("%1 %2").arg(std::round(value * 100) / 100).arg(units[unitIdx]);
}
void My_Table_View::recurseStat(const QModelIndex proxyParent, qint64 &outFileCount, qint64 &outFileSize, qint64 &outFolderCount)
{    
    if (model() == proxyModel)
    {
        QDir root_dir(F_model->filePath(proxyModel->mapToSource(proxyParent)));
        auto file_list = root_dir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);
        for (int r = 0; r < file_list.count(); ++r)
        {
            QFileInfo info(root_dir.filePath(file_list[r]));
            if (info.isDir())
            {
                ++outFolderCount;
            }
            else
            {
                ++outFileCount;
                outFileSize += info.size();
            }
        }
    }
    else
    {
        auto file_list = proxyModel->flatModel()->file_list();
        for (int r = 0; r < file_list.count(); ++r)
        {
            QFileInfo info(file_list[r]);
            if (info.isDir())
            {
                ++outFolderCount;
            }
            else
            {
                ++outFileCount;
                outFileSize += info.size();
            }
        }
    }
}
void File_Table::update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, QString checked_icon_path)
{
    m_dialog->update_style(theme_color, theme_background_color, theme_text_color, select_text_color, disabled_text_color, checked_icon_path);
    preview_file_widget->update_style(theme_color, theme_background_color, theme_text_color, select_text_color, disabled_text_color, checked_icon_path);
}
