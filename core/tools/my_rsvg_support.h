#ifndef MY_RSVG_SUPPORT_H
#define MY_RSVG_SUPPORT_H
#include <QImage>
#include <QString>

struct my_rsvg_support
{
public:
    static QImage renderSvgToQImage(const QString &filePath, int width = -1, int height = -1);
};

#include <QGraphicsItem>

class RsvgGraphicsItem : public QGraphicsItem
{
public:
    RsvgGraphicsItem(const QString &filePath, QGraphicsItem *parent = nullptr);
    ~RsvgGraphicsItem() override;
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    bool isValid() const;
private:
    void renderSvg();
    QString m_filePath = "";
    QImage m_image = QImage();
    bool m_valid = false;
    QSizeF m_svgSize = QSizeF(0, 0);
private:
    void renderSvgToCache(qreal scale);
    qreal getScaleFromPainter(const QPainter *painter) const;
    QImage m_cachedImage = QImage();
    qreal m_cachedScale = 1;
};

#endif // MY_RSVG_SUPPORT_H
