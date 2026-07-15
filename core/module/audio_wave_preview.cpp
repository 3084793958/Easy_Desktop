#include "audio_wave_preview.h"
#include <QtEndian>
#include <QPainter>
#include <QMouseEvent>
#include <math.h>
Audio_Wave_Preview::Audio_Wave_Preview()
{
    decoder->connect(decoder, &QAudioDecoder::bufferReady, decoder, [this]
    {
        QAudioBuffer buffer = decoder->read();
        if (!buffer.isValid())
        {
            return;
        }
        const QAudioFormat &fmt = buffer.format();
        sampleRate = fmt.sampleRate();
        int sampleSize = fmt.sampleSize() / 8;
        int channelCount = fmt.channelCount();
        int totalSamples = buffer.sampleCount();

        if (totalSamples == 0)
        {
            return;
        }

        QVector<float> tmp;
        tmp.reserve(totalSamples);

        if (fmt.sampleType() == QAudioFormat::SignedInt)
        {
            if (sampleSize == 1)
            {
                const qint8 *data = reinterpret_cast<const qint8*>(buffer.constData());
                for (int i = 0; i < totalSamples; ++i)
                {
                    tmp.append(data[i] / 128.0f);
                }
            }
            else if (sampleSize == 2)
            {
                const qint16 *data = reinterpret_cast<const qint16*>(buffer.constData());
                for (int i = 0; i < totalSamples; ++i)
                {
                    qint16 v = qFromLittleEndian<qint16>(data[i]);
                    tmp.append(v / 32768.0f);
                }
            }
            else if (sampleSize == 4)
            {
                const qint32 *data = reinterpret_cast<const qint32*>(buffer.constData());
                for (int i = 0; i < totalSamples; ++i)
                {
                    qint32 v = qFromLittleEndian<qint32>(data[i]);
                    tmp.append(v / 2147483648.0f);
                }
            }
        }
        else if (fmt.sampleType() == QAudioFormat::Float)
        {
            if (sampleSize == 4)
            {
                const float *data = reinterpret_cast<const float*>(buffer.constData());
                for (int i = 0; i < totalSamples; ++i)
                {
                    tmp.append(data[i]);//已在[-1,1]
                }
            }
        }

        //按声道拆分
        if (channelCount == 1)
        {
            int oldSize = m_samples.channel0.size();
            m_samples.channel0.resize(oldSize + totalSamples);
            std::copy(tmp.begin(), tmp.end(), m_samples.channel0.begin() + oldSize);
        }
        else
        {
            int half = totalSamples / channelCount;
            int old0 = m_samples.channel0.size();
            int old1 = m_samples.channel1.size();
            m_samples.channel0.resize(old0 + half);
            m_samples.channel1.resize(old1 + half);
            for (int i = 0, j = 0; i < totalSamples; i += channelCount, ++j)
            {
                m_samples.channel0[old0 + j] = tmp[i];
                m_samples.channel1[old1 + j] = tmp[i + 1];
            }
        }
    });
    decoder->connect(decoder, &QAudioDecoder::finished, decoder, [this]
    {
        m_samples.channel0.shrink_to_fit();
        m_samples.channel1.shrink_to_fit();
        handleDecodeFinished();
    });
}
Audio_Wave_Preview::~Audio_Wave_Preview()
{
    decoder->stop();
    decoder->disconnect();
    decoder->deleteLater();
    m_samples.channel0.clear();
    m_samples.channel1.clear();
    m_samples.channel0.shrink_to_fit();
    m_samples.channel1.shrink_to_fit();
}
void Audio_Wave_Preview::setSource(const QString &fileName)
{
    decoder->stop();
    m_samples.channel0.clear();
    m_samples.channel1.clear();
    m_samples.channel0.shrink_to_fit();
    m_samples.channel1.shrink_to_fit();
    decoder->setSourceFilename(fileName);
    decoder->start();
}
void Audio_Wave_Preview::handleDecodeFinished()
{
    //交给后生去做
}

