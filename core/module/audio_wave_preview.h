#ifndef AUDIO_WAVE_PREVIEW_H
#define AUDIO_WAVE_PREVIEW_H

#include <QAudioDecoder>
struct wave_channel
{
    wave_channel(bool m_valid0 = false, bool m_valid1 = false);
    bool valid0 = false;
    bool valid1 = false;
    QVector<double> channel0;
    QVector<double> channel1;
};
class Audio_Wave_Preview
{
public:
    explicit Audio_Wave_Preview();
    virtual ~Audio_Wave_Preview();
    void setSource(const QString &fileName);
private:
    QAudioDecoder *decoder = new QAudioDecoder;
    QByteArray pcmData;
    QAudioFormat format;
protected:
    virtual void handleDecodeFinished(const wave_channel &channel_data);

};

#include <QWidget>
#include <QFileInfo>
class Audio_Wave_Widget : public QWidget, public Audio_Wave_Preview
{
    Q_OBJECT
public:
    explicit Audio_Wave_Widget(QWidget *parent);
    void resetZoom();
    void setFileInfo(QFileInfo &fileinfo);
    void setPosition(qint64 position);
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;
private:
    virtual void handleDecodeFinished(const wave_channel &channel_data) override;
    qreal scale = 1.0;
    qreal scaleFactor = 1.1;
    int offset_x = 0;
};

#endif // AUDIO_WAVE_PREVIEW_H
