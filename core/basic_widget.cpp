#include "basic_widget.h"
Out_line_Label::Out_line_Label(QWidget *parent)
    :QLabel(parent)
{}
void Out_line_Label::paintEvent(QPaintEvent *event)
{
    if (text().isEmpty())
    {
        QLabel::paintEvent(event);
        return;
    }
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::TextAntialiasing);
    QFont font = this->font();
    QFontMetrics fm(font);
    QStringList lines = text().split("\n");
    int totalHeight = lines.size() * fm.height();
    int maxWidth = 0;
    for (int i = 0; i < lines.size(); i++)
    {
        maxWidth = qMax(maxWidth, fm.horizontalAdvance(lines[i]));
    }
    int startY = (height() - totalHeight) / 2 + fm.ascent();
    for (int i = 0; i < lines.size(); i++)
    {
        const int textWidth = fm.horizontalAdvance(lines[i]);
        const int textX = (width() - textWidth) / 2;
        const int textY = startY + i * fm.height();
        QPainterPath path;
        path.addText(textX, textY, font, lines[i]);
        if (outlineWidth != 0)
        {
            painter.setPen(QPen(outlineColor, outlineWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.setBrush(Qt::NoBrush);
            painter.drawPath(path);
        }
        painter.setPen(Qt::NoPen);
        painter.setBrush(text_color);
        painter.drawPath(path);
    }
}
Basic_Widget::Basic_Widget(QWidget *parent)
    :Desktop_Main_MouseSig_Event(parent)
{
    setParent(parent);
    basic_control->addAction(move_to_page_action);
    basic_control->addAction(set_background_radius);
    basic_control->addAction(set_background_color);
    basic_control->addAction(show_close_button);
    close_button_pos_top_left->setIconVisibleInMenu(false);
    close_button_pos_top_left->setIcon(QIcon(":/base/this.svg"));
    close_button_pos_menu->addAction(close_button_pos_top_left);
    close_button_pos_top_right->setIconVisibleInMenu(true);
    close_button_pos_top_right->setIcon(QIcon(":/base/this.svg"));
    close_button_pos_menu->addAction(close_button_pos_top_right);
    close_button_pos_bottom_left->setIconVisibleInMenu(false);
    close_button_pos_bottom_left->setIcon(QIcon(":/base/this.svg"));
    close_button_pos_menu->addAction(close_button_pos_bottom_left);
    close_button_pos_bottom_right->setIconVisibleInMenu(false);
    close_button_pos_bottom_right->setIcon(QIcon(":/base/this.svg"));
    close_button_pos_menu->addAction(close_button_pos_bottom_right);
    basic_control->addMenu(close_button_pos_menu);

    basic_control->addAction(show_select_button);
    show_select_button->setIconVisibleInMenu(true);
    show_select_button->setIcon(QIcon(":/base/this.svg"));
    select_button_pos_top_left->setIconVisibleInMenu(true);
    select_button_pos_top_left->setIcon(QIcon(":/base/this.svg"));
    select_button_pos_menu->addAction(select_button_pos_top_left);
    select_button_pos_top_right->setIconVisibleInMenu(false);
    select_button_pos_top_right->setIcon(QIcon(":/base/this.svg"));
    select_button_pos_menu->addAction(select_button_pos_top_right);
    select_button_pos_bottom_left->setIconVisibleInMenu(false);
    select_button_pos_bottom_left->setIcon(QIcon(":/base/this.svg"));
    select_button_pos_menu->addAction(select_button_pos_bottom_left);
    select_button_pos_bottom_right->setIconVisibleInMenu(false);
    select_button_pos_bottom_right->setIcon(QIcon(":/base/this.svg"));
    select_button_pos_menu->addAction(select_button_pos_bottom_right);
    basic_control->addMenu(select_button_pos_menu);

    basic_control->addAction(set_pos_action);
    basic_control->addAction(set_size_action);
    basic_control->addAction(close_action);
    show_close_button->setIconVisibleInMenu(true);
    show_close_button->setIcon(QIcon(":/base/this.svg"));
    close_button->resize(24, 24);
    close_button->setFocusPolicy(Qt::NoFocus);
    close_button->setStyleSheet("QPushButton{border-radius:12px 12px;background:rgba(255,255,255,150)}"
                                "QPushButton:hover{border-radius:12px 12px;background:rgba(255,255,255,200)}"
                                "QPushButton:pressed{border-radius:12px 12px;background:rgba(255,255,255,150)}");
    select_button->resize(24, 24);
    select_button->setFocusPolicy(Qt::NoFocus);
    select_button->setIcon(QIcon(":/base/select_blue.svg"));
    select_button->setIconSize(QSize(24, 24));
    select_button->setStyleSheet("QPushButton{border-radius:12px 12px;background:rgba(255,255,255,0)}"
                                 "QPushButton:hover{border-radius:12px 12px;background:rgba(255,255,255,0)}"
                                 "QPushButton:pressed{border-radius:12px 12px;background:rgba(255,255,255,0)}");
    select_button->hide();
    select_button->raise();
    Update_Background();
    connect(close_button, &QPushButton::released, this, [=]
    {
        hide();
        emit close_signals();
        if (auto_close)
        {
            this->deleteLater();
        }
    });
    setMouseTracking(true);
    background->setMouseTracking(true);
    resize(200, 100);
}
Basic_Widget::~Basic_Widget()
{
    disconnect();
}
void Basic_Widget::Update_Background()
{
    background->setStyleSheet(QString("border-radius: %1px %1px;background:rgba(%2,%3,%4,%5)").arg(background_radius).arg(background_color.red()).arg(background_color.green()).arg(background_color.blue()).arg(background_color.alpha()));
}
bool Basic_Widget::set_select(bool select)
{
    if (!show_select_button->isIconVisibleInMenu())
    {
        return false;
    }
    select_tags = select;
    select_button->setVisible(select_tags);
    select_button->raise();
    return true;
}
QWidget* Basic_Widget::get_self()
{
    return background;
}
void Basic_Widget::resize(int w, int h)
{
    background->move(5, 10);
    background->resize(w, h);
    update_close_button_pos();
    select_button->raise();
    QWidget::resize(w + 15 ,h + 15);
    emit size_changed(background->size());
}
void Basic_Widget::resize(QSize size)
{
    Basic_Widget::resize(size.width(), size.height());
}
void Basic_Widget::setParent(QWidget *parent)
{
    QWidget::setParent(parent);
    if (!parent)
    {
        if (save_sig_ptr)
        {
            disconnect(this, &Basic_Widget::sig_select_mouseMoveEvent, save_sig_ptr, &Desktop_Main_MouseSig_Event::select_mouseMoveEvent);
            disconnect(this, &Basic_Widget::sig_select_mousePressEvent, save_sig_ptr, &Desktop_Main_MouseSig_Event::select_mousePressEvent);
            disconnect(this, &Basic_Widget::sig_select_mouseReleaseEvent, save_sig_ptr, &Desktop_Main_MouseSig_Event::select_mouseReleaseEvent);
        }
        save_sig_ptr = nullptr;
        return;
    }
    if (save_sig_ptr != parent->parent())
    {
        if (save_sig_ptr)
        {
            disconnect(this, &Basic_Widget::sig_select_mouseMoveEvent, save_sig_ptr, &Desktop_Main_MouseSig_Event::select_mouseMoveEvent);
            disconnect(this, &Basic_Widget::sig_select_mousePressEvent, save_sig_ptr, &Desktop_Main_MouseSig_Event::select_mousePressEvent);
            disconnect(this, &Basic_Widget::sig_select_mouseReleaseEvent, save_sig_ptr, &Desktop_Main_MouseSig_Event::select_mouseReleaseEvent);
        }
        save_sig_ptr = reinterpret_cast<Desktop_Main_MouseSig_Event *>(this->parent()->parent());
        if (save_sig_ptr)
        {
            connect(this, &Basic_Widget::sig_select_mouseMoveEvent, save_sig_ptr, &Desktop_Main_MouseSig_Event::select_mouseMoveEvent);
            connect(this, &Basic_Widget::sig_select_mousePressEvent, save_sig_ptr, &Desktop_Main_MouseSig_Event::select_mousePressEvent);
            connect(this, &Basic_Widget::sig_select_mouseReleaseEvent, save_sig_ptr, &Desktop_Main_MouseSig_Event::select_mouseReleaseEvent);
        }
    }
}
void Basic_Widget::setGeometry(QRect rect)
{
    background->resize(rect.width() - 15, rect.height() - 15);
    update_close_button_pos();
    QWidget::setGeometry(rect);
    emit size_changed(background->size());
}
void Basic_Widget::mousePressEvent(QMouseEvent *event)
{
    raise();
    close_button->raise();
    if (close_button->isVisible() && close_button->geometry().contains(event->pos()))
    {
        on_press = false;
        press_resize = false;
        return;
    }
    if (event->button() == Qt::LeftButton)
    {
        if (background->geometry().contains(event->pos()))
        {
            press_point = event->pos();
            on_press = true;
            if (select_tags) emit sig_select_mousePressEvent(event, this);
        }
        else
        {
            on_press = false;
            if (press_towards == Towards::No)
            {
                press_resize = false;
            }
            else
            {
                resize_point = event->pos();
                press_resize = true;
            }
        }
    }
}
void Basic_Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        on_press = false;
        if (select_tags) emit sig_select_mouseReleaseEvent(event, this);
        press_resize = false;
        setCursor(Qt::CursorShape::ArrowCursor);
    }
}
void Basic_Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (on_press)
    {
        if (select_tags) emit sig_select_mouseMoveEvent(event, this);
        move(this->pos() + event->pos() - press_point);
        setCursor(Qt::CursorShape::SizeAllCursor);
    }
    else
    {
        if (press_resize)
        {
            QRect new_rect = this->geometry();
            int w_line_1 = this->y() + background->y();
            int w_line_2 = this->y() + background->y() + background->height();
            int h_line_1 = this->x() + background->x();
            int h_line_2 = this->x() + background->x() + background->width();
            int m_x = this->x() + event->x();
            int m_y = this->y() + event->y();
            int delta_x;
            int delta_y;
            switch (press_towards)
            {
            case Towards::No:
            {
                break;
            }
            case Towards::Top:
            {
                delta_y = w_line_1 - m_y;
                new_rect.setY(new_rect.y() - delta_y);
                break;
            }
            case Towards::Bottom:
            {
                delta_y = m_y - w_line_2;
                new_rect.setHeight(new_rect.height() + delta_y);
                break;
            }
            case Towards::Left:
            {
                delta_x = h_line_1 - m_x;
                new_rect.setX(new_rect.x() - delta_x);
                break;
            }
            case Towards::Right:
            {
                delta_x = m_x - h_line_2;
                new_rect.setWidth(new_rect.width() + delta_x);
                break;
            }
            case Towards::Top_Left:
            {
                delta_y = w_line_1 - m_y;
                new_rect.setY(new_rect.y() - delta_y);
                delta_x = h_line_1 - m_x;
                new_rect.setX(new_rect.x() - delta_x);
                break;
            }
            case Towards::Top_Right:
            {
                delta_y = w_line_1 - m_y;
                new_rect.setY(new_rect.y() - delta_y);
                delta_x = m_x - h_line_2;
                new_rect.setWidth(new_rect.width() + delta_x);
                break;
            }
            case Towards::Bottom_Left:
            {
                delta_y = m_y - w_line_2;
                new_rect.setHeight(new_rect.height() + delta_y);
                delta_x = h_line_1 - m_x;
                new_rect.setX(new_rect.x() - delta_x);
                break;
            }
            case Towards::Bottom_Right:
            {
                delta_y = m_y - w_line_2;
                new_rect.setHeight(new_rect.height() + delta_y);
                delta_x = m_x - h_line_2;
                new_rect.setWidth(new_rect.width() + delta_x);
                break;
            }
            }
            if (new_rect.width() <= 50 && delta_x < 0)
            {
                new_rect.setX(this->geometry().x());
                new_rect.setWidth(50);
            }
            if (new_rect.height() <= 50 && delta_y < 0)
            {
                new_rect.setY(this->geometry().y());
                new_rect.setHeight(50);
            }
            this->setGeometry(new_rect);
        }
        else
        {
            if (close_button->isVisible() && close_button->geometry().contains(event->pos()))
            {
                setCursor(Qt::CursorShape::ArrowCursor);
                return;
            }
            if (background->geometry().contains(event->pos()))
            {
                setCursor(Qt::CursorShape::ArrowCursor);
                return;
            }
            press_towards = get_towards(event->pos(), background->geometry());
            switch (press_towards)
            {
            case Towards::No:
            {
                setCursor(Qt::CursorShape::ArrowCursor);
                break;
            }
            case Towards::Top:
            {
                setCursor(Qt::CursorShape::SizeVerCursor);
                break;
            }
            case Towards::Bottom:
            {
                setCursor(Qt::CursorShape::SizeVerCursor);
                break;
            }
            case Towards::Left:
            {
                setCursor(Qt::CursorShape::SizeHorCursor);
                break;
            }
            case Towards::Right:
            {
                setCursor(Qt::CursorShape::SizeHorCursor);
                break;
            }
            case Towards::Top_Left:
            {
                setCursor(Qt::CursorShape::SizeFDiagCursor);
                break;
            }
            case Towards::Top_Right:
            {
                setCursor(Qt::CursorShape::SizeBDiagCursor);
                break;
            }
            case Towards::Bottom_Left:
            {
                setCursor(Qt::CursorShape::SizeBDiagCursor);
                break;
            }
            case Towards::Bottom_Right:
            {
                setCursor(Qt::CursorShape::SizeFDiagCursor);
                break;
            }
            }
        }
    }
}
void Basic_Widget::sig_mousePressEvent(QMouseEvent *event)
{
    raise();
    close_button->raise();
    if (close_button->isVisible() && close_button->geometry().contains(event->pos()))
    {
        on_press = false;
        press_resize = false;
        return;
    }
    if (event->button() == Qt::LeftButton)
    {
        press_point = event->pos();
        on_press = true;
    }
}
void Basic_Widget::sig_mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        on_press = false;
        press_resize = false;
        setCursor(Qt::CursorShape::ArrowCursor);
    }
}
void Basic_Widget::sig_mouseMoveEvent(QMouseEvent *event)
{
    if (on_press)
    {
        move(this->pos() + event->pos() - press_point);
        setCursor(Qt::CursorShape::SizeAllCursor);
    }
}
Towards Basic_Widget::get_towards(QPoint point, QRect rect)
{
    Towards result = Towards::No;
    short x_towards = 0;
    short y_towards = 0;
    if (point.x() <= rect.x())
    {
        x_towards = -1;
    }
    else if (point.x() >= rect.x() + rect.width())
    {
        x_towards = 1;
    }
    if (point.y() <= rect.y())
    {
        y_towards = 1;
    }
    else if (point.y() >= rect.y() + rect.height())
    {
        y_towards = -1;
    }
    if (x_towards == 1)
    {
        if (y_towards == 1)
        {
            result = Towards::Top_Right;
        }
        else if (y_towards == 0)
        {
            result = Towards::Right;
        }
        else
        {
            result = Towards::Bottom_Right;
        }
    }
    else if (x_towards == -1)
    {
        if (y_towards == 1)
        {
            result = Towards::Top_Left;
        }
        else if (y_towards == 0)
        {
            result = Towards::Left;
        }
        else
        {
            result = Towards::Bottom_Left;
        }
    }
    else
    {
        if (y_towards == 1)
        {
            result = Towards::Top;
        }
        else if (y_towards == 0)
        {
            result = Towards::No;
        }
        else
        {
            result = Towards::Bottom;
        }
    }
    return result;
}
void Basic_Widget::set_now_page(int *m_now_page)
{
    now_page = m_now_page;
}
void Basic_Widget::set_desktop_number(int *m_desktop_number)
{
    desktop_number = m_desktop_number;
}
void Basic_Widget::set_basic_list(QList<QWidget *> *m_basic_list)
{
    basic_list = m_basic_list;
}
void Basic_Widget::moveToDesktop(int index)
{
    if (index < 0 || index >= *desktop_number)
    {
        return;
    }
    if (index == *now_page)
    {
        return;
    }
    if (basic_list->count() != *desktop_number)
    {
        return;
    }
    this->setParent(basic_list->at(index));
}
void Basic_Widget::basic_context(QMenu *menu)
{
    menu->addMenu(basic_control);
}
void Basic_Widget::basic_action_func(QAction *action)
{
    if (action == nullptr)
    {
        return;
    }
    if (action == move_to_page_action)
    {
        bool ok = false;
        int new_index = QInputDialog::getInt(nullptr, tr("获取数值"), tr("移动到桌面数:\n(人类索引,非计算机索引)"), *now_page + 1, 1, *desktop_number, 1, &ok);
        if (!ok)
        {
            return;
        }
        new_index--;
        moveToDesktop(new_index);
    }
    else if (action == set_background_radius)
    {
        bool ok = false;
        int new_radius = QInputDialog::getInt(nullptr, tr("获取数值"), tr("圆角大小:"), background_radius, 0, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        background_radius = new_radius;
        background->setStyleSheet(QString("border-radius: %1px %1px;background:rgba(%2,%3,%4,%5)").arg(background_radius).arg(background_color.red()).arg(background_color.green()).arg(background_color.blue()).arg(background_color.alpha()));
    }
    else if (action == set_background_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(background_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle(tr("获取颜色"));
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        QColor new_color = colorDialog.currentColor();
        background_color = new_color;
        background->setStyleSheet(QString("border-radius: %1px %1px;background:rgba(%2,%3,%4,%5)").arg(background_radius).arg(background_color.red()).arg(background_color.green()).arg(background_color.blue()).arg(background_color.alpha()));
    }
    else if (action == show_close_button)
    {
        show_close_button->setIconVisibleInMenu(!show_close_button->isIconVisibleInMenu());
        close_button->setVisible(show_close_button->isIconVisibleInMenu());
    }
    else if (action == set_pos_action)
    {
        bool ok = false;
        int new_X = QInputDialog::getInt(nullptr, tr("获取数值"), tr("X坐标:"), x(), -2147483647, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int new_Y = QInputDialog::getInt(nullptr, tr("获取数值"), tr("Y坐标:"), y(),-2147483647 , 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        move(new_X, new_Y);
    }
    else if (action == set_size_action)
    {
        bool ok = false;
        int new_X = QInputDialog::getInt(nullptr, tr("获取数值"), tr("宽度width:"), background->width(), 10, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        int new_Y = QInputDialog::getInt(nullptr, tr("获取数值"), tr("高度height:"), background->height(), 10, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        resize(new_X, new_Y);
    }
    else if (action == close_action)
    {
        hide();
        emit close_signals();
        if (auto_close)
        {
            this->deleteLater();
        }
    }
    else if (action == close_button_pos_top_left)
    {
        close_button_pos = Button_Pos::Top_Left;
        update_close_button_pos();
    }
    else if (action == close_button_pos_top_right)
    {
        close_button_pos = Button_Pos::Top_Right;
        update_close_button_pos();
    }
    else if (action == close_button_pos_bottom_left)
    {
        close_button_pos = Button_Pos::Bottom_Left;
        update_close_button_pos();
    }
    else if (action == close_button_pos_bottom_right)
    {
        close_button_pos = Button_Pos::Bottom_Right;
        update_close_button_pos();
    }
    else if (action == show_select_button)
    {
        show_select_button->setIconVisibleInMenu(!show_select_button->isIconVisibleInMenu());
        if (!show_select_button->isIconVisibleInMenu())
        {
            select_tags = false;
            select_button->setVisible(false);
            select_button->raise();
        }
    }
    else if (action == select_button_pos_top_left)
    {
        select_button_pos = Button_Pos::Top_Left;
        update_close_button_pos();
    }
    else if (action == select_button_pos_top_right)
    {
        select_button_pos = Button_Pos::Top_Right;
        update_close_button_pos();
    }
    else if (action == select_button_pos_bottom_left)
    {
        select_button_pos = Button_Pos::Bottom_Left;
        update_close_button_pos();
    }
    else if (action == select_button_pos_bottom_right)
    {
        select_button_pos = Button_Pos::Bottom_Right;
        update_close_button_pos();
    }
}
void Basic_Widget::save(QSettings *settings)
{
    settings->setValue("pos", this->pos());
    settings->setValue("size", this->get_self()->size());
    settings->setValue("show_close", show_close_button->isIconVisibleInMenu());
    settings->setValue("background_color", background_color.rgba());
    settings->setValue("background_radius", background_radius);
    int res = -1;
    for (int i = 0; i < basic_list->count(); i++)
    {
        if (this->parentWidget() != nullptr && this->parentWidget() == basic_list->at(i))
        {
            res = i;
            break;
        }
    }
    settings->setValue("in_page", res);
    settings->setValue("close_button_pos", static_cast<int>(close_button_pos));
    settings->setValue("show_select", show_select_button->isIconVisibleInMenu());
    settings->setValue("select_button_pos", static_cast<int>(select_button_pos));
}
void Basic_Widget::load(QSettings *settings)
{
    this->move(settings->value("pos", QPoint(0, 0)).toPoint());
    this->resize(settings->value("size", QSize(200, 100)).toSize());
    bool show_close_button_bool = settings->value("show_close", true).toBool();
    show_close_button->setIconVisibleInMenu(show_close_button_bool);
    close_button->setVisible(show_close_button_bool);
    background_color = QColor::fromRgba(settings->value("background_color", QColor(0,0,0,50).rgba()).toUInt());
    background_radius = settings->value("background_radius", 10).toInt();
    background->setStyleSheet(QString("border-radius: %1px %1px;background:rgba(%2,%3,%4,%5)").arg(background_radius).arg(background_color.red()).arg(background_color.green()).arg(background_color.blue()).arg(background_color.alpha()));
    int res = settings->value("in_page", -1).toInt();
    if (res >= 0)
    {
        if (res >= basic_list->count())
        {
            res = basic_list->count() - 1;
        }
        this->setParent(basic_list->at(res));
    }
    close_button_pos = static_cast<Button_Pos>(settings->value("close_button_pos", 1).toInt());
    bool show_select_button_bool = settings->value("show_select", true).toBool();
    show_select_button->setIconVisibleInMenu(show_select_button_bool);
    select_button_pos = static_cast<Button_Pos>(settings->value("select_button_pos", 0).toInt());
    update_close_button_pos();
}
void Basic_Widget::save(QSettings *settings, QString Token)
{
    settings->setValue(Token + "pos", this->pos());
    settings->setValue(Token + "size", this->get_self()->size());
    settings->setValue(Token + "show_close", show_close_button->isIconVisibleInMenu());
    settings->setValue(Token + "background_color", background_color.rgba());
    settings->setValue(Token + "background_radius", background_radius);
    int res = -1;
    for (int i = 0; i < basic_list->count(); i++)
    {
        if (this->parentWidget() != nullptr && this->parentWidget() == basic_list->at(i))
        {
            res = i;
            break;
        }
    }
    settings->setValue(Token + "in_page", res);
    settings->setValue(Token + "close_button_pos", static_cast<int>(close_button_pos));
    settings->setValue(Token + "show_select", show_select_button->isIconVisibleInMenu());
    settings->setValue(Token + "select_button_pos", static_cast<int>(select_button_pos));
}
void Basic_Widget::load(QSettings *settings, QString Token)
{
    this->move(settings->value(Token + "pos", pos()).toPoint());
    this->resize(settings->value(Token + "size", size()).toSize());
    bool show_close_button_bool = settings->value(Token + "show_close", show_close_button->isIconVisibleInMenu()).toBool();
    show_close_button->setIconVisibleInMenu(show_close_button_bool);
    close_button->setVisible(show_close_button_bool);
    background_color = QColor::fromRgba(settings->value(Token + "background_color", background_color.rgba()).toUInt());
    background_radius = settings->value(Token + "background_radius", background_radius).toInt();
    background->setStyleSheet(QString("border-radius: %1px %1px;background:rgba(%2,%3,%4,%5)").arg(background_radius).arg(background_color.red()).arg(background_color.green()).arg(background_color.blue()).arg(background_color.alpha()));
    int save_res = -1;
    for (int i = 0; i < basic_list->count(); i++)
    {
        if (this->parentWidget() != nullptr && this->parentWidget() == basic_list->at(i))
        {
            save_res = i;
            break;
        }
    }
    int res = settings->value(Token + "in_page", save_res).toInt();
    if (res >= 0)
    {
        if (res >= basic_list->count())
        {
            res = basic_list->count() - 1;
        }
        this->setParent(basic_list->at(res));
    }
    close_button_pos = static_cast<Button_Pos>(settings->value(Token + "close_button_pos", static_cast<int>(close_button_pos)).toInt());
    bool show_select_button_bool = settings->value(Token + "show_select", true).toBool();
    show_select_button->setIconVisibleInMenu(show_select_button_bool);
    select_button_pos = static_cast<Button_Pos>(settings->value(Token + "select_button_pos", 0).toInt());
    update_close_button_pos();
}
void Basic_Widget::set_icon(QString checked_icon_path)
{
    show_close_button->setIcon(QIcon(checked_icon_path));
    close_button_pos_top_left->setIcon(QIcon(checked_icon_path));
    close_button_pos_top_right->setIcon(QIcon(checked_icon_path));
    close_button_pos_bottom_left->setIcon(QIcon(checked_icon_path));
    close_button_pos_bottom_right->setIcon(QIcon(checked_icon_path));
    select_button_pos_top_left->setIcon(QIcon(checked_icon_path));
    select_button_pos_top_right->setIcon(QIcon(checked_icon_path));
    select_button_pos_bottom_left->setIcon(QIcon(checked_icon_path));
    select_button_pos_bottom_right->setIcon(QIcon(checked_icon_path));
    show_select_button->setIcon(QIcon(checked_icon_path));
}
void Basic_Widget::update_close_button_pos()
{
    QSize size = background->size();
    switch (close_button_pos)
    {
    case Button_Pos::Top_Left:
    {
        close_button_pos_top_left->setIconVisibleInMenu(true);
        close_button_pos_top_right->setIconVisibleInMenu(false);
        close_button_pos_bottom_left->setIconVisibleInMenu(false);
        close_button_pos_bottom_right->setIconVisibleInMenu(false);
        close_button->move(0, 0);
        break;
    }
    case Button_Pos::Top_Right:
    {
        close_button_pos_top_left->setIconVisibleInMenu(false);
        close_button_pos_top_right->setIconVisibleInMenu(true);
        close_button_pos_bottom_left->setIconVisibleInMenu(false);
        close_button_pos_bottom_right->setIconVisibleInMenu(false);
        close_button->move(size.width() - 9, 0);
        break;
    }
    case Button_Pos::Bottom_Left:
    {
        close_button_pos_top_left->setIconVisibleInMenu(false);
        close_button_pos_top_right->setIconVisibleInMenu(false);
        close_button_pos_bottom_left->setIconVisibleInMenu(true);
        close_button_pos_bottom_right->setIconVisibleInMenu(false);
        close_button->move(0, size.height() - 9);
        break;
    }
    case Button_Pos::Bottom_Right:
    {
        close_button_pos_top_left->setIconVisibleInMenu(false);
        close_button_pos_top_right->setIconVisibleInMenu(false);
        close_button_pos_bottom_left->setIconVisibleInMenu(false);
        close_button_pos_bottom_right->setIconVisibleInMenu(true);
        close_button->move(size.width() - 9, size.height() - 9);
        break;
    }
    }
    close_button->raise();

    switch (select_button_pos)
    {
    case Button_Pos::Top_Left:
    {
        select_button_pos_top_left->setIconVisibleInMenu(true);
        select_button_pos_top_right->setIconVisibleInMenu(false);
        select_button_pos_bottom_left->setIconVisibleInMenu(false);
        select_button_pos_bottom_right->setIconVisibleInMenu(false);
        select_button->move(0, 0);
        break;
    }
    case Button_Pos::Top_Right:
    {
        select_button_pos_top_left->setIconVisibleInMenu(false);
        select_button_pos_top_right->setIconVisibleInMenu(true);
        select_button_pos_bottom_left->setIconVisibleInMenu(false);
        select_button_pos_bottom_right->setIconVisibleInMenu(false);
        select_button->move(size.width() - 9, 0);
        break;
    }
    case Button_Pos::Bottom_Left:
    {
        select_button_pos_top_left->setIconVisibleInMenu(false);
        select_button_pos_top_right->setIconVisibleInMenu(false);
        select_button_pos_bottom_left->setIconVisibleInMenu(true);
        select_button_pos_bottom_right->setIconVisibleInMenu(false);
        select_button->move(0, size.height() - 9);
        break;
    }
    case Button_Pos::Bottom_Right:
    {
        select_button_pos_top_left->setIconVisibleInMenu(false);
        select_button_pos_top_right->setIconVisibleInMenu(false);
        select_button_pos_bottom_left->setIconVisibleInMenu(false);
        select_button_pos_bottom_right->setIconVisibleInMenu(true);
        select_button->move(size.width() - 9, size.height() - 9);
        break;
    }
    }
    select_button->raise();
}
