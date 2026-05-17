#ifdef signals
#undef signals
#endif
#include <librsvg/rsvg.h>
#include <cairo/cairo.h>
#include <cairo/cairo-ft.h>

#include "my_rsvg_support.h"
QImage my_rsvg_support::renderSvgToQImage(const QString &filePath, int width, int height)
{
    GError *error = nullptr;
    RsvgHandle *handle = nullptr;
    handle = rsvg_handle_new_from_file(filePath.toUtf8().constData(), &error);
    if (error)
    {
        g_error_free(error);
        return QImage();
    }
    RsvgDimensionData dimension;
    rsvg_handle_get_dimensions(handle, &dimension);
    int targetWidth = (width <= 0) ? dimension.width : width;
    int targetHeight = (height <= 0) ? dimension.height : height;
    QImage image(targetWidth, targetHeight, QImage::Format_ARGB32);
    image.fill(Qt::transparent);
    //Cairo
    cairo_surface_t *surface = cairo_image_surface_create_for_data(image.bits(), CAIRO_FORMAT_ARGB32, targetWidth, targetHeight, image.bytesPerLine());
    cairo_t *cr = cairo_create(surface);
    double scale_x = static_cast<double>(targetWidth) / dimension.width;
    double scale_y = static_cast<double>(targetHeight) / dimension.height;
    cairo_scale(cr, scale_x, scale_y);
    rsvg_handle_render_cairo(handle, cr);
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
    g_object_unref(handle);
    return image;
}
bool RsvgGraphicsItem::isValid() const
{
    return m_valid;
}
RsvgGraphicsItem::RsvgGraphicsItem(const QString &filePath, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_filePath(filePath), m_valid(false)
{
    GError *error = nullptr;
    RsvgHandle *handle = rsvg_handle_new_from_file(m_filePath.toUtf8().constData(), &error);
    if (!handle || error)
    {
        if (error)
        {
            g_error_free(error);
        }
        m_valid = false;
        return;
    }
    RsvgDimensionData dim;
    rsvg_handle_get_dimensions(handle, &dim);
    m_svgSize = QSizeF(dim.width, dim.height);
    g_object_unref(handle);
    renderSvg();
    m_valid = !m_image.isNull();
}
RsvgGraphicsItem::~RsvgGraphicsItem()
{}
void RsvgGraphicsItem::renderSvg()
{
    m_image = my_rsvg_support::renderSvgToQImage(m_filePath, static_cast<int>(m_svgSize.width()), static_cast<int>(m_svgSize.height()));
}
QRectF RsvgGraphicsItem::boundingRect() const
{
    return QRectF(QPointF(0, 0), m_svgSize);
}
#include <QPainter>
void RsvgGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    (void) option;
    (void) widget;
    if (!m_valid)
    {
        return;
    }
    qreal currentScale = getScaleFromPainter(painter);
    const qreal scaleTolerance = 0.05;//重渲染阈值
    if (std::abs(currentScale - m_cachedScale) > scaleTolerance)
    {
        renderSvgToCache(currentScale);
    }
    if (!m_cachedImage.isNull())
    {
        painter->drawImage(boundingRect(), m_cachedImage);
    }
}
void RsvgGraphicsItem::renderSvgToCache(qreal scale)
{
    int targetWidth = qRound(m_svgSize.width() * scale);
    int targetHeight = qRound(m_svgSize.height() * scale);
    //不管大小,直接渲染
    m_cachedImage = my_rsvg_support::renderSvgToQImage(m_filePath, targetWidth, targetHeight);
    m_cachedScale = scale;
}
#include <cmath>
qreal RsvgGraphicsItem::getScaleFromPainter(const QPainter *painter) const
{
    const QTransform &transform = painter->transform();
    qreal sx = std::hypot(transform.m11(), transform.m12());
    qreal sy = std::hypot(transform.m21(), transform.m22());
    return (sx + sy) / 2.0;
}
