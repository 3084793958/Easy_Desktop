#include "zip_treeview.h"
#include <QScrollBar>
#include <QDebug>
#include <cmath>
#include <QMimeDatabase>
#include <QWheelEvent>
#include <QGuiApplication>
#include <QDateTime>
#include <QHeaderView>
#include <QInputDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QFontDatabase>
#include <QApplication>
#include <QMimeData>
#include <QDrag>
#include <QClipboard>

#include "core/tools/trans_sender.h"

Zip_TreeView * Zip_TreeView::catch_ptr;
//将路径列表构建为QStandardItemModel
void Zip_TreeView::buildTreeModelFromPaths(QStandardItemModel *model, const QList<Paths_File_Info> &paths)
{
    if (!model)
    {
        return;
    }
    model->clear();
    model->setHorizontalHeaderLabels(QStringList() << tr("名称") << tr("大小") << tr("类型") << tr("修改日期"));

    QMap<QString, const Paths_File_Info *> pathToInfo;
    for (const Paths_File_Info &info : paths)
    {
        QString stdPath = info.name;
        if (stdPath.endsWith('/'))
        {
            stdPath.chop(1);
        }
        pathToInfo.insert(stdPath, &info);
    }
    QMap<QString, QStandardItem *> pathToItem;
    QStandardItem *root = model->invisibleRootItem();

    for (const Paths_File_Info &info : paths)
    {
        QString stdPath = info.name;
        if (stdPath.endsWith('/'))
        {
            stdPath.chop(1);
        }
        if (stdPath.isEmpty())
        {
            continue;
        }
        QStringList parts = stdPath.split('/', Qt::SkipEmptyParts);
        if (parts.isEmpty())
        {
            continue;
        }
        QString currentPath;
        QStandardItem *parent = root;
        for (int j = 0; j < parts.size(); ++j)
        {
            const QString &part = parts[j];
            currentPath += (currentPath.isEmpty() ? part : "/" + part);

            //造
            QStandardItem *item = pathToItem.value(currentPath);
            if (!item)
            {
                item = new QStandardItem(part);
                QStandardItem *sizeItem = new QStandardItem;
                QStandardItem *typeItem = new QStandardItem(tr("文件夹"));
                QStandardItem *timeItem = new QStandardItem;
                parent->appendRow({item, sizeItem, typeItem, timeItem});
                pathToItem[currentPath] = item;
            }

            //算
            if (pathToInfo.contains(currentPath))
            {
                const Paths_File_Info *infoPtr = pathToInfo[currentPath];
                bool isFolder = infoPtr->name.endsWith('/');
                if (isFolder)
                {
                    item->setIcon(QIcon::fromTheme("folder"));
                }
                else
                {
                    QMimeDatabase mimeDb;
                    QMimeType mimeType;
                    mimeType = mimeDb.mimeTypeForFile(part);
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
                    item->setIcon(QIcon::fromTheme(theme_name));
                }
                QModelIndex idx = item->index();
                QStandardItem *sizeItem = model->itemFromIndex(idx.sibling(idx.row(), 1));
                QStandardItem *typeItem = model->itemFromIndex(idx.sibling(idx.row(), 2));
                QStandardItem *timeItem = model->itemFromIndex(idx.sibling(idx.row(), 3));
                if (sizeItem && typeItem && timeItem)
                {
                    if (isFolder)
                    {
                        sizeItem->setText(tr("文件夹"));
                        typeItem->setText(tr("文件夹"));
                    }
                    else
                    {
                        sizeItem->setText(formatSize(infoPtr->length.toLongLong()));
                        QString suffix = infoPtr->name.split(".").last();
                        if (suffix.isEmpty() || infoPtr->name == suffix)
                        {
                            typeItem->setText(tr("文件"));
                        }
                        else
                        {
                            typeItem->setText(suffix + " " + tr("文件"));
                        }
                    }
                    timeItem->setText(infoPtr->date + " " + infoPtr->time);
                }
                item->setData(QList<QVariant>() << "name" << isFolder << currentPath << infoPtr->length.toLongLong(), Qt::UserRole);
                sizeItem->setData(QList<QVariant>() << "size" << infoPtr->length.toLongLong() << isFolder, Qt::UserRole);
                typeItem->setData(QList<QVariant>() << "type" << isFolder, Qt::UserRole);
                timeItem->setData(QList<QVariant>() << "date" << QDateTime::fromString(infoPtr->date + " " + infoPtr->time, "yyyy-MM-dd hh:mm:ss"), Qt::UserRole);//时间不关心你是什么
            }
            parent = item;
        }
    }
}

