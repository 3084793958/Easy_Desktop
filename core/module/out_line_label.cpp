#include "out_line_label.h"

#include <QPainter>
#include <QPainterPath>

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
