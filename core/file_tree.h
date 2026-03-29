#ifndef FILE_TREE_H
#define FILE_TREE_H
#include "basic_widget.h"
#include <QTreeView>
#include <QFileSystemModel>
class My_TreeView_Delegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    explicit My_TreeView_Delegate(QObject *parent, QColor *m_hover_color = nullptr, QColor *m_select_color = nullptr, int *m_radius = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QColor *hover_color = nullptr;
    QColor *select_color = nullptr;
    int *radius = nullptr;
};
class My_ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit My_ProxyModel(QObject *parent = nullptr);
    void setSearchPattern(const QString &pattern);
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
    virtual void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;
private:
    QString m_pattern;
};
class My_Tree_View : public QTreeView
{
    Q_OBJECT
public:
    explicit My_Tree_View(QWidget *parent);
    QFileSystemModel *F_model;
    My_ProxyModel *proxyModel;
    static My_Tree_View * catch_ptr;
protected:
    void dropEvent(QDropEvent *event) override;
    void dragEnterEvent(QDragEnterEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    virtual void enterEvent(QEvent *event) override;
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
public:
    QString *file_open_way_process;
    QString *file_open_path_process;
    QString *file_open_info_process;
    bool *m_allow_drop;
    QString root_path = QDir::rootPath();
protected:
    QColor hover_color = QColor(227, 242, 253, 255);
    QColor select_color = QColor(0, 170, 255, 255);
    int radius = 10;
    void set_tree_view_style();
    QFileSystemModel *model = new QFileSystemModel(this);
    My_ProxyModel *proxyModel = new My_ProxyModel(this);
    QWidget *carrier_widget = new QWidget(this->get_self());
    My_Tree_View *treeView = new My_Tree_View(carrier_widget);
    My_TreeView_Delegate *my_delegate = new My_TreeView_Delegate(this, &hover_color, &select_color, &radius);
    My_Icon_Provider *icon_provider = new My_Icon_Provider;
    QLineEdit *search_edit = new QLineEdit(carrier_widget);
    QAction *search_img_action = new QAction(this);
    QAction *search_del_action = new QAction(this);
    QMenu *menu = new QMenu(this);
    QMenu *tree_setting = new QMenu(tr("树状视图操作"), this);
    QAction *single_press_mode_action = new QAction(tr("单击模式"), this);
    QAction *set_dir_path = new QAction(tr("设置文件夹路径"), this);
    QAction *refresh_action = new QAction(tr("刷新"), this);
    QAction *clean_selection_action = new QAction(tr("清除选择"), this);
    QMenu *set_style_menu = new QMenu(tr("设置外观"), this);
    QAction *set_icon_size_action = new QAction(tr("设置图标大小"), this);
    QAction *set_font_action = new QAction(tr("设置字体"), this);
    QAction *set_hover_color = new QAction(tr("设置hover颜色"), this);
    QAction *set_select_color = new QAction(tr("设置select颜色"), this);
    QAction *set_select_radius = new QAction(tr("设置圆角大小"), this);
    QAction *open_it = new QAction(tr("打开"), this);
    QAction *open_way = new QAction(tr("打开方式"), this);
    QAction *open_path_way = new QAction(tr("打开所在位置"), this);
    QAction *set_as_path_way = new QAction(tr("设置为文件夹路径"), this);
    QAction *show_info = new QAction(tr("属性"), this);
    QAction *copy_action = new QAction(tr("复制"), this);
    QAction *cut_action = new QAction(tr("剪切"), this);
    QAction *paste_action = new QAction(tr("粘贴"), this);
    QAction *rename_action = new QAction(tr("重命名"), this);
    QAction *delete_action = new QAction(tr("删除"), this);
    QAction *shortcut_copy_action = new QAction(tr("复制"), this);
    QAction *shortcut_show_info = new QAction(tr("属性"), this);
    QAction *shortcut_enter = new QAction(tr("打开"), this);
    QAction *shortcut_cut_action = new QAction(tr("剪切"), this);
    QAction *shortcut_paste_action = new QAction(tr("粘贴"), this);
    QAction *shortcut_delete_action = new QAction(tr("删除"), this);
    QAction *shortcut_force_delete_action = new QAction(tr("删除"), this);
    QAction *shortcut_find_action = new QAction(tr("搜索"), this);
private:
    void contextMenuEvent(QContextMenuEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    void Pressed(bool from_key = false);
};

#endif // FILE_TREE_H
