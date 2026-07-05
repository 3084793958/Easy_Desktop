#include "audio_wave_preview.h"
#include <QtEndian>
wave_channel::wave_channel(bool m_valid0, bool m_valid1)
    :valid0(m_valid0)
    ,valid1(m_valid1)
{}
wave_channel extractSamples(const QByteArray &pcmData, const QAudioFormat &format)
{
    QVector<double> samples;
    if (pcmData.isEmpty() || !format.isValid())
    {
        return wave_channel(false, false);
    }

    int sampleSize = format.sampleSize() / 8;
    int channelCount = format.channelCount();
    int totalSamples = pcmData.size() / sampleSize;

    if (format.sampleType() == QAudioFormat::SignedInt)
    {
        if (sampleSize == 1)//8-bit
        {
            const qint8 *data = reinterpret_cast<const qint8*>(pcmData.constData());
            for (int i = 0; i < totalSamples; ++i)
            {
                samples.append(static_cast<double>(data[i]) / 128.0);
            }
        }
        else if (sampleSize == 2)//16-bit
        {
            const qint16 *data = reinterpret_cast<const qint16*>(pcmData.constData());
            for (int i = 0; i < totalSamples; ++i)
            {
                qint16 value = qFromLittleEndian<qint16>(data[i]);
                samples.append(static_cast<double>(value) / 32768.0);
            }
        }
        else if (sampleSize == 4)//32-bit
        {
            const qint32 *data = reinterpret_cast<const qint32*>(pcmData.constData());
            for (int i = 0; i < totalSamples; ++i) {
                qint32 value = qFromLittleEndian<qint32>(data[i]);
                samples.append(static_cast<double>(value) / 2147483648.0);
            }
        }
    }
    else if (format.sampleType() == QAudioFormat::Float)
    {
        if (sampleSize == 4)//32-bit float
        {
            const float *data = reinterpret_cast<const float*>(pcmData.constData());
            for (int i = 0; i < totalSamples; ++i)
            {
                samples.append(static_cast<double>(data[i]));//通常已在[-1, 1]范围
            }
        }
    }

    wave_channel mono_samples;
    if (channelCount == 1)
    {
        for (int i = 0; i < samples.size(); i += channelCount)
        {
            mono_samples.channel0.append(samples[i]);
        }
        mono_samples.channel1 = mono_samples.channel0;
    }
    else
    {
        for (int i = 0; i < samples.size(); i += channelCount)
        {
            mono_samples.channel0.append(samples[i]);
        }
        for (int i = 1; i < samples.size(); i += channelCount)
        {
            mono_samples.channel1.append(samples[i]);
        }
    }
    if (!mono_samples.channel0.isEmpty())
    {
        mono_samples.valid0 = true;
    }
    if (!mono_samples.channel1.isEmpty())
    {
        mono_samples.valid1 = true;
    }
    return mono_samples;
}
Audio_Wave_Preview::Audio_Wave_Preview()
{
    decoder->connect(decoder, &QAudioDecoder::bufferReady, decoder, [this]
    {
        QAudioBuffer buffer = decoder->read();
        format = buffer.format();
        pcmData.append(buffer.constData<char>(), buffer.byteCount());
    });

    decoder->connect(decoder, &QAudioDecoder::finished, decoder, [this]
    {
        Audio_Wave_Preview::handleDecodeFinished(extractSamples(pcmData, format));
    });
}
Audio_Wave_Preview::~Audio_Wave_Preview()
{
    decoder->stop();
    decoder->disconnect();
    decoder->deleteLater();
    pcmData.clear();
}
void Audio_Wave_Preview::setSource(const QString &fileName)
{
    decoder->stop();
    pcmData.clear();
    decoder->setSourceFilename(fileName);
    decoder->start();
}
void Audio_Wave_Preview::handleDecodeFinished(const wave_channel &)
{
    //交给后生去做
}

Audio_Wave_Widget::Audio_Wave_Widget(QWidget *parent)
    :QWidget(parent)
    ,Audio_Wave_Preview()
{
    ;
}
void Audio_Wave_Widget::resetZoom()
{
    ;
}
void Audio_Wave_Widget::setFileInfo(QFileInfo &fileinfo)
{
    setSource(fileinfo.filePath());
}
void Audio_Wave_Widget::setPosition(qint64 position)
{
    ;
}
