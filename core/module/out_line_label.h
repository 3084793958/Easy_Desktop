#ifndef OUT_LINE_LABEL_H
#define OUT_LINE_LABEL_H

#include <QLabel>

class Out_line_Label : public QLabel
{
    Q_OBJECT
public:
    explicit Out_line_Label(QWidget *parent = nullptr);
    QColor outlineColor;
    int outlineWidth;
    QColor text_color;
private:
    void paintEvent(QPaintEvent *event);
};

#endif // OUT_LINE_LABEL_H