Audio_Wave_Widget::Audio_Wave_Widget(QWidget *parent)
    :QWidget(parent)
    ,Audio_Wave_Preview()
{
    hide();
    setAttribute(Qt::WA_TranslucentBackground);
}
void Audio_Wave_Widget::resetZoom()
{
    scale = 1.0;
    offset_x = 0;
    m_cacheDirty = true;
    update();
}
void Audio_Wave_Widget::clear()
{
    m_samples.channel0.clear();
    m_samples.channel1.clear();
    totalFrames = 0;
    m_hasData = false;
    offset_x = 0;
    scale = 1.0;
    currentPosFrame = -1;
    m_compressFactor = 1;
    update();
}
void Audio_Wave_Widget::setType(int type, const QFileInfo &fileinfo)
{
    if (static_cast<show_data_type>(type) == m_show_data_type)
    {
        return;
    }
    if (m_show_data_type == show_data_type::Unknown)
    {
        m_show_data_type = static_cast<show_data_type>(type);
        setFileInfo(fileinfo);
        return;
    }
    m_show_data_type = static_cast<show_data_type>(type);
    scale = 1.0;
    offset_x = 0;
    m_cacheDirty = true;
    if (type == 0)
    {
        clear();
    }
    else
    {
        update();
    }
}
void Audio_Wave_Widget::save(QSettings *settings, QString Token)
{
    settings->setValue(Token + "left_color", left_color.rgba());
    settings->setValue(Token + "right_color", right_color.rgba());
    settings->setValue(Token + "max_samples_size_factor", max_samples_size_factor);
    settings->setValue(Token + "currentPos_color", currentPos_color.rgba());
}
void Audio_Wave_Widget::load(QSettings *settings, QString Token)
{
    left_color = QColor::fromRgba(settings->value(Token + "left_color", QColor(255, 176, 176, 255).rgba()).toUInt());
    right_color = QColor::fromRgba(settings->value(Token + "right_color", QColor(72, 226, 237, 255).rgba()).toUInt());
    max_samples_size_factor = settings->value(Token + "max_samples_size_factor", 2).toDouble();
    currentPos_color = QColor::fromRgba(settings->value(Token + "currentPos_color", QColor(255, 0, 0, 255).rgba()).toUInt());
}
void Audio_Wave_Widget::force_update()
{
    if (m_show_data_type == show_data_type::Unknown)
    {
        return;
    }
    m_cacheDirty = true;
    update();
}
QColor Audio_Wave_Widget::get_left_color()
{
    return left_color;
}
QColor Audio_Wave_Widget::get_right_color()
{
    return right_color;
}
double Audio_Wave_Widget::get_max_samples_size_factor()
{
    return max_samples_size_factor;
}
QColor Audio_Wave_Widget::get_currentPos_color()
{
    return currentPos_color;
}
void Audio_Wave_Widget::setFileInfo(const QFileInfo &fileinfo)
{
    if (m_show_data_type == show_data_type::Unknown)
    {
        return;
    }
    setSource(fileinfo.filePath());
}
void Audio_Wave_Widget::setPosition(qint64 position)
{
    if (sampleRate <= 0 || totalFrames == 0)
    {
        currentPosFrame = -1;
        return;
    }

    qint64 posFrame = position * sampleRate / 1000;

    if (m_compressFactor > 1)
    {
        posFrame /= m_compressFactor;
    }

    if (posFrame < 0)
    {
        posFrame = 0;
    }
    if (posFrame > totalFrames) posFrame = totalFrames;
    currentPosFrame = posFrame;

    update();
}
void Audio_Wave_Widget::handleDecodeFinished()
{
    const int MAX_SAMPLES_PER_CHANNEL = static_cast<int>(max_samples_size_factor * 1024 * 1024) / 4;

    int size0 = m_samples.channel0.size();
    int size1 = m_samples.channel1.size();
    int maxSize = qMax(size0, size1);
    m_compressFactor = 1;

    if (maxSize > MAX_SAMPLES_PER_CHANNEL)
    {
        int compress = (maxSize + MAX_SAMPLES_PER_CHANNEL - 1) / MAX_SAMPLES_PER_CHANNEL;
        m_compressFactor = compress;

        //压缩左声道
        if (size0 > 0)
        {
            QVector<float> compressed;
            compressed.reserve((size0 + compress - 1) / compress);
            for (int i = 0; i < size0; i += compress)
            {
                int end = qMin(i + compress, size0);
                float sum = 0.0f;
                float sumsq = 0.0f;
                for (int j = i; j < end; ++j)
                {
                    float v = m_samples.channel0[j];
                    sum += v * v;
                    sumsq += v;
                }
                compressed.append((sumsq > 0 ? 1.0f : -1.0f) * sqrtf(sum / (end - i)));//压成RMS变成单点数据
            }
            m_samples.channel0.swap(compressed);
            m_samples.channel0.shrink_to_fit();
        }

        //压缩右声道
        if (size1 > 0)
        {
            QVector<float> compressed;
            compressed.reserve((size1 + compress - 1) / compress);
            for (int i = 0; i < size1; i += compress)
            {
                int end = qMin(i + compress, size1);
                float sum = 0.0f;
                float sumsq = 0.0f;
                for (int j = i; j < end; ++j)
                {
                    float v = m_samples.channel1[j];
                    sum += v * v;
                    sumsq += v;
                }
                compressed.append((sumsq > 0 ? 1.0f : -1.0f) * sqrtf(sum / (end - i)));
            }
            m_samples.channel1.swap(compressed);
            m_samples.channel1.shrink_to_fit();
        }
    }

    totalFrames = qMax(m_samples.channel0.size(), m_samples.channel1.size());
    m_hasData = (totalFrames > 0);
    if (m_hasData)
    {
        offset_x = 0;
        scale = 1.0;
        currentPosFrame = -1;
        m_cacheDirty = true;
        update();
    }
}
void Audio_Wave_Widget::paintEvent(QPaintEvent *event)
{
    (void) event;
    if (m_show_data_type == show_data_type::Unknown)
    {
        return;
    }
    if (!m_hasData || totalFrames == 0)
    {
        QPainter painter(this);
        painter.setPen(Qt::black);
        painter.drawText(rect(), Qt::AlignCenter, tr("等待接受数据"));
        return;
    }
    int w = width();
    int h = height();
    if (w <= 0 || h <= 0)
    {
        return;
    }

    //检查缓存是否需要更新
    if (m_cacheDirty || w != m_lastWidth || h != m_lastHeight || !qFuzzyCompare(scale, m_lastScale) || offset_x != m_lastOffset || totalFrames != m_lastTotalFrames)
    {
        updateCache();
        m_lastWidth = w;
        m_lastHeight = h;
        m_lastScale = scale;
        m_lastOffset = offset_x;
        m_lastTotalFrames = totalFrames;
        m_cacheDirty = false;
    }

    //绘制缓存图像
    QPainter painter(this);
    if (!m_cachedPixmap.isNull() && m_cachedPixmap.size() == size())
    {
        painter.drawPixmap(0, 0, m_cachedPixmap);
    }
    //绘制播放位置线
    if (currentPosFrame >= 0 && currentPosFrame < totalFrames)
    {
        double samplesPerPixel = static_cast<double>(totalFrames) / (w * scale);
        if (samplesPerPixel < 1.0) samplesPerPixel = 1.0;
        double x = (currentPosFrame - offset_x) / samplesPerPixel;
        if (x >= 0 && x < w)
        {
            painter.setPen(currentPos_color);
            painter.drawLine(QPointF(x, 0), QPointF(x, h));
        }
    }
}
void Audio_Wave_Widget::updateCache()
{
    if (!m_hasData || totalFrames == 0)
    {
        return;
    }
    int w = width();
    int h = height();
    if (w <= 0 || h <= 0)
    {
        return;
    }
    QPixmap pixmap(w, h);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);

    double samplesPerPixel = static_cast<double>(totalFrames) / (w * scale);
    if (samplesPerPixel < 1.0)
    {
        samplesPerPixel = 1.0;
    }
    double visibleSamples = w * samplesPerPixel;
    if (visibleSamples > totalFrames)
    {
        visibleSamples = totalFrames;
    }
    int start = offset_x;
    if (start < 0)
    {
        start = 0;
    }
    if (start + visibleSamples > totalFrames)
    {
        start = static_cast<int>(totalFrames - static_cast<long long>(visibleSamples));
    }
    if (start < 0)
    {
        start = 0;
    }

    int halfHeight = h / 2;
    if (!m_samples.channel0.isEmpty())
    {
        drawChannel(painter, m_samples.channel0, start, samplesPerPixel, w, halfHeight, 0, true, left_color, m_show_data_type);
    }
    if (!m_samples.channel1.isEmpty())
    {
        drawChannel(painter, m_samples.channel1, start, samplesPerPixel, w, halfHeight, halfHeight, false, right_color, m_show_data_type);
    }
    m_cachedPixmap = pixmap;
}

