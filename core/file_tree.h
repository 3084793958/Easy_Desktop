#ifndef FILE_TREE_H
#define FILE_TREE_H
#include "basic_widget.h"
#include <QTreeView>
#include <QFileSystemModel>
#include "core/tools/multilinetextinputdialog.h"
#include "core/module/preview_file_widget.h"
#include "core/module/my_icon_provider.h"
#include "core/module/my_treeview_delegate.h"
#include "core/module/asyncfilesystemmodel.h"
class My_Tree_View;
class My_ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit My_ProxyModel(QObject *parent = nullptr, My_Tree_View *m_root = nullptr, QFileSystemModel *m_fsModel = nullptr);
    void setSearchPattern(const QString &pattern, bool deeply_search = false);
    void setShowHidden(bool show);
protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
private:
    My_Tree_View *root = nullptr;
    QFileSystemModel *fsModel = nullptr;
    QString m_pattern;
    bool m_showHidden = false;
    bool m_deeply_search = false;
    char my_padding[6];

private:
    QFuture<bool> hasMatchInSubtreeAsync(const QModelIndex &sourceIndex) const;

    mutable QSet<QString> m_matchedDirsCache;//给const函数用
    mutable QMutex m_cacheMutex;
    mutable QSet<QString> m_pendingDirs;
    mutable QMutex m_pendingMutex;
    bool hasMatchInSubtree(const QModelIndex &sourceIndex) const;
