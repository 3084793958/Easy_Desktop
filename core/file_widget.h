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
    QString *terminal_process = nullptr;
private:
    QAction *open_way = new Trans_Action(tr("打开方式"), "打开方式", this->metaObject()->className(), this);
    QAction *open_path_way = new Trans_Action(tr("打开所在位置"), "打开所在位置", this->metaObject()->className(), this);
    QAction *open_in_Terminal_action = new Trans_Action(tr("在终端中打开"), "在终端中打开", this->metaObject()->className(), this);
    QMenu *set_file_process = new Trans_Menu(tr("设置文件信息"), "设置文件信息", this->metaObject()->className(), this);
    QAction *set_file_as_file = new Trans_Action(tr("文件"), "文件", this->metaObject()->className(), this);
    QAction *set_file_as_dir = new Trans_Action(tr("文件夹"), "文件夹", this->metaObject()->className(), this);
    QAction *show_info = new Trans_Action(tr("属性"), "属性", this->metaObject()->className(), this);
private:
    void contextMenuEvent(QContextMenuEvent *event);
    void update_running_path();
    QString get_running_process(QString m_file_path);
    virtual void Extra_Pressed_Do();
    virtual void Set_Base_Icon();
};

#endif // FILE_WIDGET_H
