#include "my_clock.h"
#include <QTime>
My_Clock::My_Clock(QWidget *parent)
    :Basic_Widget(parent)
{
    menu->addAction(set_outline);
    menu->addAction(set_outline_color);
    menu->addAction(set_color);
    menu->addAction(set_font);
    Basic_Widget::basic_context(menu);
    time_label->setMouseTracking(true);
    time_label->outlineWidth = 5;
    time_label->outlineColor = Qt::GlobalColor::black;
    time_label->text_color = Qt::GlobalColor::white;
    connect(clock_timer, &QTimer::timeout, this, &My_Clock::timer_update);
    time_label->setAlignment(Qt::AlignCenter);
    time_label->setStyleSheet("background:rgba(0,0,0,0)");
    connect(this, &Basic_Widget::size_changed, this, [=](QSize size)
    {
        time_label->resize(size);
        My_Clock::auto_set_font_size();
    });
    clock_timer->start(1000 - QTime::currentTime().msec());
    time_label->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
    resize(200, 100);
    show();
}

My_Clock::~My_Clock()
{
    if (my_clock_list)
    {
        my_clock_list->removeOne(this);
    }
}
void My_Clock::timer_update()
{
    clock_timer->setInterval(1000);
    time_label->setText(QDateTime::currentDateTime().toString("hh:mm:ss"));
}
void My_Clock::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == set_outline)
    {
        bool ok = false;
        int new_out_line = QInputDialog::getInt(nullptr, "获取数值", "输入描边大小:", time_label->outlineWidth, 0, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        time_label->outlineWidth = new_out_line;
    }
    else if (know_what == set_outline_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(time_label->outlineColor);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle("获取颜色");
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        QColor new_color = colorDialog.currentColor();
        time_label->outlineColor = new_color;
    }
    else if (know_what == set_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(time_label->text_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle("获取颜色");
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        QColor new_color = colorDialog.currentColor();
        time_label->text_color = new_color;
    }
    else if (know_what == set_font)
    {
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, time_label->font(), nullptr);
        if (!ok)
        {
            return;
        }
        time_label->setFont(font);
        My_Clock::auto_set_font_size();
    }
    else
    {
        Basic_Widget::basic_action_func(know_what);
    }
}
void My_Clock::auto_set_font_size()
{
    if (time_label->text().isEmpty())
    {
        return;
    }
    QFont font = time_label->font();
    const QRect label_rect = time_label->contentsRect();
    int font_size = 1;
    int step = 5;
    while (step > 0 && font_size < 250)
    {
        font.setPointSize(font_size);
        QFontMetrics fm(font);
        QRect text_rect = fm.boundingRect(label_rect, static_cast<int>(time_label->alignment() | Qt::TextWordWrap), time_label->text());
        if (text_rect.height() <= label_rect.height() && text_rect.width() <= label_rect.width())
        {
            font_size += step;
        }
        else
        {
            font_size -= step;
            step /= 2;
        }
    }
    font.setPointSize(std::max(1, font_size - 1));
    time_label->setFont(font);
}
void My_Clock::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    settings->setValue("out_line", time_label->outlineWidth);
    settings->setValue("out_line_color", time_label->outlineColor.rgba());
    settings->setValue("color", time_label->text_color.rgba());
    settings->setValue("font", time_label->font());
}
void My_Clock::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    settings->value("out_line", time_label->outlineWidth).toInt();
    time_label->outlineColor = QColor::fromRgba(settings->value("out_line_color", QColor(0, 0, 0, 255).rgba()).toUInt());
    time_label->text_color = QColor::fromRgba(settings->value("color", QColor(255, 255, 255, 255).rgba()).toUInt());
    time_label->setFont(settings->value("font", QFontDatabase::systemFont(QFontDatabase::FixedFont)).value<QFont>());
    auto_set_font_size();
}
