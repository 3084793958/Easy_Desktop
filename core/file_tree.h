#ifndef FILE_TREE_H
#define FILE_TREE_H
#include "basic_widget.h"
#include <QTreeView>
#include <QFileSystemModel>
class My_Tree_View : public QTreeView
{
    Q_OBJECT
public:
    explicit My_Tree_View(QWidget *parent);
    QFileSystemModel *F_model;
    bool **m_allow_drop;
protected:
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
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
    WId m_WinId;
    QString *file_open_way_process;
    QString *file_open_path_process;
    QString *file_open_info_process;
    bool *m_allow_drop;
    QString root_path = QDir::rootPath();
protected:
    QColor hover_color = QColor(227, 242, 253);
    QColor select_color = QColor(0, 170, 255);
    void set_tree_view_style();
    QFileSystemModel *model = new QFileSystemModel(this);
    QWidget *carrier_widget = new QWidget(this->get_self());
    My_Tree_View *treeView = new My_Tree_View(carrier_widget);
    My_Icon_Provider *icon_provider = new My_Icon_Provider;
    QLineEdit *search_edit = new QLineEdit(carrier_widget);
    QAction *search_img_action = new QAction(this);
    QAction *search_del_action = new QAction(this);
    QMenu *menu = new QMenu(this);
    QMenu *tree_setting = new QMenu(tr("树状视图操作"), this);
    QAction *set_dir_path = new QAction(tr("设置文件夹路径"), this);
    QAction *refresh_action = new QAction(tr("刷新"), this);
    QMenu *set_style_menu = new QMenu(tr("设置外观"), this);
    QAction *set_icon_size_action = new QAction(tr("设置大小"), this);
    QAction *set_hover_color = new QAction(tr("设置hover颜色"), this);
    QAction *set_select_color = new QAction(tr("设置select颜色"), this);
    QAction *open_it = new QAction(tr("打开"), this);
    QAction *open_way = new QAction(tr("打开方式"), this);
    QAction *open_path_way = new QAction(tr("打开所在位置"), this);
    QAction *show_info = new QAction(tr("属性"), this);
    QAction *copy_action = new QAction(tr("复制"), this);
private:
    void contextMenuEvent(QContextMenuEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    void X11_Raise();
};

#endif // FILE_TREE_H
