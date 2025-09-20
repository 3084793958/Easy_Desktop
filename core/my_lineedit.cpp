#include "my_lineedit.h"
#include <QX11Info>
#include <X11/Xlib.h>
void Basic_TextEdit::X11_Rasie()
{
    Window win_Id = static_cast<Window>(winId);
    Display *display = QX11Info::display();
    XRaiseWindow(display, win_Id);
    XFlush(display);
}
#undef CursorShape
Basic_TextEdit::Basic_TextEdit(QWidget *parent)
    :QTextEdit(parent)
{
    setCursorWidth(2);
    setAcceptRichText(true);
    setTextColor(Qt::GlobalColor::black);
    setWordWrapMode(QTextOption::NoWrap);
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QTextEdit::customContextMenuRequested, this, &Basic_TextEdit::self_contextMenuEvent);
    this->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    basic_format.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    basic_format.setFontItalic(false);
    basic_format.setFontWeight(QFont::Normal);
    basic_format.setBackground(Qt::transparent);
    basic_format.setForeground(Qt::black);
    basic_format.setFontUnderline(false);
    extra_mode->addAction(auto_turn_line_action);
    auto_turn_line_action->setIconVisibleInMenu(false);
    auto_turn_line_action->setIcon(QIcon(":/base/this.svg"));
    extra_mode->addAction(read_only_action);
    read_only_action->setIconVisibleInMenu(false);
    read_only_action->setIcon(QIcon(":/base/this.svg"));
    extra_mode->addAction(center_paste_action);
    center_paste_action->setIconVisibleInMenu(true);
    center_paste_action->setIcon(QIcon(":/base/this.svg"));
    insert_menu->addAction(insert_image_action);
    insert_image_action->setIconVisibleInMenu(false);
    insert_image_action->setIcon(QIcon(":/base/this.svg"));
    insert_menu->addAction(insert_table_action);
    set_control_menu->addAction(set_image_size_action);
    set_image_size_action->setIconVisibleInMenu(false);
    set_image_size_action->setIcon(QIcon(":/base/this.svg"));
    set_control_menu->addMenu(set_table);
    set_table->addAction(set_table_size_action);
    set_table_size_action->setIconVisibleInMenu(false);
    set_table_size_action->setIcon(QIcon(":/base/this.svg"));
    set_table->addAction(merge_cells_action);
    set_table->addAction(split_cells_action);
    clear_style_action->setIconVisibleInMenu(false);
    clear_style_action->setIcon(QIcon(":/base/this.svg"));
    extra_menu->addAction(clear_style_action);
    extra_menu->addAction(record_style_action);
    extra_menu->addAction(set_style_action);
    set_Top_A_action->setIconVisibleInMenu(false);
    set_Top_A_action->setIcon(QIcon(":/base/this.svg"));
    set_Aa_action->addAction(set_Top_A_action);
    set_Aa_action->addAction(set_A_action);
    set_Aa_action->addAction(set_a_action);
    extra_menu->addMenu(set_Aa_action);
    extra_menu->addSeparator();
    extra_menu->addAction(format_set_font);
    extra_menu->addAction(format_set_font_color);
    extra_menu->addAction(format_set_background_color);
    set_font_B->setIconVisibleInMenu(false);
    set_font_B->setIcon(QIcon(":/base/this.svg"));
    set_font_I->setIconVisibleInMenu(false);
    set_font_I->setIcon(QIcon(":/base/this.svg"));
    extra_menu->addAction(set_font_I);
    extra_menu->addAction(set_font_B);
}
void Basic_TextEdit::mouseMoveEvent(QMouseEvent *event)
{
    if (this->verticalScrollBar()->geometry().contains(event->pos()))
    {
        setCursor(Qt::CursorShape::ArrowCursor);
    }
    if (this->horizontalScrollBar()->geometry().contains(event->pos()))
    {
        setCursor(Qt::CursorShape::ArrowCursor);
    }
    QTextEdit::mouseMoveEvent(event);
}
void Basic_TextEdit::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::MiddleButton)
    {
        return;
    }
    QTextEdit::mousePressEvent(event);
}
void Basic_TextEdit::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::MiddleButton)
    {
        if (center_paste_action->isIconVisibleInMenu())
        {
            this->paste();
        }
        return;
    }
    QTextEdit::mouseReleaseEvent(event);
}
void Basic_TextEdit::self_contextMenuEvent(const QPoint &pos)
{
    QMenu *menu = this->createStandardContextMenu();
    Basic_TextEdit::Add_Action(menu);
    QAction *res_action = menu->exec(this->viewport()->mapToGlobal(pos));
    Basic_TextEdit::Added_Action_Func(res_action, this->viewport()->mapToGlobal(pos));
    menu->deleteLater();
}
void Basic_TextEdit::Add_Action(QMenu *menu)
{
    menu->insertMenu(menu->actions()[0], extra_menu);
    menu->insertMenu(menu->actions()[1], set_control_menu);
    menu->insertMenu(menu->actions()[2], insert_menu);
    menu->insertMenu(menu->actions()[3], extra_mode);
    menu->insertSeparator(menu->actions()[4]);
    menu->addSeparator();
    menu->addAction(window_control);
    extra_menu->setEnabled(textCursor().hasSelection() && !isReadOnly());
    set_image_size_action->setEnabled(isSelectionImage());
    set_table->setEnabled(isSelectionTable());
    set_control_menu->setEnabled(set_image_size_action->isEnabled() || set_table->isEnabled());
    if (QApplication::clipboard()->mimeData()->hasImage())
    {
        for (int i = 0; i < menu->actions().size(); i++)
        {
            if (menu->actions()[i]->objectName() == "edit-paste")
            {
                menu->actions()[i]->setEnabled(true);
                break;
            }
        }
    }
    if (isSelectionImage())
    {
        had_selected = true;
        Basic_TextEdit::first_cut();
    }
    else
    {
        had_selected = false;
    }
    if (extra_menu->isEnabled())
    {
        QTextCursor help_cursor = QTextCursor(this->textCursor());
        help_cursor.setPosition(textCursor().selectionStart() + 1);
        QTextCharFormat fmt = help_cursor.charFormat();
        fmt = Basic_format_Set(fmt);
        set_font_B->setIconVisibleInMenu(fmt.fontWeight() == QFont::Bold);
        set_font_I->setIconVisibleInMenu(fmt.fontItalic());
    }
}
void Basic_TextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->matches(QKeySequence::Copy))
    {
        if (isSelectionImage())
        {
            Basic_TextEdit::self_copy();
            event->accept();
            return;
        }
    }
    else if (event->matches(QKeySequence::Cut))
    {
        if (isSelectionImage())
        {
            Basic_TextEdit::first_cut();
            this->cut();
            Basic_TextEdit::second_cut();
            return;
        }
    }
    QTextEdit::keyPressEvent(event);
}
bool Basic_TextEdit::isSelectionImage()
{
    if (!this->textCursor().hasSelection())
    {
        return false;
    }
    QString selectedText = this->textCursor().selectedText();
    for (int i = 0; i < selectedText.length(); i++)
    {
        if (selectedText.at(i) != QChar::ObjectReplacementCharacter && selectedText.at(i) != "\u2029")
        {
            return false;
        }
    }
    QTextCharFormat charFormat = this->textCursor().charFormat();
    return charFormat.isImageFormat();
}
bool Basic_TextEdit::isSelectionTable()
{
    return this->textCursor().currentTable() != nullptr;
}
void Basic_TextEdit::Added_Action_Func(QAction *action, QPoint pos)
{
    if (action == nullptr)
    {
        return;
    }
    if (action == auto_turn_line_action)
    {
        auto_turn_line_action->setIconVisibleInMenu(!auto_turn_line_action->isIconVisibleInMenu());
        setWordWrapMode(auto_turn_line_action->isIconVisibleInMenu()?QTextOption::WrapMode::WrapAnywhere:QTextOption::WrapMode::NoWrap);
    }
    else if (action == read_only_action)
    {
        read_only_action->setIconVisibleInMenu(!read_only_action->isIconVisibleInMenu());
        setReadOnly(read_only_action->isIconVisibleInMenu());
    }
    else if (action == center_paste_action)
    {
        center_paste_action->setIconVisibleInMenu(!center_paste_action->isIconVisibleInMenu());
    }
    else if (action == window_control)
    {
        emit window_contextmenu(pos);
    }
    else if (action != nullptr && action->objectName() == "edit-copy")
    {
        if (isSelectionImage())
        {
            Basic_TextEdit::self_copy();
        }
    }
    else if (action != nullptr && action->objectName() == "edit-cut")
    {
        if (had_selected)
        {
            Basic_TextEdit::second_cut();
        }
    }
    else if (action == insert_image_action)
    {
        QList<QString> urls = QFileDialog::getOpenFileNames(nullptr, "获取图像", QDir::homePath(), "图像文件(*.png *.jpg *.jpeg *.svg *.gif *.bmp);;所有文件(*.*)");
        Basic_TextEdit::X11_Rasie();//没办法,要跟dde-desktop争夺[桌面显示权]
        if (urls.isEmpty())
        {
            return;
        }
        for (int i = 0; i < urls.size(); i++)
        {
            if (urls[i].isEmpty())
            {
                continue;
            }
            QImage image;
            bool loaded = image.load(urls[i]);
            if (!loaded)
            {
                continue;
            }
            insertImage(image);
        }
    }
    else if (action == insert_table_action)
    {
        bool ok = false;
        int rows = QInputDialog::getInt(nullptr, "获取数值", "行数", 1, 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int cols = QInputDialog::getInt(nullptr, "获取数值", "列数", 1, 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        QTextTableFormat tableFormat;
        tableFormat.setBorderStyle(QTextTableFormat::BorderStyle::BorderStyle_Double);
        this->textCursor().insertTable(rows, cols, tableFormat);
    }
    else if (action == set_image_size_action)
    {
        QTextImageFormat imageFormat = this->textCursor().charFormat().toImageFormat();
        if (imageFormat.width() <= 1 || imageFormat.height() <= 1)
        {
            if (imageFormat.isValid())
            {
                QVariant imageData = document()->resource(QTextDocument::ImageResource, QUrl(imageFormat.name()));
                if (imageData.isValid() && imageData.canConvert<QImage>())
                {
                    QImage image = imageData.value<QImage>();
                    imageFormat.setWidth(image.width());
                    imageFormat.setHeight(image.height());
                }
            }
        }
        bool ok = false;
        qreal new_width = QInputDialog::getDouble(nullptr, "获取数值", "宽度", imageFormat.width(), 0.00, 2147483647.00, 1, &ok);
        if (!ok)
        {
            return;
        }
        qreal new_height = QInputDialog::getDouble(nullptr, "获取数值", "高度", imageFormat.height(), 0.00, 2147483647.00, 1, &ok);
        if (!ok)
        {
            return;
        }
        imageFormat.setWidth(new_width);
        imageFormat.setHeight(new_height);
        this->textCursor().mergeCharFormat(imageFormat);
    }
    else if (action == set_table_size_action)
    {
        QTextTable *table = this->textCursor().currentTable();
        if (table == nullptr)
        {
            return;
        }
        bool ok = false;
        int rows = QInputDialog::getInt(nullptr, "获取数值", "行数", table->rows(), 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int cols = QInputDialog::getInt(nullptr, "获取数值", "列数", table->columns(), 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        table->resize(rows, cols);
    }
    else if (action == merge_cells_action)
    {
        QTextTable *table = this->textCursor().currentTable();
        if (table == nullptr)
        {
            return;
        }
        if (this->textCursor().hasSelection())
        {
            table->mergeCells(this->textCursor());
        }
        else
        {
            bool ok = false;
            int rows = QInputDialog::getInt(nullptr, "获取数值", "向下合并[行数]", 1, 0, 2147483647, 1, &ok);
            if (!ok)
            {
                return;
            }
            ok = false;
            int cols = QInputDialog::getInt(nullptr, "获取数值", "向右合并[列数]", 1, 0, 2147483647, 1, &ok);
            if (!ok)
            {
                return;
            }
            rows++;
            cols++;
            QTextTableCell cell = table->cellAt(this->textCursor());
            table->mergeCells(cell.row(), cell.column(), rows, cols);
        }
    }
    else if (action == split_cells_action)
    {
        QTextTable *table = this->textCursor().currentTable();
        if (table == nullptr)
        {
            return;
        }
        bool ok = false;
        int rows = QInputDialog::getInt(nullptr, "获取数值", "拆分[行数]维度\n得到单元格所占行数", 1, 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int cols = QInputDialog::getInt(nullptr, "获取数值", "拆分为[列数]维度\n得到单元格所占行数", 1, 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        QTextTableCell cell = table->cellAt(this->textCursor());
        table->splitCell(cell.row(), cell.column(), rows, cols);
    }
    else if (action == clear_style_action)
    {
        if (textCursor().hasSelection())
        {
            QTextCharFormat fmt;
            fmt.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
            fmt.setFontItalic(false);
            fmt.setFontWeight(QFont::Normal);
            fmt.setBackground(Qt::transparent);
            fmt.setForeground(Qt::black);
            fmt.setFontUnderline(false);
            this->textCursor().mergeCharFormat(fmt);
        }
    }
    else if (action == record_style_action)
    {
        if (textCursor().hasSelection())
        {
            QTextCursor help_cursor = QTextCursor(this->textCursor());
            help_cursor.setPosition(textCursor().selectionStart() + 1);
            QTextCharFormat fmt = help_cursor.charFormat();
            if (fmt.isValid())
            {
                basic_format = Basic_format_Set(fmt);
            }
        }
    }
    else if (action == set_style_action)
    {
        if (textCursor().hasSelection())
        {
            this->textCursor().mergeCharFormat(basic_format);
        }
    }
    else if (action == set_Top_A_action)
    {
        if (textCursor().hasSelection())
        {
            int start = this->textCursor().selectionStart();
            int end = this->textCursor().selectionEnd();
            this->textCursor().beginEditBlock();
            QTextCursor running_cursor = QTextCursor(this->document());
            QString ins_text;
            QTextCharFormat ins_Format;
            bool first = true;
            for (int i = start; i < end; i++)
            {
                running_cursor.setPosition(i);
                running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
                ins_text = running_cursor.selectedText();
                if (ins_text == " " || ins_text.at(0) == "\u2029")
                {
                    first = true;
                }
                else
                {
                    if (first)
                    {
                        first = false;
                        ins_text = ins_text.toUpper();
                    }
                    else
                    {
                        ins_text = ins_text.toLower();
                    }
                }
                ins_Format = running_cursor.charFormat();
                running_cursor.insertText(ins_text, ins_Format);
            }
            this->textCursor().endEditBlock();
        }
    }
    else if (action == set_A_action)
    {
        if (textCursor().hasSelection())
        {
            int start = this->textCursor().selectionStart();
            int end = this->textCursor().selectionEnd();
            this->textCursor().beginEditBlock();
            QTextCursor running_cursor = QTextCursor(this->document());
            QString ins_text;
            QTextCharFormat ins_Format;
            for (int i = start; i < end; i++)
            {
                running_cursor.setPosition(i);
                running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
                ins_text = running_cursor.selectedText().toUpper();
                ins_Format = running_cursor.charFormat();
                running_cursor.insertText(ins_text, ins_Format);
            }
            this->textCursor().endEditBlock();
        }
    }
    else if (action == set_a_action)
    {
        if (textCursor().hasSelection())
        {
            int start = this->textCursor().selectionStart();
            int end = this->textCursor().selectionEnd();
            this->textCursor().beginEditBlock();
            QTextCursor running_cursor = QTextCursor(this->document());
            QString ins_text;
            QTextCharFormat ins_Format;
            for (int i = start; i < end; i++)
            {
                running_cursor.setPosition(i);
                running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
                ins_text = running_cursor.selectedText().toLower();
                ins_Format = running_cursor.charFormat();
                running_cursor.insertText(ins_text, ins_Format);
            }
            this->textCursor().endEditBlock();
        }
    }
    else if (action == format_set_font)
    {
        QTextCursor help_cursor = QTextCursor(this->textCursor());
        help_cursor.setPosition(textCursor().selectionStart() + 1);
        QTextCharFormat fmt = help_cursor.charFormat();
        fmt = Basic_format_Set(fmt);
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, fmt.font(), nullptr);
        if (!ok)
        {
            return;
        }
        int start = this->textCursor().selectionStart();
        int end = this->textCursor().selectionEnd();
        this->textCursor().beginEditBlock();
        QTextCursor running_cursor = QTextCursor(this->document());
        QTextCharFormat ins_Format;
        for (int i = start; i < end; i++)
        {
            running_cursor.setPosition(i);
            running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
            ins_Format = running_cursor.charFormat();
            ins_Format.setFont(font);
            running_cursor.mergeCharFormat(ins_Format);
        }
        this->textCursor().endEditBlock();
    }
    else if (action == format_set_font_color)
    {
        QTextCursor help_cursor = QTextCursor(this->textCursor());
        help_cursor.setPosition(textCursor().selectionStart() + 1);
        QTextCharFormat fmt = help_cursor.charFormat();
        fmt = Basic_format_Set(fmt);
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(fmt.foreground().color());
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle("获取颜色");
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        int start = this->textCursor().selectionStart();
        int end = this->textCursor().selectionEnd();
        this->textCursor().beginEditBlock();
        QTextCursor running_cursor = QTextCursor(this->document());
        QTextCharFormat ins_Format;
        for (int i = start; i < end; i++)
        {
            running_cursor.setPosition(i);
            running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
            ins_Format = running_cursor.charFormat();
            ins_Format.setForeground(colorDialog.currentColor());
            running_cursor.mergeCharFormat(ins_Format);
        }
        this->textCursor().endEditBlock();
    }
    else if (action == format_set_background_color)
    {
        QTextCursor help_cursor = QTextCursor(this->textCursor());
        help_cursor.setPosition(textCursor().selectionStart() + 1);
        QTextCharFormat fmt = help_cursor.charFormat();
        fmt = Basic_format_Set(fmt);
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(fmt.background().color());
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle("获取颜色");
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        int start = this->textCursor().selectionStart();
        int end = this->textCursor().selectionEnd();
        this->textCursor().beginEditBlock();
        QTextCursor running_cursor = QTextCursor(this->document());
        QTextCharFormat ins_Format;
        for (int i = start; i < end; i++)
        {
            running_cursor.setPosition(i);
            running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
            ins_Format = running_cursor.charFormat();
            ins_Format.setBackground(colorDialog.currentColor());
            running_cursor.mergeCharFormat(ins_Format);
        }
        this->textCursor().endEditBlock();
    }
    else if (action == set_font_I)
    {
        int start = this->textCursor().selectionStart();
        int end = this->textCursor().selectionEnd();
        this->textCursor().beginEditBlock();
        QTextCursor running_cursor = QTextCursor(this->document());
        QTextCharFormat ins_Format;
        for (int i = start; i < end; i++)
        {
            running_cursor.setPosition(i);
            running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
            ins_Format = running_cursor.charFormat();
            ins_Format.setFontItalic(!set_font_I->isIconVisibleInMenu());
            running_cursor.mergeCharFormat(ins_Format);
        }
        this->textCursor().endEditBlock();
    }
    else if (action == set_font_B)
    {
        int start = this->textCursor().selectionStart();
        int end = this->textCursor().selectionEnd();
        this->textCursor().beginEditBlock();
        QTextCursor running_cursor = QTextCursor(this->document());
        QTextCharFormat ins_Format;
        for (int i = start; i < end; i++)
        {
            running_cursor.setPosition(i);
            running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
            ins_Format = running_cursor.charFormat();
            ins_Format.setFontWeight(set_font_B->isIconVisibleInMenu()?QFont::Normal:QFont::Bold);
            running_cursor.mergeCharFormat(ins_Format);
        }
        this->textCursor().endEditBlock();
    }
}
QTextCharFormat Basic_TextEdit::Basic_format_Set(QTextCharFormat format)
{
    QTextCharFormat fmt;
    fmt.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    fmt.setFontItalic(false);
    fmt.setFontWeight(QFont::Normal);
    fmt.setBackground(Qt::transparent);
    fmt.setForeground(Qt::black);
    fmt.setFontUnderline(false);
    fmt.setFont(format.font());
    fmt.setFontItalic(format.fontItalic());
    fmt.setFontWeight(format.fontWeight());
    fmt.setBackground(format.background());
    fmt.setForeground(format.foreground());
    fmt.setFontUnderline(format.fontUnderline());
    return fmt;
}
void Basic_TextEdit::self_copy()
{
    QTextImageFormat imageFormat = this->textCursor().charFormat().toImageFormat();
    if (imageFormat.isValid())
    {
        QVariant imageData = document()->resource(QTextDocument::ImageResource, QUrl(imageFormat.name()));
        if (imageData.isValid() && imageData.canConvert<QImage>())
        {
            QImage image = imageData.value<QImage>();
            QApplication::clipboard()->setImage(image.scaled(int(imageFormat.width()), int(imageFormat.height())));
        }
    }
}
void Basic_TextEdit::first_cut()
{
    QTextImageFormat imageFormat = this->textCursor().charFormat().toImageFormat();
    if (imageFormat.isValid())
    {
        QVariant imageData = document()->resource(QTextDocument::ImageResource, QUrl(imageFormat.name()));
        if (imageData.isValid() && imageData.canConvert<QImage>())
        {
            QImage image = imageData.value<QImage>();
            save_image = image.scaled(int(imageFormat.width()), int(imageFormat.height()));
        }
    }
}
void Basic_TextEdit::second_cut()
{
    QApplication::clipboard()->setImage(save_image);
}
void Basic_TextEdit::insertFromMimeData(const QMimeData *source)
{
    if (source->hasImage())
    {
        QImage image = qvariant_cast<QImage>(source->imageData());
        Basic_TextEdit::insertImage(image);
        return;
    }
    QTextEdit::insertFromMimeData(source);
}
void Basic_TextEdit::insertImage(const QImage &image)
{
    QTextCursor cursor = textCursor();
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QString imageData = QString("data:image/png;base64," + ba.toBase64());
    cursor.insertHtml(QString("<img src=\"%1\" style=\"background-color:transparent;\">").arg(imageData));
}
void Basic_TextEdit::set_WinId(WId m_winId)
{
    winId = m_winId;
}
My_LineEdit::My_LineEdit(QWidget *parent)
    :Basic_Widget(parent)
{
    textEdit->setStyleSheet("QWidget{background:rgba(0,0,0,0);color:rgba(0,0,0,255)}"
                            "QMenu{border-radius:10px 10px;background:rgba(255,255,255,75);margin:0px -1px 0px -1px;padding-top:8px;padding-bottom:8px;icon-size:20px;border-radius:10px 10px}"
                            "QMenu::item{color:rgba(0,0,0,255)}"
                            "QMenu::item:disabled{color:rgba(131,136,139,255)}"
                            "QMenu::item:selected{color:rgba(255,255,255,255);background:rgba(0,129,255,255)}"
                            "QMenu::separator{background:rgba(150,150,150,125)}");
    textEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{border:none;background:rgba(0,0,0,0);width:8px;margin:0px0px0px0px;}"
                                                 "QScrollBar::handle:vertical{background:rgba(0,0,0,75);border-radius:4px;min-height:20px;}"
                                                 "QScrollBar::handle:vertical:hover{background:rgba(0,0,0,125);}"
                                                 "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0px;}"
                                                 "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}");
    textEdit->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{border:none;background:rgba(0,0,0,0);height:8px;margin:0px0px0px0px;}"
                                                   "QScrollBar::handle:horizontal{background:rgba(0,0,0,75);border-radius:4px;min-width:20px;}"
                                                   "QScrollBar::handle:horizontal:hover{background:rgba(0,0,0,125);}"
                                                   "QScrollBar::add-line:horizontal,QScrollBar::sub-line:horizontal{width:0px;}"
                                                   "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:none;}");
    connect(this, &Basic_Widget::size_changed, this, [=](QSize size)
    {
        textEdit->move(5, 20);
        textEdit->resize(size - QSize(10, 25));
    });
    connect(textEdit, &Basic_TextEdit::window_contextmenu, this, [=](QPoint pos)
    {
        QAction *know_what = basic_control->exec(pos);
        if (know_what == move_to_page_action || know_what == set_background_radius || know_what == set_background_color || know_what == show_close_button)
        {
            Basic_Widget::basic_action_func(know_what);
        }
    });
    textEdit->setMouseTracking(true);
    resize(400, 250);
    show();
}

My_LineEdit::~My_LineEdit()
{
    if (my_lineedit_list)
    {
        my_lineedit_list->removeOne(this);
    }
}
void My_LineEdit::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = basic_control->exec(mapToGlobal(event->pos()));
    if (know_what != nullptr)
    {
        Basic_Widget::basic_action_func(know_what);
    }
}
void My_LineEdit::wheelEvent(QWheelEvent *event)
{
    if (!textEdit->toPlainText().isEmpty())
    {
        event->accept();
        return;
    }
}
void My_LineEdit::set_WinId(WId m_winId)
{
    this->textEdit->set_WinId(m_winId);
}
void My_LineEdit::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    this->textEdit->H_save(settings);
}
void My_LineEdit::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    this->textEdit->H_load(settings);
}
void Basic_TextEdit::H_save(QSettings *settings)
{
    settings->setValue("auto_turn_line", auto_turn_line_action->isIconVisibleInMenu());
    settings->setValue("read_only", read_only_action->isIconVisibleInMenu());
    settings->setValue("center_paste", center_paste_action->isIconVisibleInMenu());
    settings->setValue("H_SValue", this->horizontalScrollBar()->value());
    settings->setValue("V_SValue", this->verticalScrollBar()->value());
    settings->setValue("html_text", this->toHtml());
}
void Basic_TextEdit::H_load(QSettings *settings)
{
    bool auto_turn_line = settings->value("auto_turn_line", false).toBool();
    auto_turn_line_action->setIconVisibleInMenu(auto_turn_line);
    setWordWrapMode(auto_turn_line_action->isIconVisibleInMenu()?QTextOption::WrapMode::WrapAnywhere:QTextOption::WrapMode::NoWrap);
    bool read_only = settings->value("read_only", false).toBool();
    read_only_action->setIconVisibleInMenu(read_only);
    setReadOnly(read_only_action->isIconVisibleInMenu());
    bool center_paste = settings->value("center_paste", true).toBool();
    center_paste_action->setIconVisibleInMenu(center_paste);
    this->setHtml(settings->value("html_text", "").toString());
    this->horizontalScrollBar()->setValue(settings->value("H_SValue", 0).toInt());
    this->verticalScrollBar()->setValue(settings->value("V_SValue", 0).toInt());
}
