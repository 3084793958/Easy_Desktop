#include "my_lineedit.h"
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
    extra_mode->addAction(wheel_change_size_action);
    wheel_change_size_action->setIconVisibleInMenu(true);
    wheel_change_size_action->setIcon(QIcon(":/base/this.svg"));
    extra_mode->addAction(insert_mode_action);
    insert_mode_action->setIconVisibleInMenu(false);
    insert_mode_action->setIcon(QIcon(":/base/this.svg"));
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
    set_apperance_menu->addAction(show_line_num_action);
    show_line_num_action->setIconVisibleInMenu(false);
    show_line_num_action->setIcon(QIcon(":/base/this.svg"));
    set_show_line_pos_menu->addAction(set_show_line_pos_left_action);
    set_show_line_pos_left_action->setIconVisibleInMenu(true);
    set_show_line_pos_left_action->setIcon(QIcon(":/base/this.svg"));
    set_show_line_pos_menu->addAction(set_show_line_pos_right_action);
    set_show_line_pos_right_action->setIconVisibleInMenu(false);
    set_show_line_pos_right_action->setIcon(QIcon(":/base/this.svg"));
    set_apperance_menu->addMenu(set_show_line_pos_menu);
    set_apperance_menu->addAction(set_show_line_text_color_action);
    set_apperance_menu->addAction(set_show_line_mark_color_action);
    set_apperance_menu->addSeparator();
    set_apperance_menu->addAction(set_show_status_bar);
    set_show_status_bar->setIconVisibleInMenu(false);
    set_show_status_bar->setIcon(QIcon(":/base/this.svg"));
    set_apperance_menu->addAction(set_show_status_bar_text_color);
    set_apperance_menu->addSeparator();
    set_apperance_menu->addAction(set_selection_color);
    set_apperance_menu->addAction(set_search_color);
    set_apperance_menu->addAction(set_search_select_color);
    lineNumberArea->setVisible(false);
    m_statusBar->setFixedHeight(24);
    m_statusBar->hide();
    updateStatusBar_style();
    m_statusBar->setSizeGripEnabled(false);
    statusLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_statusBar->addPermanentWidget(statusLabel, 1);
    updateStatusBar();
    connect(this, &QTextEdit::textChanged, this, &Basic_TextEdit::updateLineNumberAreaWidth);
    connect(document(), &QTextDocument::blockCountChanged, this, &Basic_TextEdit::updateLineNumberAreaWidth);
    connect(verticalScrollBar(), &QScrollBar::valueChanged, this, [=]
    {
        if (lineNumberArea->isVisible())
        {
            lineNumberArea->update();
        }
    });
    connect(this, &QTextEdit::cursorPositionChanged, this, &Basic_TextEdit::updateStatusBar);
    connect(this, &QTextEdit::selectionChanged, this, &Basic_TextEdit::updateStatusBar);
    connect(this, &QTextEdit::textChanged, this, &Basic_TextEdit::updateStatusBar);
    updateLineNumberAreaWidth();
    m_searchBar->hide();
    connect(m_searchBar, &SearchBar::findNext, this, &Basic_TextEdit::goToNextMatch);
    connect(m_searchBar, &SearchBar::findPrevious, this, &Basic_TextEdit::goToPrevMatch);
    connect(m_searchBar, &SearchBar::replaceCurrent, this, &Basic_TextEdit::replaceCurrentMatch);
    connect(m_searchBar, &SearchBar::replaceAll, this, &Basic_TextEdit::replaceAllMatches);
    connect(m_searchBar, &SearchBar::visibilityChanged, this, &Basic_TextEdit::setSearchBarVisible);
}
int Basic_TextEdit::lineNumberAreaWidth() const
{
    if (!show_line_num_action->isIconVisibleInMenu())
    {
        return 0;
    }
    int digits = 1;
    int maxLines = document()->blockCount();
    while (maxLines >= 10)
    {
        maxLines /= 10;
        ++digits;
    }
    return 8 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits + 16;
}
void Basic_TextEdit::updateLineNumberAreaWidth()
{
    if (!set_show_status_bar->isIconVisibleInMenu())
    {
        m_statusBar->hide();
    }
    else
    {
        m_statusBar->show();
    }
    if (!show_line_num_action->isIconVisibleInMenu())
    {
        setViewportMargins(0, 0, 0, m_statusBar->height() * m_statusBar->isVisible());
        lineNumberArea->setVisible(false);
        return;
    }
    int space = lineNumberAreaWidth();
    if (lineArea_left)
    {
        setViewportMargins(space, 0, 0, m_statusBar->height() * m_statusBar->isVisible());
    }
    else
    {
        setViewportMargins(0, 0, space, m_statusBar->height() * m_statusBar->isVisible());
    }
    lineNumberArea->setVisible(true);
    if (lineArea_left)
    {
        lineNumberArea->setGeometry(QRect(0, 0, space, height() - (m_statusBar->height() + 5) * m_statusBar->isVisible()));
    }
    else
    {
        lineNumberArea->setGeometry(QRect(this->width() - space, 0, space, height() - (m_statusBar->height() + 5) * m_statusBar->isVisible()));
    }
}
void Basic_TextEdit::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    (void) event;
    if (!show_line_num_action->isIconVisibleInMenu())
    {
        return;
    }
    QPainter painter(lineNumberArea);
    int scrollOffset = verticalScrollBar()->value();
    QTextBlock block = document()->firstBlock();
    int blockNumber = 0;
    while (block.isValid())
    {
        QRectF blockRect = document()->documentLayout()->blockBoundingRect(block);
        double topInViewport = blockRect.top() - scrollOffset;
        double bottomInViewport = blockRect.bottom() - scrollOffset;
        if (bottomInViewport >= 0 && topInViewport <= lineNumberArea->height())
        {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(line_text_color);
            int y = qRound(topInViewport);
            if (lineArea_left)
            {
                painter.drawText(0, y, lineNumberArea->width() - 16, qRound(blockRect.height()), Qt::AlignRight | Qt::AlignVCenter, number);
            }
            else
            {
                painter.drawText(16, y, lineNumberArea->width() - 16 - 8, qRound(blockRect.height()), Qt::AlignRight | Qt::AlignVCenter, number);
            }
            if (markedLines.contains(blockNumber))
            {
                painter.setRenderHint(QPainter::Antialiasing, true);
                painter.setBrush(line_mark_color);
                painter.setPen(Qt::NoPen);
                int dotDiameter = 10;
                int dotY = y + qRound(blockRect.height()) / 2 - dotDiameter / 2 + 1;
                int dotX = 0;
                if (lineArea_left)
                {
                    dotX = lineNumberArea->width() - 10 - dotDiameter / 2;

                }
                else
                {
                    dotX = 3;
                }
                painter.drawEllipse(dotX, dotY, dotDiameter, dotDiameter);
            }
        }
        block = block.next();
        ++blockNumber;
    }
}
void Basic_TextEdit::resizeEvent(QResizeEvent *event)
{
    QTextEdit::resizeEvent(event);
    updateLineNumberAreaWidth();
    if (m_statusBar->isVisible())
    {
        m_statusBar->setGeometry(0, height() - m_statusBar->height(), width(), m_statusBar->height());
    }
    QRect editorRect = geometry();
    int x = editorRect.x() + editorRect.width() - 400 - 10;
    int y = 10;
    m_searchBar->setGeometry(x, y, 400, 80);
}
void Basic_TextEdit::updateStatusBar_style()
{
    m_statusBar->setStyleSheet(QString("QStatusBar{background:rgba(240,240,240,150); color:rgba(%1,%2,%3,%4); border-radius: 7px 7px;}").arg(statusBar_text_color.red()).arg(statusBar_text_color.green()).arg(statusBar_text_color.blue()).arg(statusBar_text_color.alpha()));
    statusLabel->setStyleSheet(QString("QLabel{background:rgba(0,0,0,0); color:rgba(%1,%2,%3,%4);}").arg(statusBar_text_color.red()).arg(statusBar_text_color.green()).arg(statusBar_text_color.blue()).arg(statusBar_text_color.alpha()));
}
void Basic_TextEdit::updateStatusBar()
{
    if (!m_statusBar->isVisible())
    {
        return;
    }
    m_statusBar->setGeometry(0, height() - m_statusBar->height(), width(), m_statusBar->height());
    int totalChars = document()->characterCount() - 1;
    int totalLines = document()->blockCount();
    QTextCursor cursor = textCursor();
    int currentLine = cursor.block().blockNumber() + 1;
    int currentColumn = cursor.positionInBlock() + 1;
    int selectedChars = 0;
    if (cursor.hasSelection())
    {
        selectedChars = cursor.selectedText().length();
    }
    QString statusText = QString(tr("选择: %1 字 | 行: %2, 列: %3 | 总字数: %4  总行数: %5")).arg(selectedChars).arg(currentLine).arg(currentColumn).arg(totalChars).arg(totalLines);
    statusLabel->setText(statusText);
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
    if (QApplication::mouseButtons() & Qt::LeftButton)
    {
        Column_end_cursor = this->cursorForPosition(event->pos());
        if (QApplication::keyboardModifiers() & Qt::AltModifier)
        {
            updateColumnSelection();
            return;
        }
        else
        {
            clearColumnSelection();
            NormalSelection();
        }
    }
    QTextEdit::mouseMoveEvent(event);
}
void Basic_TextEdit::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MouseButton::MiddleButton)
    {
        return;
    }
    if(event->button() == Qt::LeftButton)
    {
        clearColumnSelection();
        Column_start_cursor = this->cursorForPosition(event->pos());
        Column_end_cursor = Column_start_cursor;
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
            return;
        }
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
    if (!extraSelections_list.isEmpty() && extraSelections_list[0].cursor.hasSelection())
    {
        for (int i = 0; i < menu->actions().size(); i++)
        {
            if (menu->actions()[i]->objectName() == "edit-cut")
            {
                menu->actions()[i]->setEnabled(true);
            }
            if (menu->actions()[i]->objectName() == "edit-copy")
            {
                menu->actions()[i]->setEnabled(true);
            }
        }
    }
    menu->insertMenu(menu->actions()[0], extra_menu);
    menu->insertMenu(menu->actions()[1], set_control_menu);
    menu->insertMenu(menu->actions()[2], insert_menu);
    menu->insertSeparator(menu->actions()[3]);
    menu->insertMenu(menu->actions()[4], extra_mode);
    menu->insertMenu(menu->actions()[5], set_apperance_menu);
    menu->insertSeparator(menu->actions()[6]);
    menu->insertAction(menu->actions()[7], search_for_text_action);
    menu->insertAction(menu->actions()[8], jump_to_line);
    menu->insertSeparator(menu->actions()[9]);
    menu->addSeparator();
    menu->addAction(window_control);
    extra_menu->setEnabled((textCursor().hasSelection() || (!extraSelections_list.isEmpty() && extraSelections_list[0].cursor.hasSelection())) && !isReadOnly());
    set_image_size_action->setEnabled(isSelectionImage());
    set_table->setEnabled(isSelectionTable());
    set_control_menu->setEnabled(set_image_size_action->isEnabled() || set_table->isEnabled());
    insert_menu->setEnabled(!isReadOnly());
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
        QTextCursor help_cursor;
        if (extraSelections_list.isEmpty())
        {
            help_cursor = QTextCursor(this->textCursor());
            help_cursor.setPosition(textCursor().selectionStart() + 1);
        }
        else
        {
            help_cursor = QTextCursor(extraSelections_list[0].cursor);
            help_cursor.setPosition(extraSelections_list[0].cursor.selectionStart() + 1);
        }
        QTextCharFormat fmt = help_cursor.charFormat();
        fmt = Basic_format_Set(fmt);
        set_font_B->setIconVisibleInMenu(fmt.fontWeight() == QFont::Bold);
        set_font_I->setIconVisibleInMenu(fmt.fontItalic());
    }
}
void Basic_TextEdit::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Insert)
    {
        insert_mode_action->setIconVisibleInMenu(!insert_mode_action->isIconVisibleInMenu());
        setCursorWidth(insert_mode_action->isIconVisibleInMenu() ? 8 : 2);
        event->accept();
        return;
    }
    if (event->matches(QKeySequence::Find))
    {
        if (!extraSelections_list.isEmpty())
        {
            QString text;
            for (const auto &sel : extraSelections_list)
            {
                text += sel.cursor.selectedText() + QChar::LineFeed;
            }
            if (!text.isEmpty())
            {
                text.chop(1);
                m_searchBar->setfindText(text);
            }
        }
        else
        {
            QString text = this->textCursor().selectedText();
            if (!text.isEmpty())
            {
                m_searchBar->setfindText(text);
            }
        }
        m_searchBar->showSearchBar();
        return;
    }
    if (event->modifiers() == Qt::ControlModifier && event->key() == Qt::Key_H)
    {
        if (!extraSelections_list.isEmpty())
        {
            QString text;
            for (const auto &sel : extraSelections_list)
            {
                text += sel.cursor.selectedText() + QChar::LineFeed;
            }
            if (!text.isEmpty())
            {
                text.chop(1);
                m_searchBar->setfindText(text);
            }
        }
        else
        {
            QString text = this->textCursor().selectedText();
            if (!text.isEmpty())
            {
                m_searchBar->setfindText(text);
            }
        }
        m_searchBar->showSearchBar();
        return;
    }
    if (!extraSelections_list.isEmpty())
    {
        std::sort(extraSelections_list.begin(), extraSelections_list.end(), [](const QTextEdit::ExtraSelection &a, const QTextEdit::ExtraSelection &b)
        {
            return a.cursor.selectionStart() < b.cursor.selectionStart();
        });
        if (event->matches(QKeySequence::Copy))
        {
            QString text;
            for (const auto &sel : extraSelections_list)
            {
                text += sel.cursor.selectedText() + QChar::LineFeed;
            }
            text.chop(1);
            QApplication::clipboard()->setText(text);
            event->accept();
            return;
        }
        else if (event->matches(QKeySequence::Cut))
        {
            if (isReadOnly())
            {
                QString text;
                for (const auto &sel : extraSelections_list)
                {
                    text += sel.cursor.selectedText() + QChar::LineFeed;
                }
                text.chop(1);
                QApplication::clipboard()->setText(text);
                event->accept();
                return;
            }
            QString text;
            for (const auto &sel : extraSelections_list)
            {
                text += sel.cursor.selectedText() + QChar::LineFeed;
            }
            text.chop(1);
            QApplication::clipboard()->setText(text);
            QTextCursor cursor = textCursor();
            cursor.beginEditBlock();
            for (auto it = extraSelections_list.rbegin(); it != extraSelections_list.rend(); ++it)
            {
                it->cursor.removeSelectedText();
            }
            cursor.endEditBlock();
            clearColumnSelection();
            event->accept();
            return;
        }
        else if (event->key() == Qt::Key_Escape)
        {
            clearColumnSelection();
            event->accept();
            return;
        }
        else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_Down || event->key() == Qt::Key_Left || event->key() == Qt::Key_Right)
        {
            clearColumnSelection();
            event->accept();
            return;
        }
        else if (event->key() == Qt::Key_Backspace)
        {
            if (isReadOnly())
            {
                return;
            }
            QTextCursor cursor = textCursor();
            cursor.beginEditBlock();
            for (auto it = extraSelections_list.rbegin(); it != extraSelections_list.rend(); ++it)
            {
                QTextCursor editCursor = it->cursor;
                if (editCursor.hasSelection())
                {
                    editCursor.removeSelectedText();
                }
                else
                {
                    int pos = editCursor.position();
                    if (pos > 0)
                    {
                        editCursor.setPosition(pos - 1);
                        editCursor.setPosition(pos, QTextCursor::KeepAnchor);
                        editCursor.removeSelectedText();
                    }
                }
            }
            cursor.endEditBlock();
            clearColumnSelection();
            event->accept();
            return;
        }
        else if (event->key() == Qt::Key_Delete)
        {
            if (isReadOnly())
            {
                return;
            }
            QTextCursor cursor = textCursor();
            cursor.beginEditBlock();
            int docLen = document()->characterCount() - 1;
            for (auto it = extraSelections_list.rbegin(); it != extraSelections_list.rend(); ++it)
            {
                QTextCursor editCursor = it->cursor;
                if (editCursor.hasSelection())
                {
                    editCursor.removeSelectedText();
                }
                else
                {
                    int pos = editCursor.position();
                    if (pos < docLen)
                    {
                        editCursor.setPosition(pos + 1);
                        editCursor.setPosition(pos, QTextCursor::KeepAnchor);
                        editCursor.removeSelectedText();
                    }
                }
            }
            cursor.endEditBlock();
            clearColumnSelection();
            event->accept();
            return;
        }
        else if (!event->text().isEmpty() && event->text()[0].isPrint())
        {
            if (isReadOnly())
            {
                return;
            }
            if (insert_mode_action->isIconVisibleInMenu())
            {
                QString text = event->text();
                textCursor().beginEditBlock();
                for (auto &selection : extraSelections_list)
                {
                    QTextCursor cursor = selection.cursor;
                    if (cursor.hasSelection())
                    {
                        cursor.insertText(text);
                    }
                    else
                    {
                        int pos = cursor.position();
                        int docLen = document()->characterCount() - 1;
                        if (pos < docLen)
                        {
                            QChar nextChar = document()->characterAt(pos);
                            if (nextChar != QChar::ParagraphSeparator && nextChar != QChar::LineSeparator)
                            {
                                cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
                                cursor.insertText(text);
                            }
                        }
                    }
                }
                textCursor().endEditBlock();
                event->accept();
                return;
            }
            else
            {
                QString text = event->text();
                QTextCursor cursor = textCursor();
                cursor.beginEditBlock();
                for (auto it = extraSelections_list.rbegin(); it != extraSelections_list.rend(); ++it)
                {
                    QTextCursor editCursor = it->cursor;
                    if (editCursor.hasSelection())
                    {
                        editCursor.removeSelectedText();
                    }

                    editCursor.insertText(text);
                }
                cursor.endEditBlock();
                updateColumnSelection();
                event->accept();
                return;
            }
        }
    }
    if (!event->text().isEmpty() && event->text()[0].isPrint())//非列编辑
    {
        QString text = event->text();
        QTextCursor cursor = textCursor();
        cursor.beginEditBlock();
        if (insert_mode_action->isIconVisibleInMenu() && !cursor.hasSelection())
        {
            int pos = cursor.position();
            int docLen = document()->characterCount() - 1;
            if (pos < docLen)
            {
                QChar nextChar = document()->characterAt(pos);
                if (nextChar != QChar::ParagraphSeparator && nextChar != QChar::LineSeparator)
                {
                    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 1);
                    cursor.insertText(text);
                    cursor.endEditBlock();
                    event->accept();
                    return;
                }
            }
        }
        cursor.insertText(text);
        cursor.endEditBlock();
        event->accept();
        return;
    }
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
void Basic_TextEdit::inputMethodEvent(QInputMethodEvent *event)
{
    if (!extraSelections_list.isEmpty() && !event->commitString().isEmpty())
    {
        QString text = event->commitString();
        QTextCursor cursor = textCursor();
        cursor.beginEditBlock();
        for (auto it = extraSelections_list.rbegin(); it != extraSelections_list.rend(); ++it)
        {
            QTextCursor editCursor = it->cursor;
            if (editCursor.hasSelection())
            {
                editCursor.removeSelectedText();
            }
            editCursor.insertText(text);
        }
        cursor.endEditBlock();
        updateColumnSelection();
        event->accept();
        return;
    }
    QTextEdit::inputMethodEvent(event);
}
QString Basic_TextEdit::getSearchText() const
{
    return m_searchBar->findText();
}
QString Basic_TextEdit::getReplaceText() const
{
    return m_searchBar->replaceText();
}
void Basic_TextEdit::wheelEvent(QWheelEvent *event)
{
    if ((event->modifiers() & Qt::ControlModifier) && wheel_change_size_action->isIconVisibleInMenu())
    {
        if (event->angleDelta().y() > 0)
        {
            ZoomIn();//这个不是QTextEdit的zoomIn,是自己实现的
        }
        else
        {
            ZoomOut();
        }
        event->accept();
    }
    else
    {
        QTextEdit::wheelEvent(event);
        event->accept();
    }
}
void Basic_TextEdit::updateColumnSelection()
{
    if (Column_start_cursor.isNull() || Column_end_cursor.isNull())
    {
        return;
    }
    QTextCursor cursor = textCursor();
    cursor.clearSelection();
    setTextCursor(cursor);
    QTextBlock startBlock = Column_start_cursor.block();
    QTextBlock endBlock = Column_end_cursor.block();
    int startCol = Column_start_cursor.positionInBlock();
    int endCol = Column_end_cursor.positionInBlock();
    if (startBlock.blockNumber() > endBlock.blockNumber())
    {
        qSwap(startBlock, endBlock);
    }
    int leftCol = qMin(startCol, endCol);
    int rightCol = qMax(startCol, endCol);
    extraSelections_list.clear();
    m_columnCursors.clear();
    QTextBlock block = startBlock;
    while (block.isValid())
    {
        int blockLen = block.length() - 1;
        int selStart = qMin(leftCol, blockLen);
        int selEnd = qMin(rightCol, blockLen);
        if (selStart <= selEnd)
        {
            QTextCursor cursor(block);
            cursor.setPosition(block.position() + selStart);
            cursor.setPosition(block.position() + selEnd, QTextCursor::KeepAnchor);
            QTextEdit::ExtraSelection extra;
            extra.cursor = cursor;
            extra.format.setBackground(selection_color);
            extraSelections_list.append(extra);
            QTextCursor insertCursor(block);
            insertCursor.setPosition(block.position() + selStart);
            m_columnCursors.append(insertCursor);
        }
        if (block == endBlock)
        {
            break;
        }
        block = block.next();
    }
    if (!m_columnCursors.isEmpty())
    {
        std::sort(extraSelections_list.begin(), extraSelections_list.end(), [](const QTextEdit::ExtraSelection &a, const QTextEdit::ExtraSelection &b)
        {
            return a.cursor.selectionStart() < b.cursor.selectionStart();
        });
    }
    setExtraSelections(extraSelections_list);
    mergeAndSetExtraSelections();
}
void Basic_TextEdit::clearColumnSelection()
{
    extraSelections_list.clear();
    if (!m_columnCursors.isEmpty() || !extraSelections().isEmpty())
    {
        m_columnCursors.clear();
        setExtraSelections(QList<QTextEdit::ExtraSelection>());
    }
    mergeAndSetExtraSelections();
}
void Basic_TextEdit::NormalSelection()
{
    extraSelections_list.clear();
    m_columnCursors.clear();
    if (!Column_start_cursor.isNull() && !Column_end_cursor.isNull())
    {
        QTextCursor selectionCursor = Column_start_cursor;
        selectionCursor.setPosition(Column_end_cursor.position(), QTextCursor::KeepAnchor);
        setTextCursor(selectionCursor);
    }
    mergeAndSetExtraSelections();
}
void Basic_TextEdit::ZoomIn()
{
    QTextDocument *doc = document();
    QTextCursor cursor(doc);
    cursor.beginEditBlock();
    for (QTextBlock block = doc->begin(); block != doc->end(); block = block.next())
    {
        for (auto it = block.begin(); !it.atEnd(); ++it)
        {
            QTextFragment fragment = it.fragment();
            if (!fragment.isValid()) continue;
            if (fragment.charFormat().isImageFormat())
            {
                QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
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
                imageFormat.setWidth(imageFormat.width() * 1.05);
                imageFormat.setHeight(imageFormat.height() * 1.05);
                cursor.setPosition(fragment.position());
                cursor.setPosition(fragment.position() + fragment.length(), QTextCursor::KeepAnchor);
                cursor.setCharFormat(imageFormat);
            }
            else
            {
                QTextCharFormat format = fragment.charFormat();
                QFont font = format.font();
                double newSize = font.pointSizeF() * 1.05;
                newSize = qMax(1.0, newSize);
                font.setPointSizeF(newSize);
                format.setFont(font);
                cursor.setPosition(fragment.position());
                cursor.setPosition(fragment.position() + fragment.length(), QTextCursor::KeepAnchor);
                cursor.setCharFormat(format);
            }
        }
    }
    cursor.endEditBlock();
    updateLineNumberAreaWidth();
    lineNumberArea->update();
}
void Basic_TextEdit::ZoomOut()
{
    QTextDocument *doc = document();
    QTextCursor cursor(doc);
    cursor.beginEditBlock();
    for (QTextBlock block = doc->begin(); block != doc->end(); block = block.next())
    {
        for (auto it = block.begin(); !it.atEnd(); ++it)
        {
            QTextFragment fragment = it.fragment();
            if (!fragment.isValid()) continue;
            if (fragment.charFormat().isImageFormat())
            {
                QTextImageFormat imageFormat = fragment.charFormat().toImageFormat();
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
                imageFormat.setWidth(imageFormat.width() * 0.95);
                imageFormat.setHeight(imageFormat.height() * 0.95);
                if (imageFormat.width() > 10 && imageFormat.height() > 10)
                {
                    cursor.setPosition(fragment.position());
                    cursor.setPosition(fragment.position() + fragment.length(), QTextCursor::KeepAnchor);
                    cursor.setCharFormat(imageFormat);
                }
            }
            else
            {
                QTextCharFormat format = fragment.charFormat();
                QFont font = format.font();
                double newSize = font.pointSizeF() * 0.95;
                newSize = qMax(1.0, newSize);
                font.setPointSizeF(newSize);
                format.setFont(font);
                cursor.setPosition(fragment.position());
                cursor.setPosition(fragment.position() + fragment.length(), QTextCursor::KeepAnchor);
                cursor.setCharFormat(format);
            }
        }
    }
    cursor.endEditBlock();
    updateLineNumberAreaWidth();
    lineNumberArea->update();
}
bool Basic_TextEdit::isSelectionImage()
{
    QString selectedText;
    QTextCharFormat charFormat;
    if (extraSelections_list.isEmpty())
    {
        if (!this->textCursor().hasSelection())
        {
            return false;
        }
        selectedText = this->textCursor().selectedText();
        charFormat = this->textCursor().charFormat();
    }
    else
    {
        for (auto &selection : extraSelections_list)
        {
            if (selection.cursor.hasSelection())
            {
                selectedText = selection.cursor.selectedText();
                charFormat = this->textCursor().charFormat();
                break;
            }
        }
    }
    for (int i = 0; i < selectedText.length(); i++)
    {
        if (selectedText.at(i) != QChar::ObjectReplacementCharacter && selectedText.at(i) != "\u2029")
        {
            return false;
        }
    }
    return charFormat.isImageFormat();
}
bool Basic_TextEdit::isSelectionTable()
{
    if (extraSelections_list.isEmpty())
    {
        return this->textCursor().currentTable() != nullptr;
    }
    else
    {
        for (auto &selection : extraSelections_list)
        {
            QTextTable *table = selection.cursor.currentTable();
            if (table)
            {
                return true;
            }
        }
    }
    return false;

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
            return;
        }
        if (!extraSelections_list.isEmpty())
        {
            QString text;
            for (const auto &sel : extraSelections_list)
            {
                text += sel.cursor.selectedText() + QChar::LineFeed;
            }
            text.chop(1);
            QApplication::clipboard()->setText(text);
            return;
        }
    }
    else if (action != nullptr && action->objectName() == "edit-cut")
    {
        if (had_selected)//该bool仅为image存在的标记
        {
            Basic_TextEdit::second_cut();
            return;
        }
        if (!extraSelections_list.isEmpty())
        {
            QString text;
            for (const auto &sel : extraSelections_list)
            {
                text += sel.cursor.selectedText() + QChar::LineFeed;
            }
            text.chop(1);
            QApplication::clipboard()->setText(text);
            QTextCursor cursor = textCursor();
            cursor.beginEditBlock();
            for (auto it = extraSelections_list.rbegin(); it != extraSelections_list.rend(); ++it)
            {
                it->cursor.removeSelectedText();
            }
            cursor.endEditBlock();
            clearColumnSelection();
            return;
        }
    }
    else if (action == insert_image_action)
    {
        QList<QString> urls = QFileDialog::getOpenFileNames(nullptr, tr("获取图像"), QDir::homePath(), tr("图像文件") + "(*.png *.jpg *.jpeg *.svg *.gif *.bmp);;" + tr("所有文件") + "(*.*)");
        My_X11_Libs::X11_Raise();//没办法,要跟dde-desktop争夺[桌面显示权]
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
        int rows = QInputDialog::getInt(nullptr, tr("获取数值"), tr("行数"), 1, 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int cols = QInputDialog::getInt(nullptr, tr("获取数值"), tr("列数"), 1, 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        QTextTableFormat tableFormat;
        tableFormat.setBorderStyle(QTextTableFormat::BorderStyle::BorderStyle_Double);
        if (extraSelections_list.isEmpty())
        {
            this->textCursor().insertTable(rows, cols, tableFormat);
        }
        else
        {
            extraSelections_list.first().cursor.insertTable(rows, cols, tableFormat);
        }
    }
    else if (action == set_image_size_action)
    {
        QTextImageFormat imageFormat;
        if (extraSelections_list.isEmpty())
        {
            imageFormat = this->textCursor().charFormat().toImageFormat();
        }
        else
        {
            for (auto &selection : extraSelections_list)
            {
                imageFormat = selection.cursor.charFormat().toImageFormat();
                if (!imageFormat.isValid())
                {
                    break;
                }
            }
        }
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
        qreal new_width = QInputDialog::getDouble(nullptr, tr("获取数值"), tr("宽度"), imageFormat.width(), 0.00, 2147483647.00, 1, &ok);
        if (!ok)
        {
            return;
        }
        qreal new_height = QInputDialog::getDouble(nullptr, tr("获取数值"), tr("高度"), imageFormat.height(), 0.00, 2147483647.00, 1, &ok);
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
        QTextTable *table = nullptr;
        if (extraSelections_list.isEmpty())
        {
            table = this->textCursor().currentTable();
        }
        else
        {
            for (auto &selection : extraSelections_list)
            {
                table = selection.cursor.currentTable();
                if (!table)
                {
                    break;
                }
            }
        }
        if (table == nullptr)
        {
            return;
        }
        bool ok = false;
        int rows = QInputDialog::getInt(nullptr, tr("获取数值"), tr("行数"), table->rows(), 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int cols = QInputDialog::getInt(nullptr, tr("获取数值"), tr("列数"), table->columns(), 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        table->resize(rows, cols);
    }
    else if (action == merge_cells_action)
    {
        QTextTable *table = nullptr;
        if (extraSelections_list.isEmpty())
        {
            table = this->textCursor().currentTable();
        }
        else
        {
            for (auto &selection : extraSelections_list)
            {
                table = selection.cursor.currentTable();
                if (!table)
                {
                    break;
                }
            }
        }
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
            int rows = QInputDialog::getInt(nullptr, tr("获取数值"), tr("向下合并[行数]"), 1, 0, 2147483647, 1, &ok);
            if (!ok)
            {
                return;
            }
            ok = false;
            int cols = QInputDialog::getInt(nullptr, tr("获取数值"), tr("向右合并[列数]"), 1, 0, 2147483647, 1, &ok);
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
        QTextTable *table = nullptr;
        if (extraSelections_list.isEmpty())
        {
            table = this->textCursor().currentTable();
        }
        else
        {
            for (auto &selection : extraSelections_list)
            {
                table = selection.cursor.currentTable();
                if (!table)
                {
                    break;
                }
            }
        }
        if (table == nullptr)
        {
            return;
        }
        bool ok = false;
        int rows = QInputDialog::getInt(nullptr, tr("获取数值"), tr("拆分[行数]维度\n得到单元格所占行数"), 1, 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int cols = QInputDialog::getInt(nullptr, tr("获取数值"), tr("拆分为[列数]维度\n得到单元格所占行数"), 1, 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        QTextTableCell cell = table->cellAt(this->textCursor());
        table->splitCell(cell.row(), cell.column(), rows, cols);
    }
    else if (action == clear_style_action)
    {
        QTextCharFormat fmt;
        fmt.setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
        fmt.setFontItalic(false);
        fmt.setFontWeight(QFont::Normal);
        fmt.setBackground(Qt::transparent);
        fmt.setForeground(Qt::black);
        fmt.setFontUnderline(false);
        if (extraSelections_list.isEmpty())
        {
            if (textCursor().hasSelection())
            {
                this->textCursor().mergeCharFormat(fmt);
            }
        }
        else
        {
            this->textCursor().beginEditBlock();
            for (auto &ext_selection : extraSelections_list)
            {
                ext_selection.cursor.mergeCharFormat(fmt);
            }
            this->textCursor().endEditBlock();
        }
    }
    else if (action == record_style_action)
    {
        if (extraSelections_list.isEmpty())
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
        else
        {
            QTextCursor help_cursor = QTextCursor(extraSelections_list[0].cursor);
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
        if (extraSelections_list.isEmpty())
        {
            if (textCursor().hasSelection())
            {
                this->textCursor().mergeCharFormat(basic_format);
            }
        }
        else
        {
            this->textCursor().beginEditBlock();
            for (auto &ext_selection : extraSelections_list)
            {
                ext_selection.cursor.mergeCharFormat(basic_format);
            }
            this->textCursor().endEditBlock();
        }
    }
    else if (action == set_Top_A_action)
    {
        if (extraSelections_list.isEmpty())
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
        else
        {
            this->textCursor().beginEditBlock();
            for (auto &ext_selection : extraSelections_list)
            {
                int start = ext_selection.cursor.selectionStart();
                int end = ext_selection.cursor.selectionEnd();
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
            }
            this->textCursor().endEditBlock();
        }
    }
    else if (action == set_A_action)
    {
        if (extraSelections_list.isEmpty())
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
        else
        {
            this->textCursor().beginEditBlock();
            for (auto &ext_selection : extraSelections_list)
            {
                int start = ext_selection.cursor.selectionStart();
                int end = ext_selection.cursor.selectionEnd();
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
            }
            this->textCursor().endEditBlock();
        }
    }
    else if (action == set_a_action)
    {
        if (extraSelections_list.isEmpty())
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
        else
        {
            this->textCursor().beginEditBlock();
            for (auto &ext_selection : extraSelections_list)
            {
                int start = ext_selection.cursor.selectionStart();
                int end = ext_selection.cursor.selectionEnd();
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
            }
            this->textCursor().endEditBlock();
        }
    }
    else if (action == format_set_font)
    {
        QTextCursor help_cursor;
        if (extraSelections_list.isEmpty())
        {
            help_cursor = QTextCursor(this->textCursor());
            help_cursor.setPosition(textCursor().selectionStart() + 1);
        }
        else
        {
            help_cursor = QTextCursor(extraSelections_list[0].cursor);
            help_cursor.setPosition(extraSelections_list[0].cursor.selectionStart() + 1);
        }
        QTextCharFormat fmt = help_cursor.charFormat();
        fmt = Basic_format_Set(fmt);
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, fmt.font(), nullptr);
        if (!ok)
        {
            return;
        }
        QTextCursor running_cursor = QTextCursor(this->document());
        this->textCursor().beginEditBlock();
        if (extraSelections_list.isEmpty())
        {
            int start = this->textCursor().selectionStart();
            int end = this->textCursor().selectionEnd();
            QTextCharFormat ins_Format;
            for (int i = start; i < end; i++)
            {
                running_cursor.setPosition(i);
                running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
                ins_Format = running_cursor.charFormat();
                ins_Format.setFont(font);
                running_cursor.mergeCharFormat(ins_Format);
            }
        }
        else
        {
            for (auto &ext_selection : extraSelections_list)
            {
                int start = ext_selection.cursor.selectionStart();
                int end = ext_selection.cursor.selectionEnd();
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
            }
        }
        this->textCursor().endEditBlock();
    }
    else if (action == format_set_font_color)
    {
        QTextCursor help_cursor;
        if (extraSelections_list.isEmpty())
        {
            help_cursor = QTextCursor(this->textCursor());
            help_cursor.setPosition(textCursor().selectionStart() + 1);
        }
        else
        {
            help_cursor = QTextCursor(extraSelections_list[0].cursor);
            help_cursor.setPosition(extraSelections_list[0].cursor.selectionStart() + 1);
        }
        QTextCharFormat fmt = help_cursor.charFormat();
        fmt = Basic_format_Set(fmt);
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(fmt.foreground().color());
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        QTextCursor running_cursor = QTextCursor(this->document());
        this->textCursor().beginEditBlock();
        if (extraSelections_list.isEmpty())
        {
            int start = this->textCursor().selectionStart();
            int end = this->textCursor().selectionEnd();
            QTextCharFormat ins_Format;
            for (int i = start; i < end; i++)
            {
                running_cursor.setPosition(i);
                running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
                ins_Format = running_cursor.charFormat();
                ins_Format.setForeground(colorDialog.currentColor());
                running_cursor.mergeCharFormat(ins_Format);
            }
        }
        else
        {
            for (auto &ext_selection : extraSelections_list)
            {
                int start = ext_selection.cursor.selectionStart();
                int end = ext_selection.cursor.selectionEnd();
                QTextCharFormat ins_Format;
                for (int i = start; i < end; i++)
                {
                    running_cursor.setPosition(i);
                    running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
                    ins_Format = running_cursor.charFormat();
                    ins_Format.setForeground(colorDialog.currentColor());
                    running_cursor.mergeCharFormat(ins_Format);
                }
            }
        }
        this->textCursor().endEditBlock();
    }
    else if (action == format_set_background_color)
    {
        QTextCursor help_cursor;
        if (extraSelections_list.isEmpty())
        {
            help_cursor = QTextCursor(this->textCursor());
            help_cursor.setPosition(textCursor().selectionStart() + 1);
        }
        else
        {
            help_cursor = QTextCursor(extraSelections_list[0].cursor);
            help_cursor.setPosition(extraSelections_list[0].cursor.selectionStart() + 1);
        }
        QTextCharFormat fmt = help_cursor.charFormat();
        fmt = Basic_format_Set(fmt);
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(fmt.background().color());
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        QTextCursor running_cursor = QTextCursor(this->document());
        this->textCursor().beginEditBlock();
        if (extraSelections_list.isEmpty())
        {
            int start = this->textCursor().selectionStart();
            int end = this->textCursor().selectionEnd();
            QTextCharFormat ins_Format;
            for (int i = start; i < end; i++)
            {
                running_cursor.setPosition(i);
                running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
                ins_Format = running_cursor.charFormat();
                ins_Format.setBackground(colorDialog.currentColor());
                running_cursor.mergeCharFormat(ins_Format);
            }
        }
        else
        {
            for (auto &ext_selection : extraSelections_list)
            {
                int start = ext_selection.cursor.selectionStart();
                int end = ext_selection.cursor.selectionEnd();
                QTextCharFormat ins_Format;
                for (int i = start; i < end; i++)
                {
                    running_cursor.setPosition(i);
                    running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
                    ins_Format = running_cursor.charFormat();
                    ins_Format.setBackground(colorDialog.currentColor());
                    running_cursor.mergeCharFormat(ins_Format);
                }
            }
        }
        this->textCursor().endEditBlock();
    }
    else if (action == set_font_I)
    {
        QTextCursor running_cursor = QTextCursor(this->document());
        this->textCursor().beginEditBlock();
        if (extraSelections_list.isEmpty())
        {
            int start = this->textCursor().selectionStart();
            int end = this->textCursor().selectionEnd();
            QTextCharFormat ins_Format;
            for (int i = start; i < end; i++)
            {
                running_cursor.setPosition(i);
                running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
                ins_Format = running_cursor.charFormat();
                ins_Format.setFontItalic(!set_font_I->isIconVisibleInMenu());
                running_cursor.mergeCharFormat(ins_Format);
            }
        }
        else
        {
            for (auto &ext_selection : extraSelections_list)
            {
                int start = ext_selection.cursor.selectionStart();
                int end = ext_selection.cursor.selectionEnd();
                QTextCharFormat ins_Format;
                for (int i = start; i < end; i++)
                {
                    running_cursor.setPosition(i);
                    running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
                    ins_Format = running_cursor.charFormat();
                    ins_Format.setFontItalic(!set_font_I->isIconVisibleInMenu());
                    running_cursor.mergeCharFormat(ins_Format);
                }
            }
        }
        this->textCursor().endEditBlock();
        set_font_I->setIconVisibleInMenu(!set_font_I->isIconVisibleInMenu());
    }
    else if (action == set_font_B)
    {
        QTextCursor running_cursor = QTextCursor(this->document());
        this->textCursor().beginEditBlock();
        if (extraSelections_list.isEmpty())
        {
            int start = this->textCursor().selectionStart();
            int end = this->textCursor().selectionEnd();
            QTextCharFormat ins_Format;
            for (int i = start; i < end; i++)
            {
                running_cursor.setPosition(i);
                running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
                ins_Format = running_cursor.charFormat();
                ins_Format.setFontWeight(set_font_B->isIconVisibleInMenu()?QFont::Normal:QFont::Bold);
                running_cursor.mergeCharFormat(ins_Format);
            }
        }
        else
        {
            for (auto &ext_selection : extraSelections_list)
            {
                int start = ext_selection.cursor.selectionStart();
                int end = ext_selection.cursor.selectionEnd();
                QTextCharFormat ins_Format;
                for (int i = start; i < end; i++)
                {
                    running_cursor.setPosition(i);
                    running_cursor.setPosition(i + 1, QTextCursor::MoveMode::KeepAnchor);
                    ins_Format = running_cursor.charFormat();
                    ins_Format.setFontWeight(set_font_B->isIconVisibleInMenu()?QFont::Normal:QFont::Bold);
                    running_cursor.mergeCharFormat(ins_Format);
                }
            }
        }
        this->textCursor().endEditBlock();
        set_font_B->setIconVisibleInMenu(!set_font_B->isIconVisibleInMenu());
    }
    else if (action == wheel_change_size_action)
    {
        wheel_change_size_action->setIconVisibleInMenu(!wheel_change_size_action->isIconVisibleInMenu());
    }
    else if (action == set_selection_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(selection_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        selection_color = colorDialog.currentColor();
    }
    else if (action == insert_mode_action)
    {
        insert_mode_action->setIconVisibleInMenu(!insert_mode_action->isIconVisibleInMenu());
        setCursorWidth(insert_mode_action->isIconVisibleInMenu() ? 8 : 2);
    }
    else if (action == search_for_text_action)
    {
        if (!extraSelections_list.isEmpty())
        {
            QString text;
            for (const auto &sel : extraSelections_list)
            {
                text += sel.cursor.selectedText() + QChar::LineFeed;
            }
            if (!text.isEmpty())
            {
                text.chop(1);
                m_searchBar->setfindText(text);
            }
        }
        else
        {
            QString text = this->textCursor().selectedText();
            if (!text.isEmpty())
            {
                m_searchBar->setfindText(text);
            }
        }
        m_searchBar->showSearchBar();
    }
    else if (action == set_search_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(search_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        search_color = colorDialog.currentColor();
    }
    else if (action == set_search_select_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(search_and_select_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        search_and_select_color = colorDialog.currentColor();
    }
    else if (action == jump_to_line)
    {
        bool ok = false;
        int cursor_line = 1;
        if (extraSelections_list.isEmpty())
        {
            cursor_line = this->textCursor().block().firstLineNumber() + 1;
        }
        else
        {
            cursor_line = extraSelections_list.first().cursor.block().firstLineNumber() + 1;
        }
        int lineNumber = QInputDialog::getInt(nullptr, tr("跳转到行"), tr("行号 (1~%1):").arg(document()->blockCount()), cursor_line, 1, document()->blockCount(), 1, &ok);
        if (!ok)
        {
            return;
        }
        int blockIndex = lineNumber - 1;
        QTextBlock targetBlock = document()->findBlockByNumber(blockIndex);
        if (!targetBlock.isValid())
        {
            return;
        }
        QTextCursor cursor(targetBlock);
        cursor.movePosition(QTextCursor::StartOfBlock);
        setTextCursor(cursor);
        ensureCursorVisible();
    }
    else if (action == show_line_num_action)
    {
        show_line_num_action->setIconVisibleInMenu(!show_line_num_action->isIconVisibleInMenu());
        updateLineNumberAreaWidth();
    }
    else if (action == set_show_line_pos_left_action)
    {
        lineArea_left = true;
        set_show_line_pos_left_action->setIconVisibleInMenu(true);
        set_show_line_pos_right_action->setIconVisibleInMenu(false);
        updateLineNumberAreaWidth();
    }
    else if (action == set_show_line_pos_right_action)
    {
        lineArea_left = false;
        set_show_line_pos_left_action->setIconVisibleInMenu(false);
        set_show_line_pos_right_action->setIconVisibleInMenu(true);
        updateLineNumberAreaWidth();
    }
    else if (action == set_show_line_mark_color_action)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(line_mark_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        line_mark_color = colorDialog.currentColor();
        updateLineNumberAreaWidth();
    }
    else if (action == set_show_line_text_color_action)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(line_text_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        line_text_color = colorDialog.currentColor();
        updateLineNumberAreaWidth();
    }
    else if (action == set_show_status_bar)
    {
        set_show_status_bar->setIconVisibleInMenu(!set_show_status_bar->isIconVisibleInMenu());
        updateLineNumberAreaWidth();
        updateStatusBar();
    }
    else if (action == set_show_status_bar_text_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(statusBar_text_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        statusBar_text_color = colorDialog.currentColor();
        updateStatusBar_style();
        updateLineNumberAreaWidth();
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
    QTextImageFormat imageFormat;
    if (extraSelections_list.isEmpty())
    {
        imageFormat = this->textCursor().charFormat().toImageFormat();
    }
    else
    {
        imageFormat = extraSelections_list.first().cursor.charFormat().toImageFormat();
    }
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
    QTextImageFormat imageFormat;
    if (extraSelections_list.isEmpty())
    {
        imageFormat = this->textCursor().charFormat().toImageFormat();
    }
    else
    {
        imageFormat = extraSelections_list.first().cursor.charFormat().toImageFormat();
    }
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
    if (!extraSelections_list.isEmpty() && source->hasText())
    {
        QStringList lines = source->text().split('\n');
        std::sort(extraSelections_list.begin(), extraSelections_list.end(), [](const QTextEdit::ExtraSelection &a, const QTextEdit::ExtraSelection &b)
        {
            return a.cursor.selectionStart() < b.cursor.selectionStart();
        });
        QTextCursor cursor = textCursor();
        cursor.beginEditBlock();
        //从后往前处理，避免位置偏移
        for (int i = extraSelections_list.size() - 1; i >= 0; --i)
        {
            const auto &sel = extraSelections_list[i];
            QString lineText = (i < lines.size()) ? lines[i] : (lines.isEmpty() ? QString() : lines.last());
            QTextCursor editCursor = sel.cursor;
            if (editCursor.hasSelection())
                editCursor.removeSelectedText();
            editCursor.insertText(lineText);
        }
        cursor.endEditBlock();
        clearColumnSelection();
        return;
    }
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
    QTextCursor cursor;
    if (extraSelections_list.isEmpty())
    {
        cursor = textCursor();
    }
    else
    {
        cursor = extraSelections_list.first().cursor;
    }
    QByteArray ba;
    QBuffer buffer(&ba);
    buffer.open(QIODevice::WriteOnly);
    image.save(&buffer, "PNG");
    QString imageData = QString("data:image/png;base64," + ba.toBase64());
    cursor.insertHtml(QString("<img src=\"%1\" style=\"background-color:transparent;\">").arg(imageData));
}
My_LineEdit::My_LineEdit(QWidget *parent)
    :Basic_Widget(parent)
{
    textEdit->setStyleSheet("QWidget{background:rgba(0,0,0,0);color:rgba(0,0,0,255)}"
                            "QMenu{border-radius:10px 10px;background:rgba(255,255,255,75);margin:0px -1px 0px -1px;padding-top:8px;padding-bottom:8px;icon-size:20px;border-radius:10px 10px}"
                            "QMenu::item{color:rgba(0,0,0,255);background:rgba(0,0,0,0)}"
                            "QMenu::item:disabled{color:rgba(131,136,139,255)}"
                            "QMenu::item:selected{color:rgba(255,255,255,255);background:rgba(0,129,255,255)}"
                            "QMenu::indicator{width: 20px; height 20px; background: transparent;}"
                            "QMenu::indicator:checked{image: url(:/base/this.svg);}"
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
        if (know_what != nullptr)
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
    textEdit->deleteLater();
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
void My_LineEdit::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    this->textEdit->H_save(settings, "");
}
void My_LineEdit::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    this->textEdit->H_load(settings, "");
}
void My_LineEdit::update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, QString checked_icon_path)
{
    textEdit->setStyleSheet(QString("QWidget{background:rgba(0,0,0,0);color:rgba(0,0,0,255)}"
                                    "QMenu{border-radius:10px 10px;background:rgba(%1,%2,%3,%4);margin:0px -1px 0px -1px;padding-top:8px;padding-bottom:8px;icon-size:20px;border-radius:10px 10px}"
                                    "QMenu::item{color:rgba(%5,%6,%7,%8);background:rgba(0,0,0,0);}"
                                    "QMenu::item:disabled{color:rgba(%9,%10,%11,%12)}"
                                    "QMenu::item:selected{color:rgba(%13,%14,%15,%16);background:rgba(%17,%18,%19,%20)}"
                                    "QMenu::indicator{width: 20px; height 20px; background: transparent;}"
                                    "QMenu::indicator:checked{image: url(%21);}"
                                    "QMenu::separator{background:rgba(150,150,150,125)}")
                            .arg(theme_background_color.red()).arg(theme_background_color.green()).arg(theme_background_color.blue()).arg(theme_background_color.alpha())
                            .arg(theme_text_color.red()).arg(theme_text_color.green()).arg(theme_text_color.blue()).arg(theme_text_color.alpha())
                            .arg(disabled_text_color.red()).arg(disabled_text_color.green()).arg(disabled_text_color.blue()).arg(disabled_text_color.alpha())
                            .arg(select_text_color.red()).arg(select_text_color.green()).arg(select_text_color.blue()).arg(select_text_color.alpha())
                            .arg(theme_color.red()).arg(theme_color.green()).arg(theme_color.blue()).arg(theme_color.alpha())
                            .arg(checked_icon_path));
}
void My_LineEdit::set_icon(QString checked_icon_path)
{
    textEdit->set_icon(checked_icon_path);
    Basic_Widget::set_icon(checked_icon_path);
}
void Basic_TextEdit::set_icon(QString checked_icon_path)
{
    auto_turn_line_action->setIcon(QIcon(checked_icon_path));
    read_only_action->setIcon(QIcon(checked_icon_path));
    center_paste_action->setIcon(QIcon(checked_icon_path));
    insert_image_action->setIcon(QIcon(checked_icon_path));
    set_image_size_action->setIcon(QIcon(checked_icon_path));
    set_table_size_action->setIcon(QIcon(checked_icon_path));
    clear_style_action->setIcon(QIcon(checked_icon_path));
    set_Top_A_action->setIcon(QIcon(checked_icon_path));
    set_font_B->setIcon(QIcon(checked_icon_path));
    set_font_I->setIcon(QIcon(checked_icon_path));
    wheel_change_size_action->setIcon(QIcon(checked_icon_path));
    insert_mode_action->setIcon(QIcon(checked_icon_path));
    show_line_num_action->setIcon(QIcon(checked_icon_path));
    set_show_line_pos_left_action->setIcon(QIcon(checked_icon_path));
    set_show_line_pos_right_action->setIcon(QIcon(checked_icon_path));
    set_show_status_bar->setIcon(QIcon(checked_icon_path));
}
void Basic_TextEdit::H_save(QSettings *settings, QString Token)
{
    settings->setValue(Token + "auto_turn_line", auto_turn_line_action->isIconVisibleInMenu());
    settings->setValue(Token + "read_only", read_only_action->isIconVisibleInMenu());
    settings->setValue(Token + "center_paste", center_paste_action->isIconVisibleInMenu());
    settings->setValue(Token + "wheel_change_size", wheel_change_size_action->isIconVisibleInMenu());
    settings->setValue(Token + "H_SValue", this->horizontalScrollBar()->value());
    settings->setValue(Token + "V_SValue", this->verticalScrollBar()->value());
    settings->setValue(Token + "html_text", this->toHtml());
    settings->setValue(Token + "selection_color", selection_color.rgba());
    settings->setValue(Token + "insert_mode_action", insert_mode_action->isIconVisibleInMenu());
    settings->setValue(Token + "search_color", search_color.rgba());
    settings->setValue(Token + "search_and_select_color", search_and_select_color.rgba());
    settings->setValue(Token + "show_line_num_action", show_line_num_action->isIconVisibleInMenu());
    settings->setValue(Token + "line_mark_color", line_mark_color.rgba());
    settings->setValue(Token + "line_text_color", line_text_color.rgba());
    settings->setValue(Token + "statusBar_text_color", statusBar_text_color.rgba());
    settings->setValue(Token + "lineArea_left", lineArea_left);
    settings->setValue(Token + "set_show_status_bar", set_show_status_bar->isIconVisibleInMenu());
    QVariantList markList;
    for (int line : markedLines)
    {
        markList.append(line);
    }
    settings->setValue(Token + "markedLines", markList);
}
void Basic_TextEdit::H_load(QSettings *settings, QString Token)
{
    bool auto_turn_line = settings->value(Token + "auto_turn_line", false).toBool();
    auto_turn_line_action->setIconVisibleInMenu(auto_turn_line);
    setWordWrapMode(auto_turn_line_action->isIconVisibleInMenu()?QTextOption::WrapMode::WrapAnywhere:QTextOption::WrapMode::NoWrap);
    bool read_only = settings->value(Token + "read_only", false).toBool();
    read_only_action->setIconVisibleInMenu(read_only);
    setReadOnly(read_only_action->isIconVisibleInMenu());
    bool center_paste = settings->value(Token + "center_paste", true).toBool();
    center_paste_action->setIconVisibleInMenu(center_paste);
    wheel_change_size_action->setIconVisibleInMenu(settings->value(Token + "wheel_change_size", true).toBool());
    this->setHtml(settings->value(Token + "html_text", "").toString());
    this->horizontalScrollBar()->setValue(settings->value(Token + "H_SValue", 0).toInt());
    this->verticalScrollBar()->setValue(settings->value(Token + "V_SValue", 0).toInt());
    selection_color = QColor::fromRgba(settings->value(Token + "selection_color", QColor(0, 100, 255, 80).rgba()).toUInt());
    bool insert_mode = settings->value(Token + "insert_mode_action", false).toBool();
    insert_mode_action->setIconVisibleInMenu(center_paste);
    setCursorWidth(insert_mode ? 8 : 2);
    search_color = QColor::fromRgba(settings->value(Token + "search_color", QColor(255, 255, 0, 255).rgba()).toUInt());
    search_and_select_color = QColor::fromRgba(settings->value(Token + "search_and_select_color", QColor(0, 129, 255, 255).rgba()).toUInt());
    show_line_num_action->setIconVisibleInMenu(settings->value(Token + "show_line_num_action", false).toBool());
    lineArea_left = settings->value(Token + "lineArea_left", true).toBool();
    set_show_line_pos_left_action->setIconVisibleInMenu(lineArea_left);
    set_show_line_pos_right_action->setIconVisibleInMenu(!lineArea_left);
    line_mark_color = QColor::fromRgba(settings->value(Token + "line_mark_color", QColor(255, 0, 0, 255).rgba()).toUInt());
    line_text_color = QColor::fromRgba(settings->value(Token + "line_text_color", QColor(30, 30, 30, 255).rgba()).toUInt());
    statusBar_text_color = QColor::fromRgba(settings->value(Token + "statusBar_text_color", QColor(30, 30, 30, 255).rgba()).toUInt());
    set_show_status_bar->setIconVisibleInMenu(settings->value(Token + "set_show_status_bar", false).toBool());
    markedLines.clear();
    QVariantList markList = settings->value(Token + "markedLines", {}).toList();
    for (const QVariant &v : markList)
    {
        bool ok = false;
        int line = v.toInt(&ok);
        if (ok)
        {
            markedLines.insert(line);
        }
    }
    if (lineNumberArea)
    {
        lineNumberArea->update();
    }
    updateStatusBar_style();
    updateLineNumberAreaWidth();
    updateStatusBar();
}
void Basic_TextEdit::H_save_no_text(QSettings *settings, QString Token)
{
    settings->setValue(Token + "auto_turn_line", auto_turn_line_action->isIconVisibleInMenu());
    settings->setValue(Token + "read_only", read_only_action->isIconVisibleInMenu());
    settings->setValue(Token + "center_paste", center_paste_action->isIconVisibleInMenu());
    settings->setValue(Token + "wheel_change_size", wheel_change_size_action->isIconVisibleInMenu());
    settings->setValue(Token + "selection_color", selection_color.rgba());
    settings->setValue(Token + "insert_mode_action", insert_mode_action->isIconVisibleInMenu());
    settings->setValue(Token + "search_color", search_color.rgba());
    settings->setValue(Token + "search_and_select_color", search_and_select_color.rgba());
    settings->setValue(Token + "show_line_num_action", show_line_num_action->isIconVisibleInMenu());
    settings->setValue(Token + "line_mark_color", line_mark_color.rgba());
    settings->setValue(Token + "line_text_color", line_text_color.rgba());
    settings->setValue(Token + "statusBar_text_color", statusBar_text_color.rgba());
    settings->setValue(Token + "lineArea_left", lineArea_left);
    settings->setValue(Token + "set_show_status_bar", set_show_status_bar->isIconVisibleInMenu());
}
void Basic_TextEdit::H_load_no_text(QSettings *settings, QString Token)
{
    bool auto_turn_line = settings->value(Token + "auto_turn_line", false).toBool();
    auto_turn_line_action->setIconVisibleInMenu(auto_turn_line);
    setWordWrapMode(auto_turn_line_action->isIconVisibleInMenu()?QTextOption::WrapMode::WrapAnywhere:QTextOption::WrapMode::NoWrap);
    bool read_only = settings->value(Token + "read_only", false).toBool();
    read_only_action->setIconVisibleInMenu(read_only);
    setReadOnly(read_only_action->isIconVisibleInMenu());
    bool center_paste = settings->value(Token + "center_paste", true).toBool();
    center_paste_action->setIconVisibleInMenu(center_paste);
    wheel_change_size_action->setIconVisibleInMenu(settings->value(Token + "wheel_change_size", true).toBool());
    selection_color = QColor::fromRgba(settings->value(Token + "selection_color", QColor(0, 100, 255, 80).rgba()).toUInt());
    bool insert_mode = settings->value(Token + "insert_mode_action", false).toBool();
    insert_mode_action->setIconVisibleInMenu(center_paste);
    setCursorWidth(insert_mode ? 8 : 2);
    search_color = QColor::fromRgba(settings->value(Token + "search_color", QColor(255, 255, 0, 255).rgba()).toUInt());
    search_and_select_color = QColor::fromRgba(settings->value(Token + "search_and_select_color", QColor(0, 129, 255, 255).rgba()).toUInt());
    show_line_num_action->setIconVisibleInMenu(settings->value(Token + "show_line_num_action", false).toBool());
    lineArea_left = settings->value(Token + "lineArea_left", true).toBool();
    set_show_line_pos_left_action->setIconVisibleInMenu(lineArea_left);
    set_show_line_pos_right_action->setIconVisibleInMenu(!lineArea_left);
    line_mark_color = QColor::fromRgba(settings->value(Token + "line_mark_color", QColor(255, 0, 0, 255).rgba()).toUInt());
    line_text_color = QColor::fromRgba(settings->value(Token + "line_text_color", QColor(30, 30, 30, 255).rgba()).toUInt());
    statusBar_text_color = QColor::fromRgba(settings->value(Token + "statusBar_text_color", QColor(30, 30, 30, 255).rgba()).toUInt());
    set_show_status_bar->setIconVisibleInMenu(settings->value(Token + "set_show_status_bar", false).toBool());
    markedLines.clear();
    if (lineNumberArea)
    {
        lineNumberArea->update();
    }
    updateStatusBar_style();
    updateLineNumberAreaWidth();
    updateStatusBar();
}
Basic_TextEdit::LineNumberArea::LineNumberArea(Basic_TextEdit *editor)
    : QWidget(editor), textEdit(editor)
{
    setStyleSheet("background-color: rgba(240,240,240,100);border-radius: 10px 10px;");
}
QSize Basic_TextEdit::LineNumberArea::sizeHint() const
{
    return QSize(textEdit->lineNumberAreaWidth(), 0);
}
void Basic_TextEdit::LineNumberArea::paintEvent(QPaintEvent *event)
{
    textEdit->lineNumberAreaPaintEvent(event);
}
void Basic_TextEdit::LineNumberArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (textEdit->show_line_num_action->isIconVisibleInMenu() && ((textEdit->lineArea_left && event->x() > width() - 16) ||(!textEdit->lineArea_left && event->x() < 16)))
    {
        QTextCursor cursor = textEdit->cursorForPosition(QPoint(1, event->y()));
        int line = cursor.block().blockNumber();
        if (textEdit->markedLines.contains(line))
        {
            textEdit->markedLines.remove(line);
        }
        else
        {
            textEdit->markedLines.insert(line);
        }
        update();
    }
    QWidget::mouseReleaseEvent(event);
}
QString SearchBar::findText() const
{
    return m_findEdit->text();
}
QString SearchBar::replaceText() const
{
    return m_replaceEdit->text();
}
void SearchBar::setfindText(QString &text)
{
    m_findEdit->setText(text);
}
SearchBar::SearchBar(Basic_TextEdit *editor, QWidget *parent)
    :QWidget(parent)
    ,m_editor(editor)
{
    setStyleSheet("QWidget{background-color: rgba(32, 32, 32, 220);border-radius: 10px;border: 1px solid rgba(80, 80, 80, 200);}"
        "QLineEdit{background-color: rgba(48, 48, 48, 240);color: #f0f0f0;border: 1px solid #5a5a5a;border-radius: 10px;padding: 4px 6px;selection-background-color: #2c7bb6;}"
        "QLineEdit:focus {border-color: #2c7bb6;}"
        "QPushButton {background-color: #3c3c3c;color: #f0f0f0;border: 1px solid #5a5a5a;border-radius: 10px;padding: 4px 8px;}"
        "QPushButton:hover {background-color: #505050;border-color: #7a7a7a;border-radius: 10px;}"
        "QPushButton:pressed {background-color: #2c7bb6;border-color: #1e5a8a;border-radius: 10px;}"
        "QCheckBox {color: #f0f0f0;spacing: 6px;border-radius: 10px;}"
        "QCheckBox::indicator {width: 16px;height: 16px;border-radius: 3px;border: 1px solid #7a7a7a;background-color: #3c3c3c;}"
        "QCheckBox::indicator:checked {background-color: #2c7bb6;border-color: #1e5a8a;}"
        "QLabel {color: #e0e0e0;border-radius: 10px;}");
    setVisible(false);
    setFocusPolicy(Qt::StrongFocus);
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    mainLayout->setSpacing(6);

    QVBoxLayout *verticalLayout = new QVBoxLayout();
    verticalLayout->setSpacing(4);
    QHBoxLayout *findRow = new QHBoxLayout();
    QLabel *findLabel = new QLabel(tr("查找:"), this);
    m_findEdit->setPlaceholderText(tr("查找内容"));
    m_findEdit->setMinimumWidth(200);
    findRow->addWidget(findLabel);
    findRow->addWidget(m_findEdit);
    findRow->addWidget(m_prevBtn);
    findRow->addWidget(m_nextBtn);
    findRow->addWidget(m_closeBtn);

    QHBoxLayout *replaceRow = new QHBoxLayout();
    QLabel *replaceLabel = new QLabel(tr("替换为:"), this);
    m_replaceEdit->setPlaceholderText(tr("替换内容"));
    m_replaceEdit->setMinimumWidth(200);
    replaceRow->addWidget(replaceLabel);
    replaceRow->addWidget(m_replaceEdit);
    replaceRow->addWidget(m_replaceBtn);
    replaceRow->addWidget(m_replaceAllBtn);
    verticalLayout->addLayout(findRow);
    verticalLayout->addLayout(replaceRow);
    QVBoxLayout *optionsRow = new QVBoxLayout();
    optionsRow->addWidget(m_caseSensitive);
    optionsRow->addWidget(m_wholeWord);

    mainLayout->addLayout(verticalLayout);
    mainLayout->addLayout(optionsRow);

    connect(m_findEdit, &QLineEdit::textChanged, this, &SearchBar::onTextChanged);
    connect(m_prevBtn, &QPushButton::clicked, this, &SearchBar::findPrevious);
    connect(m_nextBtn, &QPushButton::clicked, this, &SearchBar::findNext);
    connect(m_replaceBtn, &QPushButton::clicked, this, &SearchBar::replaceCurrent);
    connect(m_replaceAllBtn, &QPushButton::clicked, this, &SearchBar::replaceAll);
    connect(m_closeBtn, &QPushButton::clicked, this, &SearchBar::hideSearchBar);
    connect(m_caseSensitive, &QCheckBox::toggled, this, &SearchBar::onCaseSensitiveToggled);
    connect(m_wholeWord, &QCheckBox::toggled, this, &SearchBar::onWholeWordToggled);

    QRect editorRect = geometry();
    int x = editorRect.x() + editorRect.width() - 400 - 10;
    int y = 10;
    setGeometry(x, y, 400, 80);
}
void SearchBar::showSearchBar()
{
    if (m_editor->isReadOnly())
    {
        m_replaceBtn->setEnabled(false);
        m_replaceAllBtn->setEnabled(false);
    }
    else
    {
        m_replaceBtn->setEnabled(true);
        m_replaceAllBtn->setEnabled(true);
    }
    show();
    raise();
    m_findEdit->setFocus();
    m_findEdit->selectAll();
    emit visibilityChanged(true);
}
void SearchBar::hideSearchBar()
{
    hide();
    m_editor->setSearchBarVisible(false);
    emit visibilityChanged(false);
}
void SearchBar::onTextChanged(const QString &text)
{
    Q_UNUSED(text);
    if (m_editor)
    {
        m_editor->performSearch(false);
    }
}
void SearchBar::onCaseSensitiveToggled(bool checked)
{
    if (m_editor)
    {
        m_editor->setSearchOptions(checked, m_wholeWord->isChecked());
    }
}
void SearchBar::onWholeWordToggled(bool checked)
{
    if (m_editor)
    {
        m_editor->setSearchOptions(m_caseSensitive->isChecked(), checked);
    }
}
void SearchBar::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape)
    {
        hideSearchBar();
        event->accept();
    }
    else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        if (event->modifiers() & Qt::ShiftModifier)
        {
            emit findPrevious();
        }
        else
        {
            emit findNext();
        }
        event->accept();
    }
    else
    {
        QWidget::keyPressEvent(event);
    }
}
void Basic_TextEdit::setSearchOptions(bool caseSensitive, bool wholeWord)
{
    m_caseSensitive = caseSensitive;
    m_wholeWord = wholeWord;
    this->performSearch();
}
void Basic_TextEdit::performSearch(bool moveToFirst)
{
    QString searchText = getSearchText();
    m_matchCursors.clear();
    m_searchHighlights.clear();
    m_currentMatchIndex = -1;
    if (searchText.isEmpty())
    {
        mergeAndSetExtraSelections();
        return;
    }
    QTextDocument *doc = document();
    QTextCursor cursor(doc);
    QTextDocument::FindFlags flags = QTextDocument::FindFlags();
    if (m_caseSensitive)
    {
        flags |= QTextDocument::FindCaseSensitively;
    }
    if (m_wholeWord)
    {
        flags |= QTextDocument::FindWholeWords;
    }
    while (!cursor.isNull() && !cursor.atEnd())
    {
        cursor = doc->find(searchText, cursor, flags);
        if (!cursor.isNull())
        {
            QTextCursor matchCursor = cursor;
            m_matchCursors.append(matchCursor);
            QTextEdit::ExtraSelection highlight;
            highlight.cursor = matchCursor;
            highlight.format.setBackground(search_color);
            m_searchHighlights.append(highlight);
            cursor = matchCursor;
            int pos = cursor.position();
            if (pos < doc->characterCount() - 1)
            {
                cursor.setPosition(pos + 1);
            }
            else
            {
                break;
            }
        }
    }
    if (moveToFirst && !m_matchCursors.isEmpty())
    {
        m_currentMatchIndex = 0;
        QTextCursor firstMatch = m_matchCursors.first();
        setTextCursor(firstMatch);
        ensureCursorVisible();
        QTextEdit::ExtraSelection currentSelection;
        currentSelection.cursor = firstMatch;
        currentSelection.format.setBackground(search_and_select_color);
        m_searchHighlights[m_currentMatchIndex] = currentSelection;
    }
    mergeAndSetExtraSelections();
}
void Basic_TextEdit::goToNextMatch()
{
    if (m_matchCursors.isEmpty())
    {
        this->performSearch(true);
        if (m_matchCursors.isEmpty())
        {
            return;
        }
    }
    if (m_currentMatchIndex >= 0 && m_currentMatchIndex < m_searchHighlights.size())
    {
        m_searchHighlights[m_currentMatchIndex].format.setBackground(search_color);
    }
    m_currentMatchIndex++;
    if (m_currentMatchIndex >= m_matchCursors.size())
    {
        m_currentMatchIndex = 0;
    }
    QTextCursor nextMatch = m_matchCursors[m_currentMatchIndex];
    setTextCursor(nextMatch);
    ensureCursorVisible();
    if (m_currentMatchIndex >= 0 && m_currentMatchIndex < m_searchHighlights.size())
    {
        m_searchHighlights[m_currentMatchIndex].format.setBackground(search_and_select_color);
    }
    mergeAndSetExtraSelections();
}
void Basic_TextEdit::goToPrevMatch()
{
    if (m_matchCursors.isEmpty())
    {
        this->performSearch(true);
        if (m_matchCursors.isEmpty())
        {
            return;
        }
    }
    if (m_currentMatchIndex >= 0 && m_currentMatchIndex < m_searchHighlights.size())
    {
        m_searchHighlights[m_currentMatchIndex].format.setBackground(search_color);
    }
    m_currentMatchIndex--;
    if (m_currentMatchIndex < 0)
    {
        m_currentMatchIndex = m_matchCursors.size() - 1;
    }
    QTextCursor prevMatch = m_matchCursors[m_currentMatchIndex];
    setTextCursor(prevMatch);
    ensureCursorVisible();
    if (m_currentMatchIndex >= 0 && m_currentMatchIndex < m_searchHighlights.size())
    {
        m_searchHighlights[m_currentMatchIndex].format.setBackground(search_and_select_color);
    }
    mergeAndSetExtraSelections();
}
void Basic_TextEdit::replaceCurrentMatch()
{
    if (m_matchCursors.isEmpty() || m_currentMatchIndex < 0 || m_currentMatchIndex >= m_matchCursors.size())
    {
        return;
    }

    if (isReadOnly())
    {
        return;
    }
    QString replaceText = getReplaceText();
    QTextCursor cursor = m_matchCursors[m_currentMatchIndex];
    QTextCharFormat originalFormat;
    if (cursor.hasSelection())
    {
        QTextCursor formatCursor = cursor;
        formatCursor.setPosition(cursor.selectionStart() + 1);
        if (!formatCursor.atEnd())
        {
            originalFormat = formatCursor.charFormat();
        }
    }
    cursor.beginEditBlock();
    cursor.removeSelectedText();
    cursor.insertText(replaceText, originalFormat);
    cursor.endEditBlock();
    this->performSearch(false);//刷新
}
void Basic_TextEdit::replaceAllMatches()
{
    if (m_matchCursors.isEmpty())
    {
        return;
    }
    if (isReadOnly())
    {
        return;
    }
    QString replaceText = getReplaceText();
    QTextCursor cursor = textCursor();
    cursor.beginEditBlock();
    for (int i = m_matchCursors.size() - 1; i >= 0; --i)
    {
        QTextCursor matchCursor = m_matchCursors[i];
        QTextCharFormat originalFormat;
        if (matchCursor.hasSelection())
        {
            QTextCursor formatCursor = matchCursor;
            formatCursor.setPosition(matchCursor.selectionStart() + 1);
            if (!formatCursor.atEnd())
            {
                originalFormat = formatCursor.charFormat();
            }
        }
        matchCursor.removeSelectedText();
        matchCursor.insertText(replaceText, originalFormat);
    }
    cursor.endEditBlock();
    this->performSearch(false);
}
void Basic_TextEdit::setSearchBarVisible(bool visible)
{
    if (!visible)
    {
        m_searchHighlights.clear();
        m_matchCursors.clear();
        m_currentMatchIndex = -1;
        mergeAndSetExtraSelections();
    }
}
void Basic_TextEdit::mergeAndSetExtraSelections()
{
    QList<QTextEdit::ExtraSelection> allSelections = extraSelections_list;
    allSelections.append(m_searchHighlights);
    setExtraSelections(allSelections);
}
