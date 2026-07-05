#ifndef MY_LINEEDIT_H
#define MY_LINEEDIT_H
#include "basic_widget.h"
#include <QTextEdit>
#include <QStatusBar>
class Basic_TextEdit;
class SearchBar : public QWidget
{
    Q_OBJECT
public:
    explicit SearchBar(Basic_TextEdit *editor, QWidget *parent = nullptr);
    void showSearchBar();
    void hideSearchBar();
    QString findText() const;
    QString replaceText() const;
    void setfindText(QString &text);
signals:
    void findNext();
    void findPrevious();
    void replaceCurrent();
    void replaceAll();
    void visibilityChanged(bool visible);
private slots:
    void onTextChanged(const QString &text);
    void onCaseSensitiveToggled(bool checked);
    void onWholeWordToggled(bool checked);
protected:
    void keyPressEvent(QKeyEvent *event) override;
private:
    Basic_TextEdit *m_editor;
    QLineEdit *m_findEdit = new QLineEdit(this);
    QLineEdit *m_replaceEdit = new QLineEdit(this);
    QPushButton *m_prevBtn = new Trans_PushButton(tr("上一项"), "上一项", this->metaObject()->className(), this);
    QPushButton *m_nextBtn = new Trans_PushButton(tr("下一项"), "下一项", this->metaObject()->className(), this);
    QPushButton *m_replaceBtn = new Trans_PushButton(tr("替换"), "替换", this->metaObject()->className(), this);
    QPushButton *m_replaceAllBtn = new Trans_PushButton(tr("全部替换"), "全部替换", this->metaObject()->className(), this);
    QPushButton *m_closeBtn = new Trans_PushButton(tr("×"), "×", this->metaObject()->className(), this);
    QCheckBox *m_caseSensitive = new Trans_CheckBox(tr("区分大小写"), "区分大小写", this->metaObject()->className(), this);
    QCheckBox *m_wholeWord = new Trans_CheckBox(tr("全词匹配"), "全词匹配", this->metaObject()->className(), this);
};
class Basic_TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    explicit Basic_TextEdit(QWidget *parent);
    virtual void H_save(QSettings *settings, QString Token);
    virtual void H_load(QSettings *settings, QString Token);
    void H_save_no_text(QSettings *settings, QString Token);
    void H_load_no_text(QSettings *settings, QString Token);
    void set_icon(QString checked_icon_path);
signals:
    void window_contextmenu(QPoint pos);