void Zip_TreeView::refreshTreeModel(QStandardItemModel *model, const QList<Paths_File_Info> &paths)
{
    if (!model)
    {
        return;
    }

    //构建新路径信息映射
    QSet<QString> newPathSet;
    QMap<QString, const Paths_File_Info *> pathToInfo;
    for (const Paths_File_Info &info : paths)
    {
        QString stdPath = info.name;
        if (stdPath.endsWith('/'))
        {
            stdPath.chop(1);
        }
        if (stdPath.isEmpty())
        {
            continue;
        }
        newPathSet.insert(stdPath);
        pathToInfo.insert(stdPath, &info);
    }

    //获取映射
    QMap<QString, QStandardItem *> existingPathToItem;
    QStandardItem *root = model->invisibleRootItem();
    collectExistingItems(root, existingPathToItem);

    QSet<QString> processedPaths;

    for (const QString &path : newPathSet)
    {
        processedPaths.insert(path);
        QStandardItem *item = existingPathToItem.value(path);
        if (!item)
        {
            createPathInModel(model, path, pathToInfo, existingPathToItem);
        }
        else
        {
            updateNodeData(model, item, path, pathToInfo);
        }
        existingPathToItem.remove(path);
    }
    for (auto it = existingPathToItem.begin(); it != existingPathToItem.end(); ++it)
    {
        QStandardItem *item = it.value();
        QStandardItem *parent = item->parent() ? item->parent() : root;
        parent->removeRow(item->row());
    }
}
void Zip_TreeView::collectExistingItems(QStandardItem *parent, QMap<QString, QStandardItem *> &pathToItem)
{
    for (int i = 0; i < parent->rowCount(); ++i)
    {
        QStandardItem *item = parent->child(i, 0);
        if (!item)
        {
            continue;
        }
        QVariant data = item->data(Qt::UserRole);
        if (data.canConvert<QList<QVariant>>())
        {
            QList<QVariant> list = data.toList();
            if (list.size() >= 3 && list[0].toString() == "name")
            {
                QString path = list[2].toString();
                pathToItem.insert(path, item);
                collectExistingItems(item, pathToItem);
            }
        }
    }
}
void Zip_TreeView::createPathInModel(QStandardItemModel *model, const QString &path, const QMap<QString, const Paths_File_Info *> &pathToInfo, QMap<QString, QStandardItem *> &existingPathToItem)
{
    QStringList parts = path.split('/', Qt::SkipEmptyParts);
    if (parts.isEmpty())
    {
        return;
    }
    QStandardItem *root = model->invisibleRootItem();
    QString currentPath;
    QStandardItem *parent = root;

    for (int i = 0; i < parts.size(); ++i)
    {
        const QString &part = parts[i];
        currentPath += (currentPath.isEmpty() ? part : "/" + part);

        QStandardItem *item = existingPathToItem.value(currentPath);
        if (!item)
        {
            item = new QStandardItem(part);
            QStandardItem *sizeItem = new QStandardItem;
            QStandardItem *typeItem = new QStandardItem;
            QStandardItem *timeItem = new QStandardItem;
            parent->appendRow({item, sizeItem, typeItem, timeItem});
            existingPathToItem.insert(currentPath, item);
        }
        if (pathToInfo.contains(currentPath))
        {
            updateNodeData(model, item, currentPath, pathToInfo);
        }
        parent = item;
    }
}
void Zip_TreeView::updateNodeData(QStandardItemModel *model, QStandardItem *item, const QString &path, const QMap<QString, const Paths_File_Info *> &pathToInfo)
{
    const Paths_File_Info *info = pathToInfo.value(path);
    if (!info)
    {
        return;
    }

    bool isFolder = info->name.endsWith('/');
    QModelIndex idx = item->index();
    QStandardItem *sizeItem = model->itemFromIndex(idx.sibling(idx.row(), 1));
    QStandardItem *typeItem = model->itemFromIndex(idx.sibling(idx.row(), 2));
    QStandardItem *timeItem = model->itemFromIndex(idx.sibling(idx.row(), 3));
    if (!sizeItem || !typeItem || !timeItem)
    {
        return;
    }

    if (isFolder)
    {
        item->setIcon(QIcon::fromTheme("folder"));
    }
    else
    {
        QMimeDatabase mimeDb;
        QMimeType mimeType = mimeDb.mimeTypeForFile(info->name);
        QString iconName = mimeType.iconName();
        QIcon icon = QIcon::fromTheme(iconName);
        if (icon.isNull()) {
            iconName = mimeType.genericIconName();
            icon = QIcon::fromTheme(iconName);
        }
        if (icon.isNull())
            iconName = "unknown";
        item->setIcon(QIcon::fromTheme(iconName));
    }

    if (isFolder)
    {
        sizeItem->setText(tr("文件夹"));
        typeItem->setText(tr("文件夹"));
    }
    else
    {
        sizeItem->setText(formatSize(info->length.toLongLong()));
        QString suffix = info->name.split(".").last();
        if (suffix.isEmpty() || info->name == suffix)
        {
            typeItem->setText(tr("文件"));
        }
        else
        {
            typeItem->setText(suffix + " " + tr("文件"));
        }
    }
    timeItem->setText(info->date + " " + info->time);

    item->setData(QList<QVariant>() << "name" << isFolder << path << info->length.toLongLong(), Qt::UserRole);
    sizeItem->setData(QList<QVariant>() << "size" << info->length.toLongLong() << isFolder, Qt::UserRole);
    typeItem->setData(QList<QVariant>() << "type" << isFolder, Qt::UserRole);
    timeItem->setData(QList<QVariant>() << "date" << QDateTime::fromString(info->date + " " + info->time, "yyyy-MM-dd hh:mm:ss"), Qt::UserRole);//时间不关心你是什么
}

