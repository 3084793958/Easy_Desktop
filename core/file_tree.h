#ifndef FILE_TREE_H
#define FILE_TREE_H
#include "basic_widget.h"
#include <QTreeView>
#include <QFileSystemModel>
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
    QFileSystemModel *model = new QFileSystemModel(this);
    QWidget *carrier_widget = new QWidget(this->get_self());
    QTreeView *treeView = new QTreeView(carrier_widget);
    QLineEdit *search_edit = new QLineEdit(carrier_widget);
    QAction *search_img_action = new QAction(this);
    QAction *search_del_action = new QAction(this);
    QMenu *menu = new QMenu(this);
    QMenu *tree_setting = new QMenu(tr("树状视图操作"), this);
    QAction *set_dir_path = new QAction(tr("设置文件夹路径"), this);
    QAction *refresh_action = new QAction(tr("刷新"), this);
    QAction *open_it = new QAction(tr("打开"), this);
    QAction *open_way = new QAction(tr("打开方式"), this);
    QAction *open_path_way = new QAction(tr("打开所在位置"), this);
    QAction *show_info = new QAction(tr("属性"), this);
private:
    void contextMenuEvent(QContextMenuEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
    void X11_Raise();
};

#endif // FILE_TREE_H