private:
    QMenu *extra_menu = new Trans_Menu(tr("标注"), "标注", this->metaObject()->className(), this);
    QAction *clear_style_action = new Trans_Action(tr("清除格式"), "清除格式", this->metaObject()->className(), this);
    QAction *record_style_action = new Trans_Action(tr("记录格式"), "记录格式", this->metaObject()->className(), this);
    QAction *set_style_action = new Trans_Action(tr("应用[记录格式]"), "应用[记录格式]", this->metaObject()->className(), this);
    QMenu *set_Aa_action = new Trans_Menu(tr("设置大小写"), "设置大小写", this->metaObject()->className(), this);
    QAction *set_Top_A_action = new Trans_Action(tr("首字母大写"), "首字母大写", this->metaObject()->className(), this);
    QAction *set_A_action = new Trans_Action(tr("大写"), "大写", this->metaObject()->className(), this);
    QAction *set_a_action = new Trans_Action(tr("小写"), "小写", this->metaObject()->className(), this);
    QAction *format_set_font = new Trans_Action(tr("设置字体"), "设置字体", this->metaObject()->className(), this);
    QAction *format_set_font_color = new Trans_Action(tr("设置字体颜色"), "设置字体颜色", this->metaObject()->className(), this);
    QAction *format_set_background_color = new Trans_Action(tr("设置背景颜色"), "设置背景颜色", this->metaObject()->className(), this);
    QAction *set_font_I = new Trans_Action(tr("斜体"), "斜体", this->metaObject()->className(), this);
    QAction *set_font_B = new Trans_Action(tr("粗体"), "粗体", this->metaObject()->className(), this);

    QMenu *set_control_menu = new Trans_Menu(tr("设置控件"), "设置控件", this->metaObject()->className(), this);
    QAction *set_image_size_action = new Trans_Action(tr("修改图片大小"), "修改图片大小", this->metaObject()->className(), this);
    QMenu *set_table = new Trans_Menu(tr("设置表格"), "设置表格", this->metaObject()->className(), this);
    QAction *set_table_size_action = new Trans_Action(tr("修改表格大小"), "修改表格大小", this->metaObject()->className(), this);
    QAction *merge_cells_action = new Trans_Action(tr("合并单元格"), "合并单元格", this->metaObject()->className(), this);
    QAction *split_cells_action = new Trans_Action(tr("拆分单元格"), "拆分单元格", this->metaObject()->className(), this);

    QMenu *insert_menu = new Trans_Menu(tr("插入"), "插入", this->metaObject()->className(), this);
    QAction *insert_image_action = new Trans_Action(tr("图片"), "图片", this->metaObject()->className(), this);
    QAction *insert_table_action = new Trans_Action(tr("插入表格"), "插入表格", this->metaObject()->className(), this);

    QMenu *extra_mode = new Trans_Menu(tr("模式"), "模式", this->metaObject()->className(), this);
    QAction *auto_turn_line_action = new Trans_Action(tr("自动换行"), "自动换行", this->metaObject()->className(), this);
    QAction *read_only_action = new Trans_Action(tr("只读"), "只读", this->metaObject()->className(), this);
    QAction *insert_mode_action = new Trans_Action(tr("插入模式"), "插入模式", this->metaObject()->className(), this);
    QAction *wheel_change_size_action = new Trans_Action(tr("Ctrl+滚轮修改大小"), "Ctrl+滚轮修改大小", this->metaObject()->className(), this);
    QAction *center_paste_action = new Trans_Action(tr("鼠标中键粘贴"), "鼠标中键粘贴", this->metaObject()->className(), this);

    QMenu *set_apperance_menu = new Trans_Menu(tr("设置外观"), "设置外观", this->metaObject()->className(), this);
    QAction *show_line_num_action = new Trans_Action(tr("显示行数"), "显示行数", this->metaObject()->className(), this);
    QMenu *set_show_line_pos_menu = new Trans_Menu(tr("设置行数区位置"), "设置行数区位置", this->metaObject()->className(), this);
    QAction *set_show_line_pos_left_action = new Trans_Action(tr("左"), "左", this->metaObject()->className(), this);
    QAction *set_show_line_pos_right_action = new Trans_Action(tr("右"), "右", this->metaObject()->className(), this);
    QAction *set_show_line_text_color_action = new Trans_Action(tr("设置行数区文字颜色"), "设置行数区文字颜色", this->metaObject()->className(), this);
    QAction *set_show_line_mark_color_action = new Trans_Action(tr("设置行标注颜色"), "设置行标注颜色", this->metaObject()->className(), this);
    QAction *set_show_status_bar = new Trans_Action(tr("显示状态栏"), "显示状态栏", this->metaObject()->className(), this);
    QAction *set_show_status_bar_text_color = new Trans_Action(tr("设置状态栏字体颜色"), "设置状态栏字体颜色", this->metaObject()->className(), this);
    QAction *set_selection_color = new Trans_Action(tr("设置Alt列选择颜色"), "设置Alt列选择颜色", this->metaObject()->className(), this);
    QAction *set_search_color = new Trans_Action(tr("设置查找与替换颜色[非选中]"), "设置查找与替换颜色[非选中]", this->metaObject()->className(), this);
    QAction *set_search_select_color = new Trans_Action(tr("设置查找与替换颜色[选中]"), "设置查找与替换颜色[选中]", this->metaObject()->className(), this);

    QAction *search_for_text_action = new Trans_Action(tr("查找与替换"), "查找与替换", this->metaObject()->className(), this);
    QAction *jump_to_line = new Trans_Action(tr("跳到行"), "跳到行", this->metaObject()->className(), this);
    QAction *window_control = new Trans_Action(tr("窗口控制菜单"), "窗口控制菜单", this->metaObject()->className(), this);
    QTextCharFormat basic_format;
    QImage save_image;
    QColor selection_color = QColor(0, 100, 255, 80);
    QColor line_mark_color = QColor(255, 0, 0, 255);
    QColor line_text_color = QColor(30, 30, 30, 255);
    bool lineArea_left = true;
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
    bool m_caseSensitive = false;
    bool m_wholeWord = false;
    int m_currentMatchIndex = -1;
    SearchBar *m_searchBar = new SearchBar(this, this);
    QList<QTextEdit::ExtraSelection> m_searchHighlights;
    QList<QTextCursor> m_matchCursors;
    void updateSearchHighlights();
    void mergeAndSetExtraSelections();
    QColor search_color = QColor(255, 255, 0, 255);
    QColor search_and_select_color = QColor(0, 129, 255, 255);
public:
    void performSearch(bool moveToFirst = true);
    void goToNextMatch();
    void goToPrevMatch();
    void replaceCurrentMatch();
    void replaceAllMatches();
    void setSearchOptions(bool caseSensitive, bool wholeWord);
    QString getSearchText() const;
    QString getReplaceText() const;
    void setSearchBarVisible(bool visible);
private:
    void updateColumnSelection();
    void clearColumnSelection();
    void NormalSelection();
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
        virtual void mouseReleaseEvent(QMouseEvent *event) override;
    private:
        Basic_TextEdit *textEdit;
    };
    friend class LineNumberArea;
public:
    void updateLineNumberAreaWidth();
private:
    int lineNumberAreaWidth() const;
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    LineNumberArea *lineNumberArea = new LineNumberArea(this);
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
private:
    QSet<int> markedLines;
private:
    QStatusBar *m_statusBar = new QStatusBar(this);
    QLabel *statusLabel = new QLabel(this);
    QColor statusBar_text_color = QColor(30, 30, 30, 255);
public:
    void updateStatusBar_style();
    void updateStatusBar();
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
