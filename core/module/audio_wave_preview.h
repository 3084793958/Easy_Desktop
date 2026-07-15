#ifndef AUDIO_WAVE_PREVIEW_H
#define AUDIO_WAVE_PREVIEW_H

#include <QAudioDecoder>
struct wave_channel
{
    QVector<float> channel0;
    QVector<float> channel1;
};
class Audio_Wave_Preview
{
public:
    explicit Audio_Wave_Preview();
    virtual ~Audio_Wave_Preview();
    void setSource(const QString &fileName);
    int sampleRate = 0;
private:
    QAudioDecoder *decoder = new QAudioDecoder;
protected:
    wave_channel m_samples;
protected:
    virtual void handleDecodeFinished();

};

#include <QWidget>
#include <QFileInfo>
#include <QSettings>
#include "interfaces/file-preview/preview_file_interface.h"
class Audio_Wave_Widget : public QWidget, private Audio_Wave_Preview, public Audio_Wave_Widget_Interface
{
    Q_OBJECT
public:
    explicit Audio_Wave_Widget(QWidget *parent);
    virtual void resetZoom() override;
    virtual void setFileInfo(const QFileInfo &fileinfo) override;
    virtual void setPosition(qint64 position) override;//qint64 AKA long long int
    virtual void clear() override;
    virtual void setType(int type, const QFileInfo &fileinfo) override;
    void save(QSettings *settings, QString Token);
    void load(QSettings *settings, QString Token);
    virtual void force_update() override;
    QColor left_color = QColor(255, 176, 176, 255);
    QColor right_color = QColor(72, 226, 237, 255);
    double max_samples_size_factor = 2;
    QColor currentPos_color = QColor(255, 0, 0, 255);
    virtual QColor get_left_color() override;
    virtual QColor get_right_color() override;
    virtual double get_max_samples_size_factor() override;
    virtual QColor get_currentPos_color() override;
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    virtual void handleDecodeFinished() override;
    enum class show_data_type
    {
        Unknown,
        RMS,
        dB,
        peaks
    };
private:
    qreal scale = 1.0;
    qreal scaleFactor = 1.1;
    int offset_x = 0;
    qint64 totalFrames = 0;
    qint64 currentPosFrame = -1;

    show_data_type m_show_data_type = show_data_type::RMS;
    bool m_hasData = false;
    bool m_dragging = false;
    int m_dragStartX = 0;
    int m_dragStartOffset = 0;
    static void drawChannel(QPainter &painter, const QVector<float> &data, int start, double samplesPerPixel, int width, int height, int yOffset, bool up, const QColor &color, show_data_type type);
private:
    QPixmap m_cachedPixmap;
    bool m_cacheDirty = true;//标记是否需要更新缓存
    int m_lastWidth = 0;
    int m_lastHeight = 0;
    double m_lastScale = 1.0;
    int m_lastOffset = 0;
    qint64 m_lastTotalFrames = 0;
    bool m_hasCachedData = false;
    void updateCache();
private:
    int m_compressFactor = 1;
};

#endif // AUDIO_WAVE_PREVIEW_H