void Zip_TreeView::load(QSettings *settings, QString Token)
{
    column_width1 = settings->value(Token + "column_width1", 150).toInt();
    column_width2 = settings->value(Token + "column_width2", 150).toInt();
    column_width3 = settings->value(Token + "column_width3", 150).toInt();
    column_width4 = settings->value(Token + "column_width4", 150).toInt();
    header_visual_index1 = settings->value(Token + "header_visual_index1", 0).toInt();
    header_visual_index2 = settings->value(Token + "header_visual_index2", 1).toInt();
    header_visual_index3 = settings->value(Token + "header_visual_index3", 2).toInt();
    header_visual_index4 = settings->value(Token + "header_visual_index4", 3).toInt();
    sort_section = settings->value(Token + "sort_section", 0).toInt();
    sort_order = settings->value(Token + "sort_order", true).toBool();
    hover_color = QColor::fromRgba(settings->value(Token + "hover_color", QColor(227, 242, 253, 255).rgb()).toUInt());
    select_color = QColor::fromRgba(settings->value(Token + "select_color", QColor(0, 170, 255, 255).rgb()).toUInt());
    radius = settings->value(Token + "treeview_radius", radius).toInt();
    int indentation_num = settings->value(Token + "indentation", 24).toInt();
    this->setIconSize(QSize(indentation_num, indentation_num));
    this->setIndentation(indentation_num);
    this->setFont(settings->value(Token + "text_font", QFontDatabase::systemFont(QFontDatabase::FixedFont)).value<QFont>());
    set_show_status_bar->setIconVisibleInMenu(settings->value(Token + "set_show_status_bar", false).toBool());
    this->m_statusBar->setVisible(set_show_status_bar->isIconVisibleInMenu());
    statusBar_text_color = QColor::fromRgba(settings->value(Token + "statusBar_text_color", QColor(50, 50, 50, 255).rgba()).toUInt());
    set_tree_view_style();
    updateStatusBar_style();
    updateStatusBar();
}
void Zip_TreeView::save(QSettings *settings, QString Token)
{
    if (m_model)
    {
        column_width1 = this->columnWidth(0);
        column_width2 = this->columnWidth(1);
        column_width3 = this->columnWidth(2);
        column_width4 = this->columnWidth(3);

        header_visual_index1 = this->header()->logicalIndex(0);
        header_visual_index2 = this->header()->logicalIndex(1);
        header_visual_index3 = this->header()->logicalIndex(2);
        header_visual_index4 = this->header()->logicalIndex(3);

        sort_section = this->header()->sortIndicatorSection();

        sort_order = this->header()->sortIndicatorOrder() == Qt::SortOrder::AscendingOrder;
    }
    settings->setValue(Token + "column_width1", column_width1);
    settings->setValue(Token + "column_width2", column_width2);
    settings->setValue(Token + "column_width3", column_width3);
    settings->setValue(Token + "column_width4", column_width4);
    settings->setValue(Token + "header_visual_index1", header_visual_index1);
    settings->setValue(Token + "header_visual_index2", header_visual_index2);
    settings->setValue(Token + "header_visual_index3", header_visual_index3);
    settings->setValue(Token + "header_visual_index4", header_visual_index4);
    settings->setValue(Token + "sort_section", sort_section);
    settings->setValue(Token + "sort_order", sort_order);
    settings->setValue(Token + "hover_color", hover_color.rgba());
    settings->setValue(Token + "select_color", select_color.rgba());
    settings->setValue(Token + "treeview_radius", radius);
    settings->setValue(Token + "text_font", font());
    settings->setValue(Token + "indentation", indentation());
    settings->setValue(Token + "set_show_status_bar", set_show_status_bar->isIconVisibleInMenu());
    settings->setValue(Token + "statusBar_text_color", statusBar_text_color.rgba());
}
void Zip_TreeView::set_icon(QString checked_icon_path)
{
    set_show_status_bar->setIcon(QIcon(checked_icon_path));
}
void Zip_TreeView_Carrier::set_icon(QString checked_icon_path)
{
    m_zip_treeview->set_icon(checked_icon_path);
}
QString Zip_TreeView::formatSize(qint64 bytes)
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
Zip_TreeView::Zip_TreeView(QWidget *parent)
    : QTreeView(parent)
{
    setHeaderHidden(false);
    setAlternatingRowColors(true);
    setSelectionBehavior(QAbstractItemView::SelectRows);

    this->setAlternatingRowColors(true);
    this->setLayoutDirection(Qt::LeftToRight);
    this->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    setSortingEnabled(true);
    this->setItemDelegate(my_delegate);
    this->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{border:none;background:rgba(0,0,0,0);width:8px;margin:0px0px0px0px;}"
                                                 "QScrollBar::handle:vertical{background:rgba(0,0,0,75);border-radius:4px;min-height:20px;}"
                                                 "QScrollBar::handle:vertical:hover{background:rgba(0,0,0,125);}"
                                                 "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0px;}"
                                                 "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}");
    this->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{border:none;background:rgba(0,0,0,0);height:8px;margin:0px0px0px0px;}"
                                                   "QScrollBar::handle:horizontal{background:rgba(0,0,0,75);border-radius:4px;min-width:20px;}"
                                                   "QScrollBar::handle:horizontal:hover{background:rgba(0,0,0,125);}"
                                                   "QScrollBar::add-line:horizontal,QScrollBar::sub-line:horizontal{width:0px;}"
                                                   "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:none;}");

    set_tree_view_style();

    setIconSize(QSize(24, 24));
    setIndentation(24);

    this->setAnimated(true);
    this->setSortingEnabled(true);

    this->setEditTriggers(QTreeView::EditTrigger::NoEditTriggers);//不许动

    this->m_statusBar->setFixedHeight(24);
    this->m_statusBar->hide();
    this->updateStatusBar_style();
    this->m_statusBar->setSizeGripEnabled(false);
    this->statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    this->m_statusBar->addPermanentWidget(this->statusLabel, 1);
    this->updateStatusBar();

    menu->addAction(copy_name_action);
    menu->addSeparator();
    set_show_status_bar->setIcon(QIcon(":/base/this.svg"));
    set_show_status_bar->setIconVisibleInMenu(false);
    menu->addAction(set_show_status_bar);
    menu->addAction(set_show_status_bar_text_color);
    menu->addAction(set_icon_size_action);
    menu->addAction(set_font_action);
    menu->addAction(set_hover_color);
    menu->addAction(set_select_color);
    menu->addAction(set_select_radius);

    proxyModel->setFilterKeyColumn(0);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);

    shortcut_copy_name_action->setShortcut(QKeySequence::Copy);
    shortcut_copy_name_action->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    connect(shortcut_copy_name_action, &QAction::triggered, this, [=]
    {
        if (this->selectionModel() && this == Zip_TreeView::catch_ptr)
        {
            QModelIndexList selectedList = this->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QString res = "";
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    if (!res.isEmpty()) res += "\n";
                    res += selectedList[i].data(Qt::UserRole).toList()[2].toString();
                }
                QMimeData *mimeData = new QMimeData;
                mimeData->setText(res);
                QApplication::clipboard()->setMimeData(mimeData, QClipboard::Mode::Clipboard);
            }
        }
    });

    this->addAction(shortcut_copy_name_action);

    connect(Trans_Sender::instance(), &Trans_Sender::Trans_sig, this, [=]
    {
        if (m_model)
        {
            m_model->setHorizontalHeaderLabels(QStringList() << tr("名称") << tr("大小") << tr("类型") << tr("修改日期"));
        }
    });
}
void Zip_TreeView::set_tree_view_style()
{
    setStyleSheet(QString("QTreeView{background:rgba(255,255,255,0);color:rgb(60,60,60);selection-background-color:rgba(0,0,0,0);}"
                          "QTreeView::item:first{color:rgb(0,0,0)}"
                          "QTreeView::item:selected{color:rgb(255,255,255);border: 0px solid rgba(255,255,255,0)}"
                          "QToolTip {color:rgb(60,60,60); background: rgba(%1,%2,%3,175); }")
                            .arg(hover_color.red()).arg(hover_color.green()).arg(hover_color.blue()));
}
Zip_TreeView::~Zip_TreeView()
{
    if (m_currentProcess)
    {
        m_currentProcess->kill();
        m_currentProcess->waitForFinished();
    }
}
void Zip_TreeView::setupTar(const QFileInfo &info)
{
    clear();
    m_currentArchivePath = info.absoluteFilePath();
    startArchiveListing(info);
}
void Zip_TreeView::clear()
{
    if (m_currentProcess)
    {
        m_currentProcess->kill();
        m_currentProcess->disconnect();
        m_currentProcess->deleteLater();
        m_currentProcess = nullptr;
    }

    if (m_model)
    {
        column_width1 = this->columnWidth(0);
        column_width2 = this->columnWidth(1);
        column_width3 = this->columnWidth(2);
        column_width4 = this->columnWidth(3);

        header_visual_index1 = this->header()->logicalIndex(0);
        header_visual_index2 = this->header()->logicalIndex(1);
        header_visual_index3 = this->header()->logicalIndex(2);
        header_visual_index4 = this->header()->logicalIndex(3);

        sort_section = this->header()->sortIndicatorSection();

        sort_order = this->header()->sortIndicatorOrder() == Qt::SortOrder::AscendingOrder;

        disconnect(this->selectionModel(), &QItemSelectionModel::selectionChanged, this, nullptr);
        disconnect(this->selectionModel(), &QItemSelectionModel::currentChanged, this, nullptr);
        delete m_model;
        m_model = nullptr;
        setModel(nullptr);
    }
}
void Zip_TreeView::startArchiveListing(const QFileInfo &info)
{
    QString fileName = info.fileName();
    QString suffix = info.suffix().toLower();
    QStringList args;
    QString program;

    if (suffix == "zip")
    {
        program = "unzip";
        args << "-l" << info.absoluteFilePath();
    }
    else if (fileName.endsWith(".tar.gz") || suffix == "tgz")
    {
        program = "tar";
        args << "-tzvf" << info.absoluteFilePath();
    }
    else if (fileName.endsWith(".tar.bz2") || suffix == "tbz" || suffix == "tbz2")
    {
        program = "tar";
        args << "-tjvf" << info.absoluteFilePath();
    }
    else if (fileName.endsWith(".tar.xz") || suffix == "txz")
    {
        program = "tar";
        args << "-tJvf" << info.absoluteFilePath();
    }
    else if (fileName.endsWith(".tar") || suffix == "tar")
    {
        program = "tar";
        args << "-tvf" << info.absoluteFilePath();
    }
    else
    {
        emit loadingFinished(false);
        return;
    }

    QProcess *process = new QProcess(this);
    m_currentProcess = process;
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, &Zip_TreeView::onProcessFinished);
    connect(process, &QProcess::errorOccurred, this, &Zip_TreeView::onProcessError);
    process->start(program, args);
}
void Zip_TreeView::onProcessFinished()
{
    QProcess *process = qobject_cast<QProcess *>(sender());
    if (!process || process != m_currentProcess)
    {
        return;
    }
    if (process->exitCode() == 0)
    {
        QByteArray output = process->readAllStandardOutput();
        QString program = process->program();
        if (program == "unzip")
        {
            parseZipOutput(output);
        }
        else if (program == "tar")
        {
            parseTarOutput(output);
        }
        emit loadingFinished(true);
    }
    else
    {
        emit loadingFinished(false);
    }
    process->disconnect();
    process->deleteLater();
    m_currentProcess = nullptr;
}
void Zip_TreeView::onProcessError()
{
    QProcess *process = qobject_cast<QProcess *>(sender());
    if (process && process == m_currentProcess)
    {
        emit loadingFinished(false);
        process->disconnect();
        process->deleteLater();
        m_currentProcess = nullptr;
    }
}
void Zip_TreeView::parseZipOutput(const QByteArray &output)
{
    QString text = QString::fromUtf8(output);
    QStringList lines = text.split('\n', Qt::SkipEmptyParts);
    QList<Paths_File_Info> paths;
    bool headerSkipped = false;
    for (int i = 0; i < lines.count(); ++i)
    {
        if (!headerSkipped)
        {
            if (lines[i].contains("Length") && lines[i].contains("Date") && lines[i].contains("Time") && lines[i].contains("Name"))//头部
            {
                headerSkipped = true;
                ++i;//其实可以交给下一个if
            }
            continue;
        }
        if (lines[i].trimmed().startsWith("---") /*|| lines[i].contains("file")*/)//尾部//TM的把正常文件都过滤了  尾部不为4项,过不了
        {
            continue;
        }
        QStringList info_list = lines[i].split(" ", Qt::SkipEmptyParts);
        if (info_list.count() != 4)
        {
            continue;
        }
        QString name = info_list[3];
        QString length = info_list[0];
        QString date = info_list[1];
        QString time = info_list[2];
        if (!name.isEmpty())
        {
            paths << Paths_File_Info{name, date, time, length};
        }
    }
    buildTreeModel(paths);
}
void Zip_TreeView::parseTarOutput(const QByteArray &output)
{
    QStringList lines = QString::fromUtf8(output).split('\n', Qt::SkipEmptyParts);
    QList<Paths_File_Info> paths;
    for (int i = 0; i < lines.count(); ++i)
    {
        if (lines[i].startsWith("tar: "))
        {
            continue;
        }
        QStringList info_list = lines[i].split(" ", Qt::SkipEmptyParts);
        if (info_list.count() != 6)
        {
            continue;
        }
        QString name = info_list[5];
        QString length = info_list[2];
        QString date = info_list[3];
        QString time = info_list[4];
        if (!name.isEmpty())
        {
            paths << Paths_File_Info{name, date, time, length};
        }
    }
    buildTreeModel(paths);
}
void Zip_TreeView::buildTreeModel(const QList<Paths_File_Info> &paths)
{
    if (m_model)
    {
        disconnect(this->selectionModel(), &QItemSelectionModel::selectionChanged, this, nullptr);
        disconnect(this->selectionModel(), &QItemSelectionModel::currentChanged, this, nullptr);
        delete m_model;
        m_model = nullptr;
    }
    m_model = new Zip_View_Model(this);
    buildTreeModelFromPaths(m_model, paths);
    save_info_list = paths;
    proxyModel->setSourceModel(m_model);
    setModel(proxyModel);

    connect(this->selectionModel(), &QItemSelectionModel::selectionChanged, this, [=]
    {
        this->updateStatusBar();
    });
    connect(this->selectionModel(), &QItemSelectionModel::currentChanged, this, [=]
    {
        this->updateStatusBar();
    });

    QList<int> savedOrder = {};
    savedOrder << header_visual_index1 << header_visual_index2 << header_visual_index3 << header_visual_index4;
    bool oldBlock = this->header()->blockSignals(true);
    for (int targetVisual = 0; targetVisual < savedOrder.size(); ++targetVisual)
    {
        int expectedLogical = savedOrder[targetVisual];
        int currentVisual = this->header()->visualIndex(expectedLogical);
        if (currentVisual != targetVisual)
        {
            this->header()->moveSection(currentVisual, targetVisual);
        }
    }
    this->header()->blockSignals(oldBlock);

    this->setColumnWidth(0, column_width1);
    this->setColumnWidth(1, column_width2);
    this->setColumnWidth(2, column_width3);
    this->setColumnWidth(3, column_width4);

    this->header()->setSortIndicator(sort_section, sort_order ? Qt::SortOrder::AscendingOrder : Qt::SortOrder::DescendingOrder);

    expandToDepth(0);
    updateStatusBar();

    connect(Trans_Sender::instance(), &Trans_Sender::Trans_sig, this, [=]{refreshTreeModel(m_model, save_info_list);});
}
Zip_TreeView::Paths_File_Info::Paths_File_Info(QString m_name, QString m_date, QString m_time, QString m_length)
    :name(m_name)
    ,date(m_date)
    ,time(m_time)
    ,length(m_length)
{}
void Zip_TreeView::wheelEvent(QWheelEvent *event)
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
void Zip_TreeView::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == copy_name_action)
    {
        if (this->selectionModel())
        {
            QModelIndexList selectedList = this->selectionModel()->selectedIndexes();
            if (!selectedList.isEmpty())
            {
                QString res = "";
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    if (!res.isEmpty()) res += "\n";
                    res += selectedList[i].data(Qt::UserRole).toList()[2].toString();
                }
                QMimeData *mimeData = new QMimeData;
                mimeData->setText(res);
                QApplication::clipboard()->setMimeData(mimeData, QClipboard::Mode::Clipboard);
            }
        }
    }
    else if (know_what == set_icon_size_action)
    {
        bool ok = false;
        int num = QInputDialog::getInt(nullptr, tr("获取数值"), tr("大小:"), this->indentation(), 10, 2147483647, 1, &ok);
        if (ok)
        {
            this->setIconSize(QSize(num, num));
            this->setIndentation(num);
        }
    }
    else if (know_what == set_font_action)
    {
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, this->font(), nullptr);
        if (!ok)
        {
            return;
        }
        this->setFont(font);
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
        }
    }
    else if (know_what == set_show_status_bar)
    {
        set_show_status_bar->setIconVisibleInMenu(!set_show_status_bar->isIconVisibleInMenu());
        this->m_statusBar->setVisible(set_show_status_bar->isIconVisibleInMenu());
        this->updateStatusBar();
    }
    else if (know_what == set_show_status_bar_text_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(this->statusBar_text_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        this->statusBar_text_color = colorDialog.currentColor();
        this->updateStatusBar_style();
    }
}
Zip_TreeView_Carrier::Zip_TreeView_Carrier(QWidget *parent)
    :QWidget(parent)
{
    hide();
    m_searchEdit->setPlaceholderText(tr("搜索"));
    m_searchEdit->setStyleSheet(
        "QLineEdit{"
        "    border: 0px solid rgba(0,170,255,255);"
        "    border-radius:10px;"
        "    background:rgba(0,0,0,25);"
        "    font-size:15px;"
        "    color:rgb(40,40,40);"
        "    padding: 5px 10px;"
        "}"
        "QLineEdit:hover{"
        "    border: 1px solid rgba(0,170,255,255);"
        "}"
    );
    m_searchEdit->setFixedHeight(40);
    m_searchEdit->addAction(m_searchImgAction, QLineEdit::LeadingPosition);
    connect(m_searchDelAction, &QAction::triggered, this, [=]()
    {
        m_searchEdit->clear();
    });
    connect(m_searchEdit, &QLineEdit::textChanged, this, [=](const QString &text)
    {
        if (m_zip_treeview->proxyModel)
        {
            m_zip_treeview->proxyModel->setFilterFixedString(text);
        }
        if (text.isEmpty())
        {
            m_searchEdit->removeAction(m_searchDelAction);
        }
        else
        {
            m_searchEdit->addAction(m_searchDelAction, QLineEdit::TrailingPosition);
        }
    });
    setStyleSheet("background:rgba(0,0,0,0);color:rgb(230,230,230)");
    m_zip_treeview->show();
    connect(m_zip_treeview, &Zip_TreeView::loadingFinished, this, [=](bool success)
    {
        emit loadingFinished(success);
    });
    connect(Trans_Sender::instance(), &Trans_Sender::Trans_sig, this, [=]
    {
        m_searchEdit->setPlaceholderText(tr("搜索"));
    });
}
void Zip_TreeView_Carrier::setupTar(const QFileInfo &info)
{
    if (m_searchEdit)
    {
        m_searchEdit->clear();
        m_searchEdit->setEnabled(true);
    }
    m_zip_treeview->setupTar(info);
}
void Zip_TreeView_Carrier::clear()
{
    if (m_searchEdit)
    {
        m_searchEdit->clear();
        m_searchEdit->setEnabled(false);
    }
    m_zip_treeview->clear();
}
void Zip_TreeView_Carrier::load(QSettings *settings, QString Token)
{
    m_zip_treeview->load(settings, Token);
}
void Zip_TreeView_Carrier::save(QSettings *settings, QString Token)
{
    m_zip_treeview->save(settings, Token);
}
Zip_TreeView_Carrier::~Zip_TreeView_Carrier()
{
    disconnect();
}
void Zip_TreeView_Carrier::wheelEvent(QWheelEvent *event)
{
    if (m_zip_treeview == Zip_TreeView::catch_ptr)
    {
        event->accept();
        //全部都吃掉
    }
}
void Zip_TreeView_Carrier::resizeEvent(QResizeEvent *event)
{
    (void)event;
    m_searchEdit->setGeometry(5, 5, width() - 10, 40);
    int top = m_searchEdit ? m_searchEdit->y() + m_searchEdit->height() + 5 : 5;
    m_zip_treeview->setGeometry(5, top, width() - 10, height() - top - 5);
}
void Zip_TreeView::mousePressEvent(QMouseEvent *event)
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
void Zip_TreeView::mouseReleaseEvent(QMouseEvent *event)
{
    QTreeView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton && setup_rubber)
    {
        setup_rubber = false;
        m_rubberBand->hide();
    }
}
void Zip_TreeView::mouseMoveEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton && selectionModel())
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
                QString res = "";
                for (int i = 0; i < selectedList.count(); i += 4)
                {
                    if (!res.isEmpty()) res += "\n";
                    res += selectedList[i].data(Qt::UserRole).toList()[2].toString();
                }
                QMimeData *mimeData = new QMimeData;
                mimeData->setText(res);
                QDrag *drag = new QDrag(this);
                drag->setMimeData(mimeData);
                drag->setPixmap(QIcon::fromTheme("text-x-generic").pixmap(50, 50));
                drag->setHotSpot(QPoint(25,25));
                drag->exec(Qt::MoveAction | Qt::CopyAction, Qt::CopyAction);
            }
        }
    }
    QWidget::mouseMoveEvent(event);//不喜欢QTreeView自实现的移动
}
void Zip_TreeView::enterEvent(QEvent *event)
{
    Zip_TreeView::catch_ptr = this;
    QWidget::enterEvent(event);
}
void Zip_TreeView::resizeEvent(QResizeEvent *event)
{
    QTreeView::resizeEvent(event);
    updateStatusBar();
    if (m_statusBar->isVisible())
    {
        m_statusBar->setGeometry(0, height() - m_statusBar->height(), width(), m_statusBar->height());
    }
}
void Zip_TreeView::updateStatusBar_style()
{
    m_statusBar->setStyleSheet(QString("QStatusBar{background:rgba(240,240,240,150); color:rgba(%1,%2,%3,%4); border-radius: 7px 7px;}").arg(statusBar_text_color.red()).arg(statusBar_text_color.green()).arg(statusBar_text_color.blue()).arg(statusBar_text_color.alpha()));
    statusLabel->setStyleSheet(QString("QLabel{background:rgba(0,0,0,0); color:rgba(%1,%2,%3,%4);}").arg(statusBar_text_color.red()).arg(statusBar_text_color.green()).arg(statusBar_text_color.blue()).arg(statusBar_text_color.alpha()));
}
void Zip_TreeView::updateStatusBar()
{
    QMargins margin = viewportMargins();
    margin.setBottom(m_statusBar->height() * m_statusBar->isVisible());
    m_statusBar->setGeometry(0, height() - m_statusBar->height(), width(), m_statusBar->height());
    this->setViewportMargins(margin);
    if (!m_statusBar->isVisible())
    {
        return;
    }
    QModelIndexList selected = selectionModel()->selectedIndexes();
    int dir_num = 0;
    int file_num = 0;
    qint64 file_size = 0;
    for (int i = 0; i < selected.count(); i += 4)
    {
        if (selected[i].data(Qt::UserRole).toList()[1].toBool())
        {
            ++dir_num;
        }
        else
        {
            ++file_num;
            file_size += selected[i].data(Qt::UserRole).toList()[3].toLongLong();
        }
    }
    statusLabel->setText(QString(tr("选择: 文件%1个(%2); 文件夹%3个")).arg(file_num).arg(formatSize(file_size)).arg(dir_num));
}
