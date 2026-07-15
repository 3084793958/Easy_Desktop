#ifndef MY_TREEVIEW_DELEGATE_H
#define MY_TREEVIEW_DELEGATE_H
#include <QStyledItemDelegate>

class My_TreeView_Delegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    explicit My_TreeView_Delegate(QObject *parent, QColor *m_hover_color = nullptr, QColor *m_select_color = nullptr, int *m_radius = nullptr, QModelIndex *m_proposed_action_index = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    QColor *hover_color = nullptr;
    QColor *select_color = nullptr;
    int *radius = nullptr;
    QModelIndex *proposed_action_index = nullptr;
};

class My_TableView_Delegate : public QStyledItemDelegate
{
public:
    using QStyledItemDelegate::QStyledItemDelegate;
    explicit My_TableView_Delegate(QObject *parent, QColor *m_hover_color = nullptr, QColor *m_select_color = nullptr, int *m_radius = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    QColor *hover_color = nullptr;
    QColor *select_color = nullptr;
    int *radius = nullptr;
};

#endif // MY_TREEVIEW_DELEGATE_H
