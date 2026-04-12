#ifndef FILE_TREE_H
#define FILE_TREE_H
#include "basic_widget.h"
#include <QTreeView>
#include <QFileSystemModel>
class My_TreeView_Delegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    explicit My_TreeView_Delegate(QObject *parent, QColor *m_hover_color = nullptr, QColor *m_select_color = nullptr, int *m_radius = nullptr, QModelIndex *m_proposed_action_index = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QColor *hover_color = nullptr;
    QColor *select_color = nullptr;
    int *radius = nullptr;
    QModelIndex *proposed_action_index = nullptr;
};
class My_Tree_View;
class My_ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit My_ProxyModel(QObject *parent = nullptr, My_Tree_View *m_root = nullptr);
    void setSearchPattern(const QString &pattern);
    void setShowHidden(bool show);
protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
private:
    QString m_pattern;
    char my_padding[7];
    bool m_showHidden = false;
    My_Tree_View *root = nullptr;
};
class My_Tree_View : public QTreeView
{
    Q_OBJECT
public:
    explicit My_Tree_View(QWidget *parent);
    QFileSystemModel *F_model = nullptr;
    My_ProxyModel *proxyModel = nullptr;
    static My_Tree_View * catch_ptr;
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
};
class My_Icon_Provider : public QFileIconProvider
{
public:
    My_Icon_Provider() = default;
    QIcon icon(IconType type) const override;
    QIcon icon(const QFileInfo &info) const override;
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
protected:
    QModelIndex proposed_action_index;
    QColor hover_color = QColor(227, 242, 253, 255);
    QColor select_color = QColor(0, 170, 255, 255);
    int radius = 10;
    void set_tree_view_style();
    QWidget *carrier_widget = new QWidget(this->get_self());
    QFileSystemModel *model = new QFileSystemModel(this);
    My_Tree_View *treeView = new My_Tree_View(carrier_widget);
    My_ProxyModel *proxyModel = new My_ProxyModel(this, treeView);
    My_TreeView_Delegate *my_delegate = new My_TreeView_Delegate(this, &hover_color, &select_color, &radius, &proposed_action_index);
    My_Icon_Provider *icon_provider = new My_Icon_Provider;
    QLineEdit *search_edit = new QLineEdit(carrier_widget);
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

    QAction *select_all_action = new QAction(tr("全选"), this);
    QAction *clean_selection_action = new QAction(tr("清除选择"), this);
    QAction *refresh_action = new QAction(tr("刷新"), this);

    QAction *cut_action = new QAction(tr("剪切"), this);
    QAction *copy_action = new QAction(tr("复制"), this);
    QAction *paste_action = new QAction(tr("粘贴"), this);
    QAction *rename_action = new QAction(tr("重命名"), this);
    QAction *delete_action = new QAction(tr("删除"), this);
    QAction *show_info = new QAction(tr("属性"), this);

    QMenu *tree_setting = new QMenu(tr("树状视图操作"), this);
    QAction *single_press_mode_action = new QAction(tr("单击模式"), this);
    QAction *set_dir_path = new QAction(tr("设置文件夹路径"), this);
    QMenu *set_style_menu = new QMenu(tr("设置外观"), this);
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
};

#endif // FILE_TREE_H