private slots:
    void onMatchCheckFinished(const QString &dirPath, bool hasMatch) const;
};
class My_Tree_View : public QTreeView
{
    Q_OBJECT
public:
    explicit My_Tree_View(QWidget *parent, QString *m_root_path_ptr);
    ~My_Tree_View() override;
    void p_save(QSettings *settings);
    void p_load(QSettings *settings);
    QFileSystemModel *F_model = nullptr;
    My_ProxyModel *proxyModel = nullptr;
    QString *root_path_ptr = nullptr;
    static My_Tree_View * catch_ptr;
    void backToPath();
protected:
    void dropEvent(QDropEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragLeaveEvent(QDragLeaveEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
private:
    QRubberBand *m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    QPoint origin_pos = QPoint();
    bool setup_rubber = false;
    char my_padding[7];
public:
    QStatusBar *m_statusBar = new QStatusBar(this);
    QLabel *statusLabel = new QLabel(this);
    QColor statusBar_text_color = QColor(50, 50, 50, 255);
    void updateStatusBar_style();
    void updateStatusBar();
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
private:
    QString formatSize(qint64 bytes);
    void recurseStat(const QModelIndex proxyParent, qint64 &outFileCount, qint64 &outFileSize, qint64 &outFolderCount);
private:
    QTimer *m_sizeUpdateTimer = new QTimer(this);
    QFutureWatcher<qint64> *m_futureWatcher = new QFutureWatcher<qint64>(this);
    QStringList m_currentDirPath = {};
    std::atomic<qint64> temp_folder_total_size{0};
    std::atomic<bool> m_cancelCalculation{false};
    char m_padding[7];
    QString for_bar_text = "";
private slots:
    void updateFolderSize();
    void onSizeCalculated();
};
class File_Tree : public Basic_Widget
{
    Q_OBJECT
public:
    explicit File_Tree(QWidget *parent);
    ~File_Tree();
    QList<File_Tree *> *file_tree_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
    virtual void set_icon(QString checked_icon_path);
public:
    QString *file_open_way_process = nullptr;
    QString *file_open_path_process = nullptr;
    QString *file_open_info_process = nullptr;
    QString *terminal_process = nullptr;
    QString *compressor_process = nullptr;
    QString *compressor_zip_process = nullptr;
    QString *compressor_7z_process = nullptr;
    bool *m_allow_drop = nullptr;
    QString root_path = QDir::rootPath();
    void update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, QString checked_icon_path);
protected:
    QModelIndex proposed_action_index;
    QColor hover_color = QColor(227, 242, 253, 255);
    QColor select_color = QColor(0, 170, 255, 255);
    int radius = 10;
    void set_tree_view_style();
    QWidget *carrier_widget = new QWidget(this->get_self());
    AsyncFileSystemModel *model = new AsyncFileSystemModel(this);
    My_Tree_View *treeView = new My_Tree_View(carrier_widget, &root_path);
    My_ProxyModel *proxyModel = new My_ProxyModel(this, treeView, model);
    My_TreeView_Delegate *my_delegate = new My_TreeView_Delegate(this, &hover_color, &select_color, &radius, &proposed_action_index);
    My_Icon_Provider *icon_provider = new My_Icon_Provider;
    QLineEdit *search_edit = new QLineEdit(carrier_widget);
    QPushButton *deeply_search_button = new Trans_PushButton(tr("深层搜索"), "深层搜索", this->metaObject()->className(), carrier_widget);
    QAction *search_img_action = new QAction(this);
    QAction *search_del_action = new QAction(this);
    QMenu *menu = new QMenu(this);

    QAction *open_it = new Trans_Action(tr("打开"), "打开", this->metaObject()->className(), this);
    QAction *open_way = new Trans_Action(tr("打开方式"), "打开方式", this->metaObject()->className(), this);
    QAction *open_path_way = new Trans_Action(tr("打开所在位置"), "打开所在位置", this->metaObject()->className(), this);

    QAction *open_in_Terminal_action = new Trans_Action(tr("在终端中打开"), "在终端中打开", this->metaObject()->className(), this);

    QMenu *compress_menu = new QMenu("压缩", this);
    QAction *compressor_action = new Trans_Action(tr("压缩"), "压缩", this->metaObject()->className(), this);
    QAction *save_as_zip_action = new Trans_Action(tr("压缩到zip"), "压缩到zip", this->metaObject()->className(), this);
    QAction *save_as_7z_action = new Trans_Action(tr("压缩到7z"), "压缩到7z", this->metaObject()->className(), this);

    QMenu *create_menu = new Trans_Menu(tr("新建"), "新建", this->metaObject()->className(), this);
    QAction *create_new_file = new Trans_Action(tr("新建文件"), "新建文件", this->metaObject()->className(), this);
    QAction *create_new_folder = new Trans_Action(tr("新建文件夹"), "新建文件夹", this->metaObject()->className(), this);

    QMenu *control_menu = new Trans_Menu(tr("控制"), "控制", this->metaObject()->className(), this);
    QAction *set_as_path_way = new Trans_Action(tr("进入"), "进入", this->metaObject()->className(), this);
    QAction *set_parent_as_path_way = new Trans_Action(tr("返回上级"), "返回上级", this->metaObject()->className(), this);
    QAction *show_hidden_action = new Trans_Action(tr("显示隐藏文件"), "显示隐藏文件", this->metaObject()->className(), this);
    QAction *preview_file_action = new Trans_Action(tr("显示预览窗口"), "显示预览窗口", this->metaObject()->className(), this);

    QAction *select_all_action = new Trans_Action(tr("全选"), "全选", this->metaObject()->className(), this);
    QAction *clean_selection_action = new Trans_Action(tr("清除选择"), "清除选择", this->metaObject()->className(), this);
    QAction *refresh_action = new Trans_Action(tr("刷新"), "刷新", this->metaObject()->className(), this);

    QAction *cut_action = new Trans_Action(tr("剪切"), "剪切", this->metaObject()->className(), this);
    QAction *copy_action = new Trans_Action(tr("复制"), "复制", this->metaObject()->className(), this);
    QAction *paste_action = new Trans_Action(tr("粘贴"), "粘贴", this->metaObject()->className(), this);
    QAction *rename_action = new Trans_Action(tr("重命名"), "重命名", this->metaObject()->className(), this);
    QAction *delete_action = new Trans_Action(tr("删除"), "删除", this->metaObject()->className(), this);
    QAction *show_info = new Trans_Action(tr("属性"), "属性", this->metaObject()->className(), this);

    QMenu *tree_setting = new Trans_Menu(tr("树状视图控制"), "树状视图控制", this->metaObject()->className(), this);
    QAction *single_press_mode_action = new Trans_Action(tr("单击模式"), "单击模式", this->metaObject()->className(), this);
    QAction *set_dir_path = new Trans_Action(tr("设置文件夹路径"), "设置文件夹路径", this->metaObject()->className(), this);
    QMenu *set_style_menu = new Trans_Menu(tr("设置外观"), "设置外观", this->metaObject()->className(), this);
    QAction *set_show_status_bar = new Trans_Action(tr("显示状态栏"), "显示状态栏", this->metaObject()->className(), this);
    QAction *set_show_status_bar_text_color = new Trans_Action(tr("设置状态栏字体颜色"), "设置状态栏字体颜色", this->metaObject()->className(), this);
    QAction *set_icon_size_action = new Trans_Action(tr("图标大小"), "图标大小", this->metaObject()->className(), this);
    QAction *set_font_action = new Trans_Action(tr("字体"), "字体", this->metaObject()->className(), this);
    QAction *set_hover_color = new Trans_Action(tr("悬停颜色"), "悬停颜色", this->metaObject()->className(), this);
    QAction *set_select_color = new Trans_Action(tr("选择颜色"), "选择颜色", this->metaObject()->className(), this);
    QAction *set_select_radius = new Trans_Action(tr("圆角大小"), "圆角大小", this->metaObject()->className(), this);

    //shortcut
    QAction *shortcut_enter = new Trans_Action(tr("打开"), "打开", this->metaObject()->className(), this);
    QAction *shortcut_copy_action = new Trans_Action(tr("复制"), "复制", this->metaObject()->className(), this);
    QAction *shortcut_cut_action = new Trans_Action(tr("剪切"), "剪切", this->metaObject()->className(), this);
    QAction *shortcut_paste_action = new Trans_Action(tr("粘贴"), "粘贴", this->metaObject()->className(), this);
    QAction *shortcut_delete_action = new Trans_Action(tr("删除"), "删除", this->metaObject()->className(), this);
    QAction *shortcut_force_delete_action = new Trans_Action(tr("强制删除"), "强制删除", this->metaObject()->className(), this);
    QAction *shortcut_rename_action = new Trans_Action(tr("重命名"), "重命名", this->metaObject()->className(), this);
    QAction *shortcut_find_action = new Trans_Action(tr("搜索"), "搜索", this->metaObject()->className(), this);
    QAction *shortcut_show_info = new Trans_Action(tr("属性"), "属性", this->metaObject()->className(), this);
    QAction *shortcut_show_hidden_action = new Trans_Action(tr("显示隐藏文件"), "显示隐藏文件", this->metaObject()->className(), this);
    //shortcut
private:
    void contextMenuEvent(QContextMenuEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    void Pressed(bool from_key = false);
public:
    void first_set_preview_pos();
private:
    MultiLineTextInputDialog *m_dialog = new MultiLineTextInputDialog(nullptr);
    Preview_File_Widget *preview_file_widget = nullptr;
};

#endif // FILE_TREE_H
