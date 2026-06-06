#include "my_treeview_delegate.h"
#include <QPainter>
#include <QPainterPath>
#include <QTreeView>
#include <QHeaderView>

My_TreeView_Delegate::My_TreeView_Delegate(QObject *parent, QColor *m_hover_color, QColor *m_select_color, int *m_radius, QModelIndex *m_proposed_action_index)
    :QStyledItemDelegate(parent)
    ,hover_color(m_hover_color)
    ,select_color(m_select_color)
    ,radius(m_radius)
    ,proposed_action_index(m_proposed_action_index)
{}
void My_TreeView_Delegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QStyleOptionViewItem opt = option;
    if (opt.state & QStyle::State_MouseOver || opt.state & QStyle::State_Selected || (proposed_action_index && proposed_action_index->isValid()))
    {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing, true);

        int index_id = 1;
        const QWidget *widget = option.widget;
        const QTreeView *treeView = qobject_cast<const QTreeView *>(widget);
        if (!treeView)
        {
            treeView = qobject_cast<const QTreeView *>(widget->parent());
        }
        if (treeView && treeView->header())
        {
            index_id = treeView->header()->visualIndex(index.column());
        }

        if (opt.state & QStyle::State_MouseOver || (proposed_action_index->row() == index.row() && proposed_action_index->parent() == index.parent()))
        {
            QColor hoverColor(227, 242, 253, 255);
            if (hover_color) hoverColor = *hover_color;
            int Radius = 10;
            if (radius) Radius = *radius;
            painter->setBrush(hoverColor);
            painter->setPen(Qt::NoPen);
            if (index_id == 0)
            {
                QPainterPath path;
                QRectF rect = opt.rect;
                path.moveTo(rect.x() + Radius, rect.y());
                path.lineTo(rect.right(), rect.y());
                path.lineTo(rect.right(), rect.bottom());
                path.lineTo(rect.x() + Radius, rect.bottom());
                path.quadTo(rect.bottomLeft(), QPointF(rect.x(), rect.bottom() - Radius));
                path.lineTo(rect.x(), rect.y() + Radius);
                path.quadTo(rect.topLeft(), QPointF(rect.x() + Radius, rect.y()));
                path.closeSubpath();
                painter->drawPath(path);
            }
            else if (index_id == 3)
            {
                QPainterPath path;
                QRectF rect = opt.rect;
                path.moveTo(rect.topLeft());
                QPointF topRight = rect.topRight();
                path.lineTo(topRight.x() - Radius, topRight.y());
                path.quadTo(topRight, QPointF(topRight.x(), topRight.y() + Radius));
                QPointF bottomRight = rect.bottomRight();
                path.lineTo(bottomRight.x(), bottomRight.y() - Radius);
                path.quadTo(bottomRight, QPointF(bottomRight.x() - Radius, bottomRight.y()));
                path.lineTo(rect.bottomLeft());
                path.closeSubpath();
                painter->drawPath(path);
            }
            else
            {
                painter->drawRect(opt.rect);
            }
        }
        if (opt.state & QStyle::State_Selected)
        {
            QColor selectColor(0, 170, 255, 255);
            if (select_color) selectColor = *select_color;
            int Radius = 10;
            if (radius) Radius = *radius;
            painter->setBrush(selectColor);
            painter->setPen(Qt::NoPen);
            if (index_id == 0)
            {
                QPainterPath path;
                QRectF rect = opt.rect;
                path.moveTo(rect.x() + Radius, rect.y());
                path.lineTo(rect.right(), rect.y());
                path.lineTo(rect.right(), rect.bottom());
                path.lineTo(rect.x() + Radius, rect.bottom());
                path.quadTo(rect.bottomLeft(), QPointF(rect.x(), rect.bottom() - Radius));
                path.lineTo(rect.x(), rect.y() + Radius);
                path.quadTo(rect.topLeft(), QPointF(rect.x() + Radius, rect.y()));
                path.closeSubpath();
                painter->drawPath(path);
            }
            else if (index_id == 3)
            {
                QPainterPath path;
                QRectF rect = opt.rect;
                path.moveTo(rect.topLeft());
                QPointF topRight = rect.topRight();
                path.lineTo(topRight.x() - Radius, topRight.y());
                path.quadTo(topRight, QPointF(topRight.x(), topRight.y() + Radius));
                QPointF bottomRight = rect.bottomRight();
                path.lineTo(bottomRight.x(), bottomRight.y() - Radius);
                path.quadTo(bottomRight, QPointF(bottomRight.x() - Radius, bottomRight.y()));
                path.lineTo(rect.bottomLeft());
                path.closeSubpath();
                painter->drawPath(path);
            }
            else
            {
                painter->drawRect(opt.rect);
            }
        }
        opt.backgroundBrush = Qt::NoBrush;
        painter->restore();
    }
    QStyledItemDelegate::paint(painter, opt, index);
}
