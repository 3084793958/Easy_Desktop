#ifndef FILE_WIDGET_H
#define FILE_WIDGET_H
#include "process_widget.h"

class File_Widget : public Process_Widget
{
    Q_OBJECT
public:
    explicit File_Widget(QWidget *parent);
    ~File_Widget();
    void set_file_or_dir(bool file);
    QString file_path = QDir::homePath();
    QList<File_Widget *> *file_widget_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
    void quickly_set(QString filepath);
    QString *file_open_way_process;
    QString *file_open_path_process;
    QString *file_open_info_process;
private:
    QAction *open_way = new QAction(tr("打开方式"), this);
    QAction *open_path_way = new QAction(tr("打开所在位置"), this);
    QMenu *set_file_process = new QMenu(tr("设置文件信息"), this);
    QAction *set_file_as_file = new QAction(tr("文件"), this);
    QAction *set_file_as_dir = new QAction(tr("文件夹"), this);
    QAction *show_info = new QAction(tr("属性"), this);
private:
    void contextMenuEvent(QContextMenuEvent *event);
    void update_running_path();
    QString get_running_process(QString m_file_path);
    virtual void Extra_Pressed_Do();
    virtual void Set_Base_Icon();
};

#endif // FILE_WIDGET_H