void Audio_Wave_Widget::drawChannel(QPainter &painter, const QVector<float> &data, int start, double samplesPerPixel, int width, int height, int yOffset, bool up, const QColor &color, show_data_type type)
{
    if (data.isEmpty() || height <= 0)
    {
        return;
    }
    QPen pen(color, 1);
    painter.setPen(pen);

    double centerY = yOffset + height / 2.0;
    double amplitude = height / 2.0 - 2.0;//留边距

    QVector<QLineF> lines;
    lines.reserve(width);
    for (int x = 0; x < width; ++x)
    {
        int iStart = static_cast<int>(start + x * samplesPerPixel);
        int iEnd = static_cast<int>(start + (x + 1) * samplesPerPixel);
        if (iStart >= data.size())
        {
            break;
        }

        if (iEnd > data.size())
        {
            iEnd = data.size();
        }
        if (iStart == iEnd)
        {
            continue;
        }
        switch (type)
        {
        case show_data_type::RMS:
        {
            float left_rms = 0.0f;
            for (int i = iStart; i < iEnd; ++i)
            {
                left_rms += data[i] * data[i];
            }
            left_rms = sqrtf(left_rms / (iEnd - iStart));
            const float silence_threshold = 1e-6f; // -120 dB
            if (left_rms < silence_threshold) left_rms = silence_threshold;

            double yMax = 0, yMin = 0;
            if (up)
            {
                yMax = centerY - static_cast<double>(left_rms) * amplitude;
                yMin = yOffset + height;
            }
            else
            {
                yMax = centerY + static_cast<double>(left_rms) * amplitude;
                yMin = yOffset;
            }
            lines.append(QLineF(QPointF(x, yMin), QPointF(x, yMax)));
            break;
        }
        case show_data_type::dB:
        {
            float left_rms = 0.0f;
            for (int i = iStart; i < iEnd; ++i)
            {
                left_rms += data[i] * data[i];
            }
            left_rms = sqrtf(left_rms / (iEnd - iStart));
            const float silence_threshold = 1e-6f; // -120 dB
            if (left_rms < silence_threshold) left_rms = silence_threshold;

            float left_db = (20 * log10f(left_rms) + 120) / 120.0f;

            double yMax = 0, yMin = 0;
            if (up)
            {
                yMax = centerY - static_cast<double>(left_db) * amplitude;
                yMin = yOffset + height;
            }
            else
            {
                yMax = centerY + static_cast<double>(left_db) * amplitude;
                yMin = yOffset;
            }
            lines.append(QLineF(QPointF(x, yMin), QPointF(x, yMax)));
            break;
        }
        default:
        {
            float minVal = 1.0f, maxVal = -1.0f;
            for (int i = iStart; i < iEnd; ++i)
            {
                float val = data[i];
                if (val < minVal) minVal = val;
                if (val > maxVal) maxVal = val;
            }

            double yMax = 0, yMin = 0;

            if (up)
            {
                yMin = centerY - static_cast<double>(maxVal) * amplitude;
                yMax = centerY - static_cast<double>(minVal) * amplitude;
            }
            else
            {
                yMin = centerY + static_cast<double>(maxVal) * amplitude;
                yMax = centerY + static_cast<double>(minVal) * amplitude;
            }
            lines.append(QLineF(QPointF(x, yMin), QPointF(x, yMax)));
            break;
        }
        }
    }
    painter.drawLines(lines);
}
void Audio_Wave_Widget::wheelEvent(QWheelEvent *event)
{
    if (!m_hasData || totalFrames == 0)
    {
        event->ignore();
        return;
    }

    int numDegrees = event->angleDelta().y() / 8;
    int numSteps = numDegrees / 15;
    double zoomFactor = (numSteps > 0) ? scaleFactor : (1.0 / scaleFactor);

    double mouseX = event->position().x();
    double samplesPerPixel = static_cast<double>(totalFrames) / (width() * scale);
    if (samplesPerPixel < 1.0)
    {
        samplesPerPixel = 1.0;
    }
    double mouseSample = offset_x + mouseX * samplesPerPixel;
    scale *= zoomFactor;
    if (scale < 1.0)
    {
        scale = 1.0;
    }
    double newSamplesPerPixel = static_cast<double>(totalFrames) / (width() * scale);
    if (newSamplesPerPixel < 1.0)
    {
        newSamplesPerPixel = 1.0;
    }
    int newOffset = static_cast<int>(mouseSample - mouseX * newSamplesPerPixel);
    double visibleSamples = width() * newSamplesPerPixel;
    if (visibleSamples > totalFrames)
    {
        visibleSamples = totalFrames;
    }
    newOffset = qBound(0, newOffset, static_cast<int>(totalFrames - visibleSamples));
    offset_x = newOffset;
    m_cacheDirty = true;
    update();
    event->accept();
}
void Audio_Wave_Widget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_hasData)
    {
        m_dragging = true;
        m_dragStartX = event->x();
        m_dragStartOffset = offset_x;
    }
    event->accept();
}
void Audio_Wave_Widget::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && m_hasData)
    {
        int deltaX = event->x() - m_dragStartX;
        double samplesPerPixel = static_cast<double>(totalFrames) / (width() * scale);
        if (samplesPerPixel < 1.0)
        {
            samplesPerPixel = 1.0;
        }
        int deltaSamples = static_cast<int>(deltaX * samplesPerPixel);
        int newOffset = m_dragStartOffset - deltaSamples;
        double visibleSamples = width() * samplesPerPixel;
        if (visibleSamples > totalFrames)
        {
            visibleSamples = totalFrames;
        }
        newOffset = qBound(0, newOffset, static_cast<int>(totalFrames - visibleSamples));
        offset_x = newOffset;
        m_cacheDirty = true;
        update();
    }
    event->accept();
}
void Audio_Wave_Widget::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = false;
    }
    event->accept();
}
