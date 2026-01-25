#ifndef MY_LINEEDIT_H
#define MY_LINEEDIT_H
#include "basic_widget.h"
#include <QTextEdit>
class Basic_TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit Basic_TextEdit(QWidget *parent);
    void set_WinId(WId m_winId);
    virtual void H_save(QSettings *settings);
    virtual void H_load(QSettings *settings);
signals:
    void window_contextmenu(QPoint pos);
private:
    QMenu *extra_menu = new QMenu(tr("标注"), this);
    QAction *clear_style_action = new QAction(tr("清除格式"), this);
    QAction *record_style_action = new QAction(tr("记录格式"), this);
    QAction *set_style_action = new QAction(tr("应用[记录格式]"), this);
    QMenu *set_Aa_action = new QMenu(tr("设置大小写"), this);
    QAction *set_Top_A_action = new QAction(tr("首字母大写"), this);
    QAction *set_A_action = new QAction(tr("大写"), this);
    QAction *set_a_action = new QAction(tr("小写"), this);
    QAction *format_set_font = new QAction(tr("设置字体"), this);
    QAction *format_set_font_color = new QAction(tr("设置字体颜色"), this);
    QAction *format_set_background_color = new QAction(tr("设置背景颜色"), this);
    QAction *set_font_I = new QAction(tr("斜体"), this);
    QAction *set_font_B = new QAction(tr("粗体"), this);
    QMenu *set_control_menu = new QMenu(tr("设置控件"), this);
    QAction *set_image_size_action = new QAction(tr("修改图片大小"), this);
    QMenu *set_table = new QMenu(tr("设置表格"), this);
    QAction *set_table_size_action = new QAction(tr("修改表格大小"), this);
    QAction *merge_cells_action = new QAction(tr("合并单元格"), this);
    QAction *split_cells_action = new QAction(tr("拆分单元格"), this);
    QMenu *insert_menu = new QMenu(tr("插入"), this);
    QAction *insert_image_action = new QAction(tr("图片"), this);
    QAction *insert_table_action = new QAction(tr("插入表格"), this);
    QMenu *extra_mode = new QMenu(tr("模式"), this);
    QAction *auto_turn_line_action = new QAction(tr("自动换行"), this);
    QAction *read_only_action = new QAction(tr("只读"), this);
    QAction *center_paste_action = new QAction(tr("鼠标中键粘贴"), this);
    QAction *window_control = new QAction(tr("窗口控制菜单"), this);
    QTextCharFormat basic_format;
    QImage save_image;
    WId winId;
    bool had_selected = false;
    char m_padding[7];
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void keyPressEvent(QKeyEvent *event);
    void self_contextMenuEvent(const QPoint &pos);
    void Add_Action(QMenu *menu);
    void Added_Action_Func(QAction *action, QPoint pos);
    void insertFromMimeData(const QMimeData *source);
    void insertImage(const QImage &image);
    void X11_Rasie();
    void self_copy();
    bool isSelectionImage();
    bool isSelectionTable();
    void first_cut();
    void second_cut();
    QTextCharFormat Basic_format_Set(QTextCharFormat format);
};
class My_LineEdit : public Basic_Widget
{
    Q_OBJECT
public:
    explicit My_LineEdit(QWidget *parent);
    ~My_LineEdit();
    void set_WinId(WId m_winId);
    QList<My_LineEdit *> *my_lineedit_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
    void update_style(QColor theme_background_color, QColor theme_text_color, QColor theme_color);
private:
    Basic_TextEdit *textEdit = new Basic_TextEdit(this->get_self());
private:
    void contextMenuEvent(QContextMenuEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
};
#endif // MY_LINEEDIT_H
