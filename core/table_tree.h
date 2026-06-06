#ifndef TABLE_TREE_H
#define TABLE_TREE_H
#include "basic_widget.h"
#include <QListView>
#include <QFileSystemModel>
#include "core/tools/multilinetextinputdialog.h"
#include "core/module/preview_file_widget.h"
#include "core/module/my_icon_provider.h"
class My_TableView_Delegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    explicit My_TableView_Delegate(QObject *parent, QColor *m_hover_color = nullptr, QColor *m_select_color = nullptr, int *m_radius = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    QColor *hover_color = nullptr;
    QColor *select_color = nullptr;
    int *radius = nullptr;
};
class My_Table_View;
class My_Table_ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit My_Table_ProxyModel(QObject *parent = nullptr, My_Table_View *m_root = nullptr, int *m_sort_type = nullptr, QFileSystemModel *m_fsModel = nullptr);
    void setSearchPattern(const QString &pattern, bool deeply_search = false);
    void setShowHidden(bool show);
protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    virtual bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
private:
    My_Table_View *root = nullptr;
    QString m_pattern;
    bool m_showHidden = false;
    bool m_deeply_search = false;
    char my_padding[6];
    int *sort_type_ptr = nullptr;
    QFileSystemModel *fsModel = nullptr;

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
class My_Table_View : public QListView
{
    Q_OBJECT
public:
    explicit My_Table_View(QWidget *parent, QString *m_root_path_ptr);
    ~My_Table_View() override;
    void p_save(QSettings *settings);
    void p_load(QSettings *settings);
    QFileSystemModel *F_model = nullptr;
    My_Table_ProxyModel *proxyModel = nullptr;
    static My_Table_View * catch_ptr;
    QString *root_path_ptr = nullptr;
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
class File_Table : public Basic_Widget
{
    Q_OBJECT
public:
    explicit File_Table(QWidget *parent);
    ~File_Table();
    QList<File_Table *> *file_table_list;
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
    QColor hover_color = QColor(227, 242, 253, 255);
    QColor select_color = QColor(0, 170, 255, 255);
    int radius = 10;
    int sort_type = 0;
    void set_tree_view_style();
    QWidget *carrier_widget = new QWidget(this->get_self());
    QFileSystemModel *model = new QFileSystemModel(this);
    My_Table_View *treeView = new My_Table_View(carrier_widget, &root_path);
    My_Table_ProxyModel *proxyModel = new My_Table_ProxyModel(this, treeView, &sort_type, model);
    My_TableView_Delegate *my_delegate = new My_TableView_Delegate(this, &hover_color, &select_color, &radius);
    My_Icon_Provider *icon_provider = new My_Icon_Provider;
    QLineEdit *search_edit = new QLineEdit(carrier_widget);
    QPushButton *deeply_search_button = new QPushButton(tr("深层搜索"), carrier_widget);
    QAction *search_img_action = new QAction(this);
    QAction *search_del_action = new QAction(this);
    QMenu *menu = new QMenu(this);

    QAction *open_it = new QAction(tr("打开"), this);
    QAction *open_way = new QAction(tr("打开方式"), this);
    QAction *open_path_way = new QAction(tr("打开所在位置"), this);

    QAction *open_in_Terminal_action = new QAction(tr("在终端中打开"), this);

    QMenu *compress_menu = new QMenu("压缩", this);
    QAction *compressor_action = new QAction(tr("压缩"), this);
    QAction *save_as_zip_action = new QAction(tr("压缩到zip"), this);
    QAction *save_as_7z_action = new QAction(tr("压缩到7z"), this);

    QMenu *create_menu = new QMenu(tr("新建"), this);
    QAction *create_new_file = new QAction(tr("新建文件"), this);
    QAction *create_new_folder = new QAction(tr("新建文件夹"), this);

    QMenu *control_menu = new QMenu(tr("控制"), this);
    QAction *set_as_path_way = new QAction(tr("进入"), this);
    QAction *set_parent_as_path_way = new QAction(tr("返回上级"), this);
    QAction *show_hidden_action = new QAction(tr("显示隐藏文件"), this);
    QAction *preview_file_action = new QAction(tr("显示预览窗口"), this);

    QAction *select_all_action = new QAction(tr("全选"), this);
    QAction *clean_selection_action = new QAction(tr("清除选择"), this);
    QAction *refresh_action = new QAction(tr("刷新"), this);

    QAction *cut_action = new QAction(tr("剪切"), this);
    QAction *copy_action = new QAction(tr("复制"), this);
    QAction *paste_action = new QAction(tr("粘贴"), this);
    QAction *rename_action = new QAction(tr("重命名"), this);
    QAction *delete_action = new QAction(tr("删除"), this);
    QAction *show_info = new QAction(tr("属性"), this);

    QMenu *tree_setting = new QMenu(tr("图标视图控制"), this);
    QAction *single_press_mode_action = new QAction(tr("单击模式"), this);
    QAction *set_dir_path = new QAction(tr("设置文件夹路径"), this);
    QMenu *set_style_menu = new QMenu(tr("设置外观"), this);
    QAction *set_show_status_bar = new QAction(tr("显示状态栏"), this);
    QAction *set_show_status_bar_text_color = new QAction(tr("设置状态栏字体颜色"), this);

    QMenu *sort_type_menu = new QMenu(tr("排序方式"), this);
    QAction *sort_type_name = new QAction(tr("名称(顺)"), this);
    QAction *sort_type_size = new QAction(tr("大小(顺)"), this);
    QAction *sort_type_date = new QAction(tr("日期(顺)"), this);
    QAction *sort_type_name_Z = new QAction(tr("名称(反)"), this);
    QAction *sort_type_size_Z = new QAction(tr("大小(反)"), this);
    QAction *sort_type_date_Z = new QAction(tr("日期(反)"), this);

    QAction *set_icon_size_action = new QAction(tr("图标大小"), this);
    QAction *set_font_action = new QAction(tr("字体"), this);
    QAction *set_hover_color = new QAction(tr("悬停颜色"), this);
    QAction *set_select_color = new QAction(tr("选择颜色"), this);
    QAction *set_select_radius = new QAction(tr("圆角大小"), this);

    //shortcut
    QAction *shortcut_enter = new QAction(tr("打开"), this);
    QAction *shortcut_copy_action = new QAction(tr("复制"), this);
    QAction *shortcut_cut_action = new QAction(tr("剪切"), this);
    QAction *shortcut_paste_action = new QAction(tr("粘贴"), this);
    QAction *shortcut_delete_action = new QAction(tr("删除"), this);
    QAction *shortcut_force_delete_action = new QAction(tr("强制删除"), this);
    QAction *shortcut_rename_action = new QAction(tr("重命名"), this);
    QAction *shortcut_find_action = new QAction(tr("搜索"), this);
    QAction *shortcut_show_info = new QAction(tr("属性"), this);
    QAction *shortcut_show_hidden_action = new QAction(tr("显示隐藏文件"), this);
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

#endif // TABLE_TREE_H
