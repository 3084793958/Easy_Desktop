#ifndef MY_LINEEDIT_H
#define MY_LINEEDIT_H
#include "basic_widget.h"
#include <QTextEdit>
class Basic_TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit Basic_TextEdit(QWidget *parent);
    virtual void H_save(QSettings *settings);
    virtual void H_load(QSettings *settings);
    void set_icon(QString checked_icon_path);
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
    QAction *insert_mode_action = new QAction(tr("插入模式"), this);
    QAction *show_line_num_action = new QAction(tr("显示行数"), this);
    QAction *wheel_change_size_action = new QAction(tr("Ctrl+滚轮修改大小"), this);
    QAction *center_paste_action = new QAction(tr("鼠标中键粘贴"), this);
    QMenu *set_color_menu = new QMenu(tr("设置外观颜色"), this);
    QAction *set_selection_color = new QAction(tr("Alt列选择颜色"), this);
    QAction *set_search_color = new QAction(tr("查找与替换颜色"), this);
    QAction *search_for_text_action = new QAction(tr("查找与替换"), this);
    QAction *jump_to_line = new QAction(tr("跳到行"), this);
    QAction *window_control = new QAction(tr("窗口控制菜单"), this);
    QTextCharFormat basic_format;
    QImage save_image;
    QColor selection_color = QColor(0, 100, 255, 80);
    QColor search_color = QColor(255, 255, 0, 255);
    bool had_selected = false;
    void self_contextMenuEvent(const QPoint &pos);
    void Add_Action(QMenu *menu);
    void Added_Action_Func(QAction *action, QPoint pos);
    void insertFromMimeData(const QMimeData *source) override;
    void insertImage(const QImage &image);
    void self_copy();
    bool isSelectionImage();
    bool isSelectionTable();
    void first_cut();
    void second_cut();
    QTextCharFormat Basic_format_Set(QTextCharFormat format);
    void ZoomIn();
    void ZoomOut();
private:
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void inputMethodEvent(QInputMethodEvent *event) override;
private:
    void updateColumnSelection();
    void clearColumnSelection();
    void NormalSelection();
    char m_padding[7];
    QTextCursor Column_start_cursor;
    QTextCursor Column_end_cursor;
    QVector<QTextCursor> m_columnCursors;
    QList<QTextEdit::ExtraSelection> extraSelections_list;
private:
    class LineNumberArea : public QWidget
    {
    public:
        LineNumberArea(Basic_TextEdit *editor);
        QSize sizeHint() const override;
    protected:
        void paintEvent(QPaintEvent *event) override;
    private:
        Basic_TextEdit *textEdit;
    };
    friend class LineNumberArea;
private:
    void updateLineNumberAreaWidth();
    int lineNumberAreaWidth() const;
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    LineNumberArea *lineNumberArea = new LineNumberArea(this);
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};
class My_LineEdit : public Basic_Widget
{
    Q_OBJECT
public:
    explicit My_LineEdit(QWidget *parent);
    ~My_LineEdit();
    QList<My_LineEdit *> *my_lineedit_list;
    virtual void save(QSettings *settings);
    virtual void load(QSettings *settings);
    virtual void set_icon(QString checked_icon_path);
    void update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, QString checked_icon_path);
private:
    Basic_TextEdit *textEdit = new Basic_TextEdit(this->get_self());
private:
    void contextMenuEvent(QContextMenuEvent *event);
    virtual void wheelEvent(QWheelEvent *event);
};
#endif // MY_LINEEDIT_H
