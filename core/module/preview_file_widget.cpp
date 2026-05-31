#include "preview_file_widget.h"
#include <QSvgRenderer>
#ifdef USE_PDF
#include <QPdfPageNavigation>
#endif
#include <QtConcurrent/QtConcurrent>
#include <core/tools/my_rsvg_support.h>
Preview_File_Widget::Preview_File_Widget(QWidget *parent, QAction *m_preview_action)
    :Basic_Widget(parent)
    ,preview_action(m_preview_action)
{
    hide();
    m_textEdit->hide();
    //m_textEdit->setReadOnly(true); //玩就玩吧,反正改不了
    m_imageViewer->hide();
#ifdef USE_PDF
    m_pdfViewer->hide();
#endif
    m_videoViewer->hide();
    this->auto_close = false;
    menu->addAction(prevAction);
    menu->addAction(nextAction);
    prevButton->setEnabled(false);
    nextButton->setEnabled(false);
    menu->addSeparator();
    Ext_Preview_Plugin_Control_Menu->addAction(Ext_Preview_Plugin_Control_Add_Action);
    Ext_Preview_Plugin_Control_Menu->addAction(Ext_Preview_Plugin_Control_Set_Index_Action);
    Ext_Preview_Plugin_Control_Menu->addAction(Ext_Preview_Plugin_Control_Remove_Action);
    menu->addMenu(Ext_Preview_Plugin_Control_Menu);
    menu->addSeparator();
    textEdit_Mode_TEXT->setIcon(QIcon(":/base/this.svg"));
    textEdit_Mode_TEXT->setIconVisibleInMenu(true);
    textEdit_View_Mode_Menu->addAction(textEdit_Mode_TEXT);
    textEdit_Mode_HTML->setIcon(QIcon(":/base/this.svg"));
    textEdit_Mode_HTML->setIconVisibleInMenu(false);
    textEdit_View_Mode_Menu->addAction(textEdit_Mode_HTML);
    textEdit_Mode_MARKDOWN->setIcon(QIcon(":/base/this.svg"));
    textEdit_Mode_MARKDOWN->setIconVisibleInMenu(false);
    textEdit_View_Mode_Menu->addAction(textEdit_Mode_MARKDOWN);
    textEdit_Mode_SVG->setIcon(QIcon(":/base/this.svg"));
    textEdit_Mode_SVG->setIconVisibleInMenu(false);
    textEdit_View_Mode_Menu->addAction(textEdit_Mode_SVG);
    textEdit_Mode_HEX->setIcon(QIcon(":/base/this.svg"));
    textEdit_Mode_HEX->setIconVisibleInMenu(false);
    textEdit_View_Mode_Menu->addAction(textEdit_Mode_HEX);
    menu->addMenu(textEdit_View_Mode_Menu);
    textEdit_View_Mode_Menu->setEnabled(false);
    menu->addSeparator();
    menu->addAction(prevPage);
    menu->addAction(nextPage);
    prevPage->setEnabled(false);
    nextPage->setEnabled(false);
    menu->addSeparator();
    menu->addAction(reset_size_action);
    auto_play_action->setIcon(QIcon(":/base/this.svg"));
    auto_play_action->setIconVisibleInMenu(false);
    menu->addAction(auto_play_action);
    menu->addAction(force_read_action);
    menu->addSeparator();
    menu->addAction(play_action);
    menu->addAction(stop_action);
    play_action->setEnabled(false);
    stop_action->setEnabled(false);
    menu->addAction(media_control_action);
    media_control_action->setEnabled(false);
    Basic_Widget::basic_context(menu);
    connect(this, &Basic_Widget::close_signals, this, [=]
    {
        m_preview_action->setIconVisibleInMenu(false);
    });
    connect(this, &Basic_Widget::size_changed, this, [=](QSize size)
    {
        prevButton->move(size.width() - 75, 5);
        nextButton->move(size.width() - 40, 5);
        m_infoWidget->resize(size.width() - 20, 100);
        m_textModeCombo->move(size.width() - 80 - m_textModeCombo->width(), 7);
        m_textModeCombo->resize(m_textModeCombo->width(), 30);
        nextPageButton->move(prevButton->x() - 5 - nextPageButton->width(), 5);
        prevPageButton->move(nextPageButton->x() - 5 - prevPageButton->width(), 5);
        stopButton->move(prevButton->x() - 5 - stopButton->width(), 5);
        playButton->move(stopButton->x() - 5 - playButton->width(), 5);
        m_textEdit->resize(size - QSize(10, 150));
        m_imageViewer->resize(size - QSize(10, 150));
#ifdef USE_PDF
        m_pdfViewer->resize(size - QSize(10, 150));
#endif
        m_videoViewer->resize(size - QSize(10, 150));
        for (int i = 0; i < preview_file_plugin_list.count(); ++i)
        {
            if (preview_file_plugin_list[i])
            {
                if (preview_file_plugin_list[i]->Plugin_Version >= P_Version{0, 0, 1})
                {
                    if (preview_file_plugin_list[i]->inited)
                    {
                        if (preview_file_plugin_list[i]->previewItem())
                        {
                            preview_file_plugin_list[i]->previewItem()->resize(size - QSize(10, 150));
                        }
                    }
                }
            }
        }
    });

    prevButton->resize(30, 30);
    prevButton->setStyleSheet("QPushButton{border-radius:10px 10px;background:rgba(255,255,255,150)}"
                              "QPushButton:hover{border-radius:10px 10px;background:rgba(255,255,255,200)}"
                              "QPushButton:pressed{border-radius:10px 10px;background:rgba(255,255,255,150)}");
    nextButton->resize(30, 30);
    nextButton->setStyleSheet("QPushButton{border-radius:10px 10px;background:rgba(255,255,255,150)}"
                              "QPushButton:hover{border-radius:10px 10px;background:rgba(255,255,255,200)}"
                              "QPushButton:pressed{border-radius:10px 10px;background:rgba(255,255,255,150)}");

    prevPageButton->hide();
    prevPageButton->resize(50, 30);
    prevPageButton->setStyleSheet("QPushButton{border-radius:10px 10px;background:rgba(255,255,255,150)}"
                              "QPushButton:hover{border-radius:10px 10px;background:rgba(255,255,255,200)}"
                              "QPushButton:pressed{border-radius:10px 10px;background:rgba(255,255,255,150)}");
    nextPageButton->hide();
    nextPageButton->resize(50, 30);
    nextPageButton->setStyleSheet("QPushButton{border-radius:10px 10px;background:rgba(255,255,255,150)}"
                              "QPushButton:hover{border-radius:10px 10px;background:rgba(255,255,255,200)}"
                              "QPushButton:pressed{border-radius:10px 10px;background:rgba(255,255,255,150)}");

    playButton->hide();
    playButton->resize(50, 30);
    playButton->setStyleSheet("QPushButton{border-radius:10px 10px;background:rgba(255,255,255,150)}"
                              "QPushButton:hover{border-radius:10px 10px;background:rgba(255,255,255,200)}"
                              "QPushButton:pressed{border-radius:10px 10px;background:rgba(255,255,255,150)}");
    stopButton->hide();
    stopButton->resize(50, 30);
    stopButton->setStyleSheet("QPushButton{border-radius:10px 10px;background:rgba(255,255,255,150)}"
                              "QPushButton:hover{border-radius:10px 10px;background:rgba(255,255,255,200)}"
                              "QPushButton:pressed{border-radius:10px 10px;background:rgba(255,255,255,150)}");

    m_infoWidget->move(10, 40);
    m_infoWidget->show();

    force_read_Button->resize(100, 30);
    force_read_Button->move(5, 5);
    force_read_Button->setStyleSheet("QPushButton{border-radius:10px 10px;background:rgba(255,255,255,150)}"
                              "QPushButton:hover{border-radius:10px 10px;background:rgba(255,255,255,200)}"
                              "QPushButton:pressed{border-radius:10px 10px;background:rgba(255,255,255,150)}");

    //105
    m_textModeCombo->addItem(tr("纯文本"));
    m_textModeCombo->addItem(tr("HTML"));
    m_textModeCombo->addItem(tr("Markdown"));
    m_textModeCombo->addItem(tr("查看svg"));
    m_textModeCombo->addItem(tr("十六进制"));
    m_textModeCombo->setCurrentIndex(0);
    m_textModeCombo->hide();
    m_textEdit->move(5, 145);
    m_imageViewer->move(5, 145);
#ifdef USE_PDF
    m_pdfViewer->move(5, 145);
#endif
    m_videoViewer->move(5, 145);
    clearCurrentPreview();

    connect(prevButton, &QPushButton::clicked, this, &Preview_File_Widget::onPrevClicked);
    connect(nextButton, &QPushButton::clicked, this, &Preview_File_Widget::onNextClicked);
    connect(prevPageButton, &QPushButton::clicked, this, &Preview_File_Widget::prevPdfPage);
    connect(nextPageButton, &QPushButton::clicked, this, &Preview_File_Widget::nextPdfPage);
    connect(force_read_Button, &QPushButton::clicked, this, &Preview_File_Widget::force_read_file);
    connect(playButton, &QPushButton::clicked, this, [=]
    {
        if (m_imageViewer->isVisible())
        {
            m_imageViewer->gif_movie->setPaused(false);
        }
        if (!m_mediaPlayer->media().isNull())
        {
            if (m_mediaPlayer->state() == QMediaPlayer::State::StoppedState)
            {
                m_mediaPlayer->setMedia(QUrl::fromLocalFile(currentFileInfos[currentIndex].filePath()));
            }
            m_mediaPlayer->play();
        }
    });
    connect(stopButton, &QPushButton::clicked, this, [=]
    {
        m_imageViewer->gif_movie->setPaused(true);
        m_mediaPlayer->pause();
    });
    connect(m_textModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]
    {
        clearCurrentPreview();
        setupTextPreview(currentFileInfos[currentIndex]);
    });
    connect(media_control_action, &Media_WidgetAction::change_signals_P, this, [=](int value)
    {
        Set_Position(value);
    });
    connect(media_control_action, &Media_WidgetAction::change_signals_V, this, [=](int value)
    {
        Set_Volume(value);
    });
    connect(media_control_action, &Media_WidgetAction::change_signals_S, this, [=](int value)
    {
        Set_Speed(value);
    });
    connect(this, &Preview_File_Widget::send_position, this, [=](int value, QString text)
    {
        media_control_action->set_second(value, text);
    });
    holding_pos_timer->setInterval(50);
    connect(holding_pos_timer, &QTimer::timeout, this, [=]
    {
        if (holding_time < holding_max_time && !m_mediaPlayer->media().isNull())
        {
            holding_time++;//可能不优雅,但想不到更好的方法.positionChanged(qint64 position)并不稳定,load时无效
            if (m_mediaPlayer->duration() != 0)
            {
                m_mediaPlayer->setPosition(m_mediaPlayer->duration() * holding_value / 100);
                holding_pos_timer->stop();
                holding_time = 0;
            }
        }
        else
        {
            holding_pos_timer->stop();
            holding_time = 0;
        }
    });
    resize(350, 400);
}
void Preview_File_Widget::Set_Speed(int value)
{
    qreal result = static_cast<qreal>(value) / 100;
    m_mediaPlayer->setPlaybackRate(result);
    m_imageViewer->gif_movie->setSpeed(value);
}
void Preview_File_Widget::Set_Volume(int value)
{
    m_mediaPlayer->setVolume(value);
}
void Preview_File_Widget::Set_Position(int value)
{
    holding_value = value;
    holding_time = 0;
    holding_pos_timer->start();
    m_imageViewer->gif_movie->jumpToFrame(static_cast<int>(static_cast<double>(m_imageViewer->gif_movie->frameCount() * value) / 100));
}
Preview_File_Widget::~Preview_File_Widget()
{
    int count = preview_file_plugin_list.count();
    for (int i = 0; i < count; ++i)
    {
        if (preview_file_plugin_list[0]->Plugin_Version >= P_Version{0, 0, 1})
        {
            QPluginLoader *loader = preview_file_plugin_list[0]->your_plugin_loader;
            preview_file_plugin_list[0]->RemovePlugin();
            if (loader)
            {
                loader->unload();
                loader->deleteLater();
            }
        }
        preview_file_plugin_list.removeAt(0);
    }
}
void Preview_File_Widget::onPrevClicked()
{
    if (currentIndex > 0)
    {
        --currentIndex;
        updateCurrentPreview();
    }
    prevButton->setEnabled(currentIndex > 0);
    nextButton->setEnabled(currentIndex < currentFileInfos.size() - 1);
    prevAction->setEnabled(currentIndex > 0);
    nextAction->setEnabled(currentIndex < currentFileInfos.size() - 1);
}
void Preview_File_Widget::onNextClicked()
{
    if (currentIndex + 1 < currentFileInfos.size())
    {
        ++currentIndex;
        updateCurrentPreview();
    }
    prevButton->setEnabled(currentIndex > 0);
    nextButton->setEnabled(currentIndex < currentFileInfos.size() - 1);
    prevAction->setEnabled(currentIndex > 0);
    nextAction->setEnabled(currentIndex < currentFileInfos.size() - 1);
}
void Preview_File_Widget::save(QSettings *settings, QString Token)
{
    Basic_Widget::save(settings, Token);
    m_textEdit->H_save_no_text(settings, Token + "preview_textedit_");
    settings->setValue("auto_play_action", auto_play_action->isIconVisibleInMenu());
    QStringList plugin_path_list = {};
    for (int i = 0; i < preview_file_plugin_list.count(); ++i)
    {
        if (preview_file_plugin_list[i])
        {
            if (preview_file_plugin_list[i]->Plugin_Version >= P_Version{0, 0, 1})
            {
                if (preview_file_plugin_list[i]->inited)
                {
                    plugin_path_list << preview_file_plugin_list[i]->plugin_path;
                }
            }
        }
    }
    settings->setValue("plugin_path_list", plugin_path_list);
}
void Preview_File_Widget::load(QSettings *settings, QString Token)
{
    Basic_Widget::load(settings, Token);
    m_textEdit->H_load_no_text(settings, Token + "preview_textedit_");
    auto_play_action->setIconVisibleInMenu(settings->value("auto_play_action", false).toBool());
    QStringList plugin_path_list = settings->value("plugin_path_list").toStringList();
    for (int i = 0; i < plugin_path_list.count(); ++i)
    {
        load_plugin(plugin_path_list[i]);
    }
}
void Preview_File_Widget::set_icon(QString checked_icon_path)
{
    Basic_Widget::set_icon(checked_icon_path);//预留点,可能不会添加,这段话是在构造这个类的时候写的.
    textEdit_Mode_TEXT->setIcon(QIcon(checked_icon_path));
    textEdit_Mode_HTML->setIcon(QIcon(checked_icon_path));
    textEdit_Mode_MARKDOWN->setIcon(QIcon(checked_icon_path));
    textEdit_Mode_SVG->setIcon(QIcon(checked_icon_path));
    textEdit_Mode_HEX->setIcon(QIcon(checked_icon_path));
    auto_play_action->setIcon(QIcon(checked_icon_path));
    m_textEdit->set_icon(checked_icon_path);
}
void Preview_File_Widget::updatePreview(QStringList selectionFileList, QString parent_dir, bool force_update)
{
    if (!isVisible() && !force_update)
    {
        return;
    }
    m_parent_dir = parent_dir;
    currentFileInfos.clear();
    for (const QString &fileName : selectionFileList)
    {
        currentFileInfos.append(QFileInfo(fileName));
    }
    currentIndex = 0;
    prevButton->setEnabled(currentIndex > 0);
    nextButton->setEnabled(currentIndex < currentFileInfos.size() - 1);
    prevAction->setEnabled(currentIndex > 0);
    nextAction->setEnabled(currentIndex < currentFileInfos.size() - 1);
    updateCurrentPreview();
}
void Preview_File_Widget::updateCurrentPreview()
{
    if (!currentFileInfos.isEmpty())
    {
        if (currentIndex < 0 || currentIndex >= currentFileInfos.size()) return;
        if (previewing_file_info == currentFileInfos[currentIndex])
        {
            return;
        }
        previewing_file_info = currentFileInfos[currentIndex];
    }
    clearCurrentPreview();
    if (currentFileInfos.isEmpty())
    {
        QFileInfo dirInfo(m_parent_dir);
        m_infoWidget->Set_Data(dirInfo);
        return;
    }
    QFileInfo &info = currentFileInfos[currentIndex];
    m_infoWidget->Set_Data(info);

    for (int i = 0; i < preview_file_plugin_list.count(); ++i)
    {
        if (preview_file_plugin_list[i])
        {
            if (preview_file_plugin_list[i]->Plugin_Version >= P_Version{0, 0, 1})
            {
                if (preview_file_plugin_list[i]->inited)
                {
                    if (preview_file_plugin_list[i]->previewFile(info))
                    {
                        if (preview_file_plugin_list[i]->previewItem())
                        {
                            preview_file_plugin_list[i]->previewItem()->setParent(this->get_self());
                            preview_file_plugin_list[i]->previewItem()->move(5, 145);
                            preview_file_plugin_list[i]->previewItem()->resize(this->get_self()->size() - QSize(10, 150));
                            preview_file_plugin_list[i]->previewItem()->show();
                        }
                        if (!preview_file_plugin_list[i]->willThrowFileToNextPreview())
                        {
                            return;
                        }
                    }
                }
            }
        }
    }

    ContentType type = getContentType(info);
    textEdit_View_Mode_Menu->setEnabled(type == ContentType::TypeText);
    prevPage->setEnabled(false);
    nextPage->setEnabled(false);
    switch (type)
    {
    case ContentType::TypeText:
    {
        setupTextPreview(info);
        break;
    }
    case ContentType::TypeImage:
    {
        setupImagePreview(info);
        break;
    }
    case ContentType::TypeVideo:
    {
        setupVideoPreview(info);
        break;
    }
    case ContentType::TypeAudio:
    {
        setupAudioPreview(info);
        break;
    }
    case ContentType::TypePdf:
    {
#ifdef USE_PDF
        setupPdfPreview(info);
#endif
        break;
    }
    case ContentType::TypeFolder:
    {
        break;
    }
    case ContentType::TypeFont:
    {
        setupFontPreview(info);
        break;
    }
    default:
    {
        break;
    }
    }
}
void Preview_File_Widget::clearCurrentPreview()
{
    media_control_action->setEnabled(false);
    m_textEdit->hide();
    m_textEdit->clear();
    m_textModeCombo->hide();
    //此处本应将m_textModeCombo的currentIndex设为0,但考虑到连续性,故先在此放置标记
    m_imageViewer->hide();
    m_imageViewer->clear();
#ifdef USE_PDF
    m_pdfViewer->hide();
    m_pdfDocument->close();
    m_pdfViewer->setDocument(nullptr);
#endif
    prevPageButton->hide();
    nextPageButton->hide();
    prevPage->setEnabled(false);
    nextPage->setEnabled(false);
    play_action->setEnabled(false);
    stop_action->setEnabled(false);
    playButton->hide();
    stopButton->hide();
    m_mediaPlayer->stop();
    m_mediaPlayer->setMedia(nullptr);
    m_mediaPlayer->disconnect();

    m_videoViewer->hide();
    m_videoViewer->clear();

    force_read_Button->setEnabled(false);
    force_read_action->setEnabled(false);
    force_read_Button->hide();
    if (!currentFileInfos.isEmpty())
    {
        if (currentFileInfos[currentIndex].isFile() && getContentType(currentFileInfos[currentIndex]) != ContentType::TypeText)
        {
            force_read_Button->setEnabled(true);
            force_read_action->setEnabled(true);
            force_read_Button->show();
        }
    }
    if (m_currentFontId != -1)
    {
        QFontDatabase::removeApplicationFont(m_currentFontId);
        m_currentFontId = -1;
        m_textEdit->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    }
    for (int i = 0; i < preview_file_plugin_list.count(); ++i)
    {
        if (preview_file_plugin_list[i])
        {
            if (preview_file_plugin_list[i]->Plugin_Version >= P_Version{0, 0, 1})
            {
                if (preview_file_plugin_list[i]->inited)
                {
                    if (preview_file_plugin_list[i]->previewItem())
                    {
                        preview_file_plugin_list[i]->previewItem()->hide();
                    }
                    preview_file_plugin_list[i]->clear();
                }
            }
        }
    }
}
void Preview_File_Widget::setupTextPreview(const QFileInfo &info)
{
    m_textEdit->clear();
    QFile file(info.filePath());
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        QString text = stream.readAll();
        switch (m_textModeCombo->currentIndex())
        {
        case 0:
        default:
        {
            m_textEdit->setPlainText(text);
            break;
        }
        case 1:
        {
            m_textEdit->setHtml(text);
            break;
        }
        case 2:
        {
            m_textEdit->setMarkdown(text);
            break;
        }
        case 3:
        {
            setupImagePreview(info);
            m_textModeCombo->show();
            return;
            break;
        }
        case 4:
        {
            file.close();
            if (file.open(QIODevice::ReadOnly))
            {
                QByteArray data = file.readAll();
                QString hexText;
                const int bytesPerLine = 16;
                for (int i = 0; i < data.size(); i += bytesPerLine)
                {
                    QString hexLine;
                    QString asciiLine;
                    for (int j = 0; j < bytesPerLine && i + j < data.size(); ++j)
                    {
                        unsigned char byte = static_cast<unsigned char>(data[i + j]);
                        hexLine.append(QString("%1 ").arg(byte, 2, 16, QChar('0')));
                        if (byte >= 0x20 && byte <= 0x7E)
                        {
                            asciiLine.append(static_cast<char>(byte));
                        }
                        else
                        {
                            asciiLine.append('.');
                        }
                    }
                    hexLine = hexLine.leftJustified(bytesPerLine * 3, ' ');
                    hexText += QString("%1: %2 | %3\n").arg(i, 8, 16, QChar('0')).arg(hexLine).arg(asciiLine);
                }
                m_textEdit->setPlainText(hexText);
            }
            else
            {
                m_textEdit->setPlainText(tr("无法读取文件:%1").arg(file.errorString()));
            }
        }
        }
        file.close();
    }
    else
    {
        m_textEdit->setPlainText(tr("无法读取文件:%1").arg(file.errorString()));
    }
    m_textEdit->textCursor().setPosition(0);
    m_textModeCombo->show();
    m_textEdit->show();
    m_textEdit->updateStatusBar_style();
    m_textEdit->updateLineNumberAreaWidth();
    m_textEdit->updateStatusBar();
}
void Preview_File_Widget::setupPdfPreview(const QFileInfo &info)
{
#ifdef USE_PDF
    prevPageButton->show();
    nextPageButton->show();
    m_pdfDocument->load(info.filePath());
    if (m_pdfDocument->pageCount() == 0)
    {
        m_textEdit->setPlainText(tr("无法加载 PDF文件或文件为空"));
        m_textEdit->show();
        return;
    }
    pdf_currentIndex = 0;
    QSize pageSize = m_pdfDocument->pageSize(0).toSize();
    if (pageSize.isEmpty())
    {
        pageSize = QSize(500, 500);
    }
    m_pdfViewer->setDocument(m_pdfDocument);
    m_pdfViewer->setZoomFactor(GraphicsViewer::get_scaled(pageSize, m_pdfViewer->size()));
    pdf_currentIndex = 0;
    prevPage->setEnabled(pdf_currentIndex > 0);
    nextPage->setEnabled(pdf_currentIndex < m_pdfDocument->pageCount() - 1);
    prevPageButton->setEnabled(pdf_currentIndex > 0);
    nextPageButton->setEnabled(pdf_currentIndex < m_pdfDocument->pageCount() - 1);
    connect(m_pdfViewer->pageNavigation(), &QPdfPageNavigation::currentPageChanged, this, [=]
    {
        pdf_currentIndex = m_pdfViewer->pageNavigation()->currentPage();
        prevPage->setEnabled(pdf_currentIndex > 0);
        nextPage->setEnabled(pdf_currentIndex < m_pdfDocument->pageCount() - 1);
        prevPageButton->setEnabled(pdf_currentIndex > 0);
        nextPageButton->setEnabled(pdf_currentIndex < m_pdfDocument->pageCount() - 1);
    });
    m_pdfViewer->show();
#else
    (void) info;
#endif
}
void Preview_File_Widget::setupImagePreview(const QFileInfo &info)
{
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(info);
    QString mimeName = mime.name();
    if (mimeName == "image/svg+xml")
    {
        setupSvgPreview(info);
        return;
    }
    if (mimeName == "image/gif")
    {
        play_action->setEnabled(true);
        stop_action->setEnabled(true);
        playButton->show();
        stopButton->show();
        media_control_action->setEnabled(true);
        m_imageViewer->setGif(info);
        m_imageViewer->show();
        return;
    }
    QImageReader reader(info.filePath());
    QImage image = reader.read();
    if (image.isNull())
    {
        m_textEdit->setPlainText(tr("无法加载图片:%1").arg(reader.errorString()));
        m_textEdit->show();
        return;
    }
    m_imageViewer->setImage(image);
    m_imageViewer->show();
}
void Preview_File_Widget::setupVideoPreview(const QFileInfo &info)
{
    play_action->setEnabled(true);
    stop_action->setEnabled(true);
    playButton->show();
    stopButton->show();
    media_control_action->setEnabled(true);
    m_mediaPlayer->stop();
    m_mediaPlayer->setMedia(nullptr);
    m_mediaPlayer->setMedia(QUrl::fromLocalFile(info.filePath()));
    auto video_item = new QGraphicsVideoItem;
    m_mediaPlayer->setVideoOutput(video_item);
    m_videoViewer->clear();
    m_videoViewer->setGraphicsItem(video_item);
    m_videoViewer->show();
    m_mediaPlayer->setPosition(0);
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, [=](qint64 position)
    {
        qint64 all_position = m_mediaPlayer->duration();
        if (all_position <= 0)
        {
            return;
        }
        int result = static_cast<int>(position * 100 / all_position);
        position /= 1000;//mSec
        int sec = position % 60;
        int min = static_cast<int>(position / 60);
        QString text;
        if (min < 10)
        {
            text = "0";
        }
        text += QString::number(min);
        text += ":";
        if (sec < 10)
        {
            text += "0";
        }
        text += QString::number(sec);
        emit send_position(result, text);
    });
    connect(m_mediaPlayer, &QMediaPlayer::mediaStatusChanged, this, [=](QMediaPlayer::MediaStatus status)
    {
        if (status == QMediaPlayer::LoadedMedia)
        {
            m_mediaPlayer->setPosition(0);
            m_mediaPlayer->pause();
            m_videoViewer->resetZoom();
        }
        else if (status == QMediaPlayer::EndOfMedia)
        {
            m_mediaPlayer->setMedia(QUrl::fromLocalFile(info.filePath()));
        }
    });
    if (m_mediaPlayer->mediaStatus() == QMediaPlayer::LoadedMedia)
    {
        m_mediaPlayer->setPosition(0);
        m_mediaPlayer->pause();
    }
    if (auto_play_action->isIconVisibleInMenu())
    {
        m_mediaPlayer->play();
    }
}
void Preview_File_Widget::setupAudioPreview(const QFileInfo &info)
{
    play_action->setEnabled(true);
    stop_action->setEnabled(true);
    playButton->show();
    stopButton->show();
    media_control_action->setEnabled(true);
    m_mediaPlayer->stop();
    m_mediaPlayer->setMedia(QUrl::fromLocalFile(info.filePath()));
    connect(m_mediaPlayer, &QMediaPlayer::positionChanged, this, [=](qint64 position)
    {
        qint64 all_position = m_mediaPlayer->duration();
        if (all_position <= 0)
        {
            return;
        }
        int result = static_cast<int>(position * 100 / all_position);
        position /= 1000;//mSec
        int sec = position % 60;
        int min = static_cast<int>(position / 60);
        QString text;
        if (min < 10)
        {
            text = "0";
        }
        text += QString::number(min);
        text += ":";
        if (sec < 10)
        {
            text += "0";
        }
        text += QString::number(sec);
        emit send_position(result, text);
    });
    if (auto_play_action->isIconVisibleInMenu())
    {
        m_mediaPlayer->play();
    }
}
void Preview_File_Widget::setupSvgPreview(const QFileInfo &info)
{
    auto Item = new RsvgGraphicsItem(info.filePath());
    if (!Item->isValid())
    {
        delete Item;
        Item = nullptr;
        m_textEdit->setPlainText(tr("无法加载SVG文件:%1").arg(info.fileName()));
        m_textEdit->show();
        return;
    }
    m_imageViewer->setGraphicsItem(Item);
    m_imageViewer->show();
}
void Preview_File_Widget::setupFontPreview(const QFileInfo &info)
{
    clearCurrentPreview();
    m_textEdit->clear();
    m_textEdit->show();
    int fontId = QFontDatabase::addApplicationFont(info.filePath());
    if (fontId == -1)
    {
        m_textEdit->setPlainText(tr("无法加载字体文件"));
        return;
    }
    m_currentFontId = fontId;
    QStringList families = QFontDatabase::applicationFontFamilies(fontId);
    if (families.isEmpty())
    {
        m_textEdit->setPlainText(tr("字体文件未包含任何有效字体族"));
        return;
    }
    QString primaryFamily = families.first();
    QTextCursor cursor = m_textEdit->textCursor();
    QTextCharFormat defaultFormat;
    QFont sampleFont(primaryFamily);
    defaultFormat.setFont(sampleFont);
    cursor.insertText(tr("文件名: %1\n").arg(info.fileName()), defaultFormat);
    cursor.insertText(tr("文件大小: %1\n").arg(Info_Widget::formatSize(info.size())), defaultFormat);
    cursor.insertText(tr("字体族名称: %1\n").arg(primaryFamily), defaultFormat);
    if (families.size() > 1)
    {
        QString allFamilies = families.join(", ");
        cursor.insertText(tr("其他族名: %1\n").arg(allFamilies), defaultFormat);
    }
    QFontDatabase fontDb;
    QStringList styles = fontDb.styles(primaryFamily);
    if (!styles.isEmpty())
    {
        cursor.insertText(tr("可用样式: %1\n").arg(styles.join(", ")), defaultFormat);
    }
    cursor.insertText(tr("示例:\n"
                         "void Info_Widget::updateFolderSize()"
                         "\n" "{"
                         "\n" "    if (!this->isVisible())"
                         "\n" "    {"
                         "\n" "        if (m_futureWatcher->isRunning())"
                         "\n" "        {"
                         "\n" "            m_cancelCalculation = true;"
                         "\n" "            m_futureWatcher->setPaused(true);"
                         "\n" "            m_futureWatcher->cancel();"
                         "\n" "            m_futureWatcher->future().cancel();"
                         "\n" "        }"
                         "\n" "        return;"
                         "\n" "    }"
                         "\n" "    if (m_currentDirPath.isEmpty())"
                         "\n" "    {"
                         "\n" "        return;"
                         "\n" "    }"
                         "\n" "    m_cancelCalculation = false;"
                         "\n" "    // 异步计算"
                         "\n" "    QFuture<qint64> future = QtConcurrent::run([this]() -> qint64"
                         "\n" "    {"
                         "\n" "        temp_folder_total_size = 0;"
                         "\n" "        QDirIterator it(m_currentDirPath, QDir::Files | QDir::Hidden | QDir::NoSymLinks, QDirIterator::Subdirectories);"
                         "\n" "        while (it.hasNext())"
                         "\n" "        {"
                         "\n" "            if (m_cancelCalculation.load())"
                         "\n" "            {"
                         "\n" "                return temp_folder_total_size;"
                         "\n" "            }"
                         "\n" "            it.next();//QDirIterator特性"
                         "\n" "            temp_folder_total_size += it.fileInfo().size();"
                         "\n" "        }"
                         "\n" "        return temp_folder_total_size;"
                         "\n" "    });"
                         "\n" "    m_futureWatcher->setFuture(future);"
                         "\n" "}"), defaultFormat);
    cursor.insertBlock();
    cursor.setPosition(0);
    m_textEdit->setTextCursor(cursor);
    m_textEdit->updateStatusBar_style();
    m_textEdit->updateLineNumberAreaWidth();
    m_textEdit->updateStatusBar();
}
void Preview_File_Widget::resizeEvent(QResizeEvent *event)
{
    Basic_Widget::resizeEvent(event);
}
void Preview_File_Widget::showEvent(QShowEvent *event)
{
    Basic_Widget::showEvent(event);
}
void Preview_File_Widget::hideEvent(QHideEvent *event)
{
    clearCurrentPreview();
    Basic_Widget::hideEvent(event);
}
void Preview_File_Widget::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == prevAction)
    {
        onPrevClicked();
    }
    else if (know_what == nextAction)
    {
        onNextClicked();
    }
    else if (know_what == textEdit_Mode_TEXT)
    {
        textEdit_Mode_TEXT->setIconVisibleInMenu(true);
        textEdit_Mode_HTML->setIconVisibleInMenu(false);
        textEdit_Mode_MARKDOWN->setIconVisibleInMenu(false);
        textEdit_Mode_SVG->setIconVisibleInMenu(false);
        textEdit_Mode_HEX->setIconVisibleInMenu(false);
        m_textModeCombo->setCurrentIndex(0);
        clearCurrentPreview();
        setupTextPreview(currentFileInfos[currentIndex]);
    }
    else if (know_what == textEdit_Mode_HTML)
    {
        textEdit_Mode_TEXT->setIconVisibleInMenu(false);
        textEdit_Mode_HTML->setIconVisibleInMenu(true);
        textEdit_Mode_MARKDOWN->setIconVisibleInMenu(false);
        textEdit_Mode_SVG->setIconVisibleInMenu(false);
        textEdit_Mode_HEX->setIconVisibleInMenu(false);
        m_textModeCombo->setCurrentIndex(1);
        clearCurrentPreview();
        setupTextPreview(currentFileInfos[currentIndex]);
    }
    else if (know_what == textEdit_Mode_MARKDOWN)
    {
        textEdit_Mode_TEXT->setIconVisibleInMenu(false);
        textEdit_Mode_HTML->setIconVisibleInMenu(false);
        textEdit_Mode_MARKDOWN->setIconVisibleInMenu(true);
        textEdit_Mode_SVG->setIconVisibleInMenu(false);
        textEdit_Mode_HEX->setIconVisibleInMenu(false);
        m_textModeCombo->setCurrentIndex(2);
        clearCurrentPreview();
        setupTextPreview(currentFileInfos[currentIndex]);
    }
    else if (know_what == textEdit_Mode_SVG)
    {
        textEdit_Mode_TEXT->setIconVisibleInMenu(false);
        textEdit_Mode_HTML->setIconVisibleInMenu(false);
        textEdit_Mode_MARKDOWN->setIconVisibleInMenu(false);
        textEdit_Mode_SVG->setIconVisibleInMenu(true);
        textEdit_Mode_HEX->setIconVisibleInMenu(false);
        m_textModeCombo->setCurrentIndex(3);
        clearCurrentPreview();
        setupTextPreview(currentFileInfos[currentIndex]);
    }
    else if (know_what == textEdit_Mode_HEX)
    {
        textEdit_Mode_TEXT->setIconVisibleInMenu(false);
        textEdit_Mode_HTML->setIconVisibleInMenu(false);
        textEdit_Mode_MARKDOWN->setIconVisibleInMenu(false);
        textEdit_Mode_SVG->setIconVisibleInMenu(false);
        textEdit_Mode_HEX->setIconVisibleInMenu(true);
        m_textModeCombo->setCurrentIndex(4);
        clearCurrentPreview();
        setupTextPreview(currentFileInfos[currentIndex]);
    }
    else if (know_what == prevPage)
    {
        prevPdfPage();
    }
    else if (know_what == nextPage)
    {
        nextPdfPage();
    }
    else if (know_what == play_action)
    {
        if (m_imageViewer->isVisible())
        {
            m_imageViewer->gif_movie->setPaused(false);
        }
        if (!m_mediaPlayer->media().isNull())
        {
            m_mediaPlayer->play();
        }
    }
    else if (know_what == stop_action)
    {
        m_imageViewer->gif_movie->setPaused(true);
        m_mediaPlayer->pause();
    }
    else if (know_what == reset_size_action)
    {
        if (m_imageViewer->isVisible())
        {
            m_imageViewer->resetZoom();
        }
#ifdef USE_PDF
        else if (m_pdfViewer->isVisible())
        {
            m_pdfViewer->resetZoom();
        }
#endif
        else if (m_videoViewer->isVisible())
        {
            m_videoViewer->resetZoom();
        }
        for (int i = 0; i < preview_file_plugin_list.count(); ++i)
        {
            if (preview_file_plugin_list[i])
            {
                if (preview_file_plugin_list[i]->Plugin_Version >= P_Version{0, 0, 1})
                {
                    if (preview_file_plugin_list[i]->inited)
                    {
                        preview_file_plugin_list[i]->resetZoom();
                    }
                }

            }
        }
    }
    else if (know_what == auto_play_action)
    {
        auto_play_action->setIconVisibleInMenu(!auto_play_action->isIconVisibleInMenu());
    }
    else if (know_what == force_read_action)
    {
        Preview_File_Widget::force_read_file();
    }
    else if (know_what == Ext_Preview_Plugin_Control_Add_Action)
    {
        QString plugin_filename = QFileDialog::getOpenFileName(nullptr, tr("获取插件"), QDir::homePath(), tr("插件") + "(*.so);;" + tr("所有文件") + "(*.*)");
        My_X11_Libs::X11_Raise();
        if (!plugin_filename.isEmpty())
        {
            QFileInfo fileinfo(plugin_filename);
            if (fileinfo.isReadable())
            {
                load_plugin(plugin_filename);
            }
        }
    }
    else if (know_what == Ext_Preview_Plugin_Control_Set_Index_Action)
    {
        if (preview_file_plugin_list.isEmpty())
        {
            return;
        }
        QStringList items = {};
        for (int i = 0; i < preview_file_plugin_list.count(); ++i)
        {
            QString name = "[无效项]";
            if (preview_file_plugin_list[i])
            {
                if (preview_file_plugin_list[i]->Plugin_Version >= P_Version{0, 0, 1})
                {
                    if (preview_file_plugin_list[i]->inited)
                    {
                        name = preview_file_plugin_list[i]->Plugin_Name();
                    }
                }
            }
            name += "[" + QString::number(i) + "]";
            items << name;
        }
        bool ok = false;
        QString selected = QInputDialog::getItem(nullptr, tr("移动插件"), tr("请选择要移动的插件:"), items, 0, false, &ok);
        if (ok)
        {
            int remove_index = items.indexOf(selected);
            ok = false;
            int move_to_index = QInputDialog::getInt(nullptr, tr("移动插件"), tr("移动到索引(0始):"), remove_index, 0, preview_file_plugin_list.count() - 1, 1, &ok);
            if (ok && remove_index != move_to_index)
            {
                preview_file_plugin_list.move(remove_index, move_to_index);
            }
        }
    }
    else if (know_what == Ext_Preview_Plugin_Control_Remove_Action)
    {
        if (preview_file_plugin_list.isEmpty())
        {
            return;
        }
        QStringList items = {};
        for (int i = 0; i < preview_file_plugin_list.count(); ++i)
        {
            QString name = "[无效项]";
            if (preview_file_plugin_list[i])
            {
                if (preview_file_plugin_list[i]->Plugin_Version >= P_Version{0, 0, 1})
                {
                    if (preview_file_plugin_list[i]->inited)
                    {
                        name = preview_file_plugin_list[i]->Plugin_Name();
                    }
                }
            }
            name += "[" + QString::number(i) + "]";
            items << name;
        }
        bool ok = false;
        QString selected = QInputDialog::getItem(this, tr("移除插件"), tr("请选择要移除的插件:"), items, 0, false, &ok);
        if (ok)
        {
            int remove_index = items.indexOf(selected);
            if (preview_file_plugin_list[remove_index]->Plugin_Version >= P_Version{0, 0, 1})
            {
                QPluginLoader *loader = preview_file_plugin_list[remove_index]->your_plugin_loader;
                preview_file_plugin_list[remove_index]->RemovePlugin();
                if (loader)
                {
                    loader->unload();
                    loader->deleteLater();
                }
            }
            preview_file_plugin_list.removeAt(remove_index);
        }
    }
    else
    {
        Basic_Widget::basic_action_func(know_what);
    }
}
void Preview_File_Widget::prevPdfPage()
{
#ifdef USE_PDF
    m_pdfViewer->pageNavigation()->goToPreviousPage();
    pdf_currentIndex = m_pdfViewer->pageNavigation()->currentPage();
    prevPage->setEnabled(pdf_currentIndex > 0);
    nextPage->setEnabled(pdf_currentIndex < m_pdfDocument->pageCount() - 1);
    prevPageButton->setEnabled(pdf_currentIndex > 0);
    nextPageButton->setEnabled(pdf_currentIndex < m_pdfDocument->pageCount() - 1);
#endif
}
void Preview_File_Widget::nextPdfPage()
{
#ifdef USE_PDF
    m_pdfViewer->pageNavigation()->goToNextPage();
    pdf_currentIndex = m_pdfViewer->pageNavigation()->currentPage();
    prevPage->setEnabled(pdf_currentIndex > 0);
    nextPage->setEnabled(pdf_currentIndex < m_pdfDocument->pageCount() - 1);
    prevPageButton->setEnabled(pdf_currentIndex > 0);
    nextPageButton->setEnabled(pdf_currentIndex < m_pdfDocument->pageCount() - 1);
#endif
}
void Preview_File_Widget::force_read_file()
{
    if (currentFileInfos.isEmpty())
    {
        return;
    }
    if (currentFileInfos[currentIndex].isDir())
    {
        return;
    }
    QMessageBox::StandardButton _ans = QMessageBox::question(nullptr, tr("是否强制读取?"), tr("是否强制读取?"));
    if (_ans == QMessageBox::Yes)
    {
        m_textModeCombo->setCurrentIndex(4);
        clearCurrentPreview();
        QFileInfo &info = currentFileInfos[currentIndex];
        setupTextPreview(info);
    }
}
QSize Preview_File_Widget::get_Image_Size(QString path)
{
    QImageReader reader(path);
    if (!reader.canRead())
    {
        return QSize(0,0);
    }
    QSize size = reader.size();
    if (!size.isValid())
    {
        QImage image =reader.read();
        if (!image.isNull())
        {
            size = image.size();
        }
    }
    return size;
}
static const QStringList textMimeTypes =
{
    "application/xml", "text/xml", "application/json",
    "application/x-shellscript", "application/x-python", "application/x-perl",
    "application/x-php", "application/javascript", "application/x-javascript",
    "application/x-qmake", "application/x-desktop",
    "application/x-awk", "application/x-csh",
    "text/x-c", "text/x-c++", "text/x-java", "text/x-python",
    "application/x-ms-dos-executable"
};
Preview_File_Widget::ContentType Preview_File_Widget::getContentType(const QFileInfo &info)
{
    if (info.isDir())
    {
        return ContentType::TypeFolder;
    }
    QMimeDatabase db;
    QMimeType mime = db.mimeTypeForFile(info);
    QString mimeName = mime.name();
    QString suffix = info.suffix().toLower();
    if (suffix == "exe")
    {
        return ContentType::TypeUnKnown;
    }
    if (mimeName.startsWith("text/"))
    {
        return ContentType::TypeText;
    }
    if (mime.inherits("text/plain"))
    {
        return ContentType::TypeText;
    }
    if (textMimeTypes.contains(mimeName))
    {
        return ContentType::TypeText;
    }
    if (mime.inherits("application/xml") || mime.inherits("text/xml"))
    {
        return ContentType::TypeText;
    }
    if (suffix == "qrc" || suffix == "json" || suffix == "pro" || suffix == "sh" ||
        suffix == "xml" || suffix == "cpp" || suffix == "h" || suffix == "txt" ||
        suffix == "md" || suffix == "py" || suffix == "js" || suffix == "html" ||
        suffix == "css" || suffix == "conf" || suffix == "ini")
    {
        return ContentType::TypeText;
    }
    if (mimeName.startsWith("image/"))
    {
        return ContentType::TypeImage;
    }
    if (mimeName.startsWith("video/"))
    {
        return ContentType::TypeVideo;
    }
    if (mimeName.startsWith("audio/"))
    {
        return ContentType::TypeAudio;
    }
    if (mimeName == "application/pdf")
    {
        return ContentType::TypePdf;
    }
    if (mimeName == "font/ttf" || mimeName == "font/otf" ||
        mimeName == "application/x-font-ttf" || mimeName == "application/x-font-otf" ||
        mimeName == "font/collection" || mimeName == "application/font-woff" ||
        mimeName == "application/font-woff2")
    {
        return ContentType::TypeFont;
    }
    if (suffix == "ttf" || suffix == "otf" || suffix == "ttc" ||
        suffix == "woff" || suffix == "woff2")
    {
        return ContentType::TypeFont;
    }
    return ContentType::TypeUnKnown;
}
QIcon Preview_File_Widget::get_icon(const QFileInfo &info)
{
    if (info.isFile())
    {
        QMimeDatabase mimeDb;
        QMimeType mimeType;
        mimeType = mimeDb.mimeTypeForFile(info);
        QString mimeName = mimeType.name();
        if (mimeName.startsWith("image/"))
        {
            QIcon icon = QIcon::fromTheme(info.filePath());
            if (!icon.isNull() && Preview_File_Widget::get_Image_Size(info.filePath()) != QSize(0, 0))
            {
                return icon;//应使用filePath而不是filename;
            }
            else
            {
                QIcon icon = QIcon::fromTheme(mimeType.iconName());
                QString theme_name = mimeType.iconName();
                if (icon.isNull())
                {
                    icon = QIcon::fromTheme(mimeType.genericIconName());
                    theme_name = mimeType.genericIconName();
                }
                if (icon.isNull())
                {
                    theme_name = "unknown";
                }
                return QIcon::fromTheme(theme_name);
            }
        }
        else if (mimeName == "application/x-desktop")
        {
            QSettings desktopSettings(info.filePath(), QSettings::IniFormat);
            desktopSettings.setIniCodec("UTF-8");
            desktopSettings.beginGroup("Desktop Entry");
            QString theme_name = desktopSettings.value("Icon", "application").toString();
            desktopSettings.endGroup();
            return QIcon::fromTheme(theme_name);
        }
        else
        {
            QIcon icon = QIcon::fromTheme(mimeType.iconName());
            QString theme_name = mimeType.iconName();
            if (icon.isNull())
            {
                icon = QIcon::fromTheme(mimeType.genericIconName());
                theme_name = mimeType.genericIconName();
            }
            if (icon.isNull())
            {
                theme_name = "unknown";
            }
            return QIcon::fromTheme(theme_name);
        }
    }
    else
    {
        QString theme_name = "folder";
        return QIcon::fromTheme(theme_name);
    }
}
void Preview_File_Widget::update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, QString checked_icon_path)
{
    m_theme_color = &theme_color;
    m_theme_background_color = &theme_background_color;
    m_theme_text_color = &theme_text_color;
    m_select_text_color = &select_text_color;
    m_disabled_text_color = &disabled_text_color;
    m_checked_icon_path = &checked_icon_path;
    m_textEdit->setStyleSheet(QString("QWidget{background:rgba(0,0,0,0);color:rgba(0,0,0,255)}"
                                      "QMenu{border-radius:10px 10px;background:rgba(%1,%2,%3,%4);margin:0px -1px 0px -1px;padding-top:8px;padding-bottom:8px;icon-size:20px;border-radius:10px 10px}"
                                      "QMenu::item{color:rgba(%5,%6,%7,%8);background:rgba(0,0,0,0);}"
                                      "QMenu::item:disabled{color:rgba(%9,%10,%11,%12)}"
                                      "QMenu::item:selected{color:rgba(%13,%14,%15,%16);background:rgba(%17,%18,%19,%20)}"
                                      "QMenu::indicator{width: 20px; height 20px; background: transparent;}"
                                      "QMenu::indicator:checked{image: url(%21);}"
                                      "QMenu::separator{background:rgba(150,150,150,125)}")
                              .arg(theme_background_color.red()).arg(theme_background_color.green()).arg(theme_background_color.blue()).arg(theme_background_color.alpha())
                              .arg(theme_text_color.red()).arg(theme_text_color.green()).arg(theme_text_color.blue()).arg(theme_text_color.alpha())
                              .arg(disabled_text_color.red()).arg(disabled_text_color.green()).arg(disabled_text_color.blue()).arg(disabled_text_color.alpha())
                              .arg(select_text_color.red()).arg(select_text_color.green()).arg(select_text_color.blue()).arg(select_text_color.alpha())
                              .arg(theme_color.red()).arg(theme_color.green()).arg(theme_color.blue()).arg(theme_color.alpha())
                              .arg(checked_icon_path));
    m_textEdit->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{border:none;background:rgba(0,0,0,0);width:8px;margin:0px0px0px0px;}"
                                                 "QScrollBar::handle:vertical{background:rgba(0,0,0,75);border-radius:4px;min-height:20px;}"
                                                 "QScrollBar::handle:vertical:hover{background:rgba(0,0,0,125);}"
                                                 "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0px;}"
                                                 "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}");
    m_textEdit->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{border:none;background:rgba(0,0,0,0);height:8px;margin:0px0px0px0px;}"
                                                   "QScrollBar::handle:horizontal{background:rgba(0,0,0,75);border-radius:4px;min-width:20px;}"
                                                   "QScrollBar::handle:horizontal:hover{background:rgba(0,0,0,125);}"
                                                   "QScrollBar::add-line:horizontal,QScrollBar::sub-line:horizontal{width:0px;}"
                                                   "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:none;}");
    m_textModeCombo->setStyleSheet(QString("QComboBox{background:rgba(255,255,255,200);color:rgba(0,0,0,255);border-radius:10px 10px;}"
                                           "QComboBox QAbstractItemView{background:rgba(0,0,0,0);}"
                                           "QComboBox QAbstractItemView::item:hover{background:rgba(0,0,0,0);}"));
    m_imageViewer->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{border:none;background:rgba(0,0,0,0);width:8px;margin:0px0px0px0px;}"
                                                 "QScrollBar::handle:vertical{background:rgba(0,0,0,75);border-radius:4px;min-height:20px;}"
                                                 "QScrollBar::handle:vertical:hover{background:rgba(0,0,0,125);}"
                                                 "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0px;}"
                                                 "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}");
    m_imageViewer->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{border:none;background:rgba(0,0,0,0);height:8px;margin:0px0px0px0px;}"
                                                   "QScrollBar::handle:horizontal{background:rgba(0,0,0,75);border-radius:4px;min-width:20px;}"
                                                   "QScrollBar::handle:horizontal:hover{background:rgba(0,0,0,125);}"
                                                   "QScrollBar::add-line:horizontal,QScrollBar::sub-line:horizontal{width:0px;}"
                                                   "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:none;}");
#ifdef USE_PDF
    m_pdfViewer->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{border:none;background:rgba(0,0,0,0);width:8px;margin:0px0px0px0px;}"
                                                 "QScrollBar::handle:vertical{background:rgba(0,0,0,75);border-radius:4px;min-height:20px;}"
                                                 "QScrollBar::handle:vertical:hover{background:rgba(0,0,0,125);}"
                                                 "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0px;}"
                                                 "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}");
    m_pdfViewer->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{border:none;background:rgba(0,0,0,0);height:8px;margin:0px0px0px0px;}"
                                                   "QScrollBar::handle:horizontal{background:rgba(0,0,0,75);border-radius:4px;min-width:20px;}"
                                                   "QScrollBar::handle:horizontal:hover{background:rgba(0,0,0,125);}"
                                                   "QScrollBar::add-line:horizontal,QScrollBar::sub-line:horizontal{width:0px;}"
                                                   "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:none;}");
#endif
    m_videoViewer->verticalScrollBar()->setStyleSheet("QScrollBar:vertical{border:none;background:rgba(0,0,0,0);width:8px;margin:0px0px0px0px;}"
                                                 "QScrollBar::handle:vertical{background:rgba(0,0,0,75);border-radius:4px;min-height:20px;}"
                                                 "QScrollBar::handle:vertical:hover{background:rgba(0,0,0,125);}"
                                                 "QScrollBar::add-line:vertical,QScrollBar::sub-line:vertical{height:0px;}"
                                                 "QScrollBar::add-page:vertical,QScrollBar::sub-page:vertical{background:none;}");
    m_videoViewer->horizontalScrollBar()->setStyleSheet("QScrollBar:horizontal{border:none;background:rgba(0,0,0,0);height:8px;margin:0px0px0px0px;}"
                                                   "QScrollBar::handle:horizontal{background:rgba(0,0,0,75);border-radius:4px;min-width:20px;}"
                                                   "QScrollBar::handle:horizontal:hover{background:rgba(0,0,0,125);}"
                                                   "QScrollBar::add-line:horizontal,QScrollBar::sub-line:horizontal{width:0px;}"
                                                   "QScrollBar::add-page:horizontal,QScrollBar::sub-page:horizontal{background:none;}");
    this->media_control_action->set_color(theme_text_color);
    for (int i = 0; i < preview_file_plugin_list.count(); ++i)
    {
        if (preview_file_plugin_list[i])
        {
            if (preview_file_plugin_list[i]->Plugin_Version >= P_Version{0, 0, 1})
            {
                if (preview_file_plugin_list[i]->inited)
                {
                    preview_file_plugin_list[i]->update_style(theme_color, theme_background_color, theme_text_color, select_text_color, disabled_text_color, checked_icon_path);
                }
            }
        }
    }
}
Info_Widget::Info_Widget(QWidget *parent)
    :QWidget(parent)
{
    setStyleSheet("border-radius: 7px; background:rgba(255,255,255,25)");
    icon_showing->setStyleSheet("background:rgba(255,255,255,50);border-radius: 10px;");
    icon_showing->move(5, 5);
    icon_showing->setAlignment(Qt::AlignCenter);
    m_sizeUpdateTimer->setInterval(1000);
    connect(m_sizeUpdateTimer, &QTimer::timeout, this, [=]()
    {
        Info_Widget::onSizeCalculated();
    });
    connect(m_futureWatcher, &QFutureWatcher<qint64>::finished, this, &Info_Widget::onSizeCalculated);
}
Info_Widget::~Info_Widget()
{
    m_sizeUpdateTimer->stop();
    if (m_futureWatcher->isRunning())
    {
        m_cancelCalculation = true;
        m_futureWatcher->cancel();
    }
}
void Info_Widget::Set_Data(QFileInfo &info)
{
    if (!this->isVisible())
    {
        return;
    }
    QIcon icon = Preview_File_Widget::get_icon(info);//icon不可能为空
    QPixmap pixmap = icon.pixmap(icon_showing->size());
    QSize pic_size = Preview_File_Widget::get_Image_Size(info.filePath());
    if (Preview_File_Widget::get_Image_Size(info.filePath()) != QSize(0, 0) && pic_size.width() != pic_size.height())
    {
        icon_showing->setScaledContents(false);
        if (pic_size.width() < pic_size.height())
        {
            pixmap.scaled(QSize(static_cast<int>(pic_size.width() * (90.0 / pic_size.height())), 90));
        }
        else
        {
            pixmap.scaled(QSize(90, static_cast<int>(pic_size.height() * (90.0 / pic_size.width()))));
        }
    }
    else
    {
        icon_showing->setScaledContents(true);
    }
    icon_showing->setPixmap(pixmap);
    file_name_label->setText(tr("名称:%1").arg(info.fileName()));
    QFont font = file_name_label->font();
    font.setBold(true);
    file_name_label->setFont(font);

    qint64 size = info.size();
    QString sizeStr;
    if (info.isDir())
    {
        if (m_currentDirPath != info.filePath())
        {
            m_cancelCalculation = false;
            temp_folder_total_size = 0;
            m_currentDirPath = info.filePath();
            if (m_futureWatcher->isRunning())
            {
                m_cancelCalculation = true;
                m_futureWatcher->setPaused(true);
                m_futureWatcher->cancel();
                m_futureWatcher->future().cancel();
                //m_futureWatcher->waitForFinished(); //delay no more [doge]
            }
            size_label->setText(tr("大小:文件夹"));
            updateFolderSize();
            m_sizeUpdateTimer->start();
        }
    }
    else
    {
        m_sizeUpdateTimer->stop();
        if (m_futureWatcher->isRunning())
        {
            m_cancelCalculation = true;
            m_futureWatcher->setPaused(true);
            m_futureWatcher->cancel();
            m_futureWatcher->future().cancel();
        }
        m_currentDirPath.clear();
        sizeStr = Info_Widget::formatSize(size);
        size_label->setText(tr("大小:%1").arg(sizeStr));
    }
    QString timeStr = info.lastModified().toString(Qt::SystemLocaleShortDate);
    change_time_label->setText(tr("修改时间:%1").arg(timeStr));

    QString typeStr;
    if (info.isDir())
    {
        typeStr = tr("文件夹");
    }
    else if (info.isSymLink())
    {
        typeStr = tr("符号链接");
    }
    else if (info.isFile())
    {
        QString suffix = info.suffix().toUpper();
        if (suffix.isEmpty())
        {
            typeStr = tr("文件");
        }
        else
        {
            typeStr = tr("%1 文件").arg(suffix);
        }
    }
    else
    {
        typeStr = tr("未知");
    }
    type_name_label->setText(tr("类型:%1").arg(typeStr));
}
void Info_Widget::updateFolderSize()
{
    if (!this->isVisible())
    {
        if (m_futureWatcher->isRunning())
        {
            m_cancelCalculation = true;
            m_futureWatcher->setPaused(true);
            m_futureWatcher->cancel();
            m_futureWatcher->future().cancel();
        }
        return;
    }
    if (m_currentDirPath.isEmpty())
    {
        return;
    }
    m_cancelCalculation = false;
    // 异步计算
    QFuture<qint64> future = QtConcurrent::run([this]() -> qint64
    {
        temp_folder_total_size = 0;
        QDirIterator it(m_currentDirPath, QDir::Files | QDir::Hidden | QDir::NoSymLinks, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            if (m_cancelCalculation.load())
            {
                return temp_folder_total_size;
            }
            it.next();//QDirIterator特性
            temp_folder_total_size += it.fileInfo().size();
        }
        return temp_folder_total_size;
    });
    m_futureWatcher->setFuture(future);
}
void Info_Widget::onSizeCalculated()
{
    if (!this->isVisible())
    {
        return;
    }
    if (!m_futureWatcher)
    {
        return;
    }
    if (m_currentDirPath.isEmpty() || !QDir(m_currentDirPath).exists())
    {
        return;
    }
    if (!m_futureWatcher->isFinished())
    {
        QString sizeStr = formatSize(temp_folder_total_size);
        size_label->setText(tr("大小:%1(计算中)").arg(sizeStr));
        return;
    }
    QFuture<long long> future = m_futureWatcher->future();
    if (!future.isResultReadyAt(0))
    {
        return;
    }
    m_sizeUpdateTimer->stop();
    QString sizeStr = formatSize(future.result());
    size_label->setText(tr("大小:%1").arg(sizeStr));
}
QString Info_Widget::formatSize(qint64 bytes)
{
    const char* units[] = {"B", "KiB", "MiB", "GiB"};
    double value = bytes;
    int unitIdx = 0;
    while (unitIdx < 3 && value > 2048.0)
    {
        value /= 1024.0;
        ++unitIdx;
    }
    return QString("%1 %2").arg(std::round(value * 100) / 100).arg(units[unitIdx]);
}
void Info_Widget::resizeEvent(QResizeEvent *event)
{
    icon_showing->resize(event->size().height() - 10, event->size().height() - 10);
    file_name_label->move(icon_showing->geometry().topRight() + QPoint(5, 0));
    file_name_label->resize(event->size().width() - file_name_label->x() - 5, (event->size().height() - 25) / 4);
    size_label->move(icon_showing->geometry().topRight() + QPoint(5, ((event->size().height() - 25) / 4 + 5)));
    size_label->resize(event->size().width() - size_label->x() - 5, (event->size().height() - 25) / 4);
    change_time_label->move(icon_showing->geometry().topRight() + QPoint(5, 2 * ((event->size().height() - 25) / 4 + 5)));
    change_time_label->resize(event->size().width() - change_time_label->x() - 5, (event->size().height() - 25) / 4);
    type_name_label->move(icon_showing->geometry().topRight() + QPoint(5, 3 * ((event->size().height() - 25) / 4 + 5)));
    type_name_label->resize(event->size().width() - type_name_label->x() - 5, (event->size().height() - 25) / 4);
}
GraphicsViewer::GraphicsViewer(QWidget *parent, Preview_File_Widget *m_preview_ptr)
    :QGraphicsView(parent)
    ,preview_ptr(m_preview_ptr)
{
    setScene(m_scene);
    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setStyleSheet("background: transparent; border: none;");
    gif_movie->setCacheMode(QMovie::CacheAll);
}
void GraphicsViewer::setGif(const QFileInfo &info)
{
    clear();
    gif_movie->setFileName(info.filePath());
    QPixmap firstFrame = gif_movie->currentPixmap();
    if (firstFrame.isNull())
    {
        gif_movie->jumpToNextFrame();
        firstFrame = gif_movie->currentPixmap();
    }
    m_originalSize = firstFrame.size();
    m_item = m_scene->addPixmap(firstFrame);
    m_scene->setSceneRect(m_item->boundingRect());

    resetTransform();
    m_currentScale = get_scaled(firstFrame.size(), this->size());
    scale(m_currentScale, m_currentScale);
    connect(gif_movie, &QMovie::frameChanged, this, [=]
    {
        m_item->setPixmap(gif_movie->currentPixmap());
        if (preview_ptr)
        {
            emit preview_ptr->send_position(static_cast<int>((static_cast<double>(gif_movie->currentFrameNumber()) / gif_movie->frameCount()) * 100), QString("%1/%2").arg(gif_movie->currentFrameNumber()).arg(gif_movie->frameCount()));
        }
    });
    gif_movie->start();
}
void GraphicsViewer::setImage(const QImage &image)
{
    setPixmap(QPixmap::fromImage(image));
}
void GraphicsViewer::setPixmap(const QPixmap &pixmap)
{
    clear();
    if (pixmap.isNull())
    {
        return;
    }
    m_originalSize = pixmap.size();
    m_item = m_scene->addPixmap(pixmap);
    m_scene->setSceneRect(m_item->boundingRect());
    fitInView(m_item, Qt::KeepAspectRatio);
    resetTransform();
    m_currentScale = get_scaled(pixmap.size(), this->size());
    scale(m_currentScale, m_currentScale);
}
void GraphicsViewer::clear()
{
    gif_movie->stop();
    disconnect(gif_movie, &QMovie::frameChanged, this, nullptr);
    m_scene->clear();
    m_item = nullptr;
    resetTransform();
    m_currentScale = 1.0;
    scale(m_currentScale, m_currentScale);
}
double GraphicsViewer::get_scaled(QSize image, QSize size)
{
    double scale_x = static_cast<double>(size.width()) / image.width();
    double scale_y = static_cast<double>(size.height()) / image.height();
    return qMin(scale_x, scale_y);
}
void GraphicsViewer::setGraphicsItem(QGraphicsItem *item)
{
    clear();
    if (!item)
    {
        return;
    }
    m_originalSize = item->boundingRect().size().toSize();
    m_scene->addItem(item);
    m_scene->setSceneRect(item->boundingRect());
    fitInView(item, Qt::KeepAspectRatio);
    m_currentScale = transform().m11();
}
void GraphicsViewer::wheelEvent(QWheelEvent *event)
{
    qreal scaleFactor = 1.1;
    if (event->angleDelta().y() < 0)
    {
        scaleFactor = 1.0 / scaleFactor;
    }
    QPointF mousePos = event->position();
    QPointF oldScenePos = mapToScene(mousePos.toPoint());
    qreal newScale = m_currentScale * scaleFactor;
    if (newScale < 0.1)
    {
        newScale = 0.1;
    }
    if (newScale > 10.0)
    {
        newScale = 10.0;
    }
    scale(scaleFactor, scaleFactor);
    m_currentScale = newScale;
    mousePos = event->position();
    QPointF newScenePos = mapToScene(mousePos.toPoint());
    QPointF delta = oldScenePos - newScenePos;//这里需要反向一下(已实现)
    horizontalScrollBar()->setValue(horizontalScrollBar()->value() + static_cast<int>(delta.x()));
    verticalScrollBar()->setValue(verticalScrollBar()->value() + static_cast<int>(delta.y()));
    event->accept();
}
void GraphicsViewer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_panning = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
}
void GraphicsViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (m_panning)
    {
        QPointF delta = event->pos() - m_lastPanPoint;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - static_cast<int>(delta.x()));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - static_cast<int>(delta.y()));
        m_lastPanPoint = event->pos();
    }
}
void GraphicsViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_panning = false;
        setCursor(Qt::ArrowCursor);
    }
}
void GraphicsViewer::resetZoom()
{
    if (m_originalSize.isEmpty())
    {
        qreal newScale = get_scaled(this->items().first()->boundingRect().size().toSize(), this->size());
        resetTransform();
        scale(newScale, newScale);
        m_currentScale = newScale;
        horizontalScrollBar()->setValue(0);
        verticalScrollBar()->setValue(0);
        return;
    }
    qreal newScale = get_scaled(m_originalSize, this->size());
    resetTransform();
    scale(newScale, newScale);
    m_currentScale = newScale;
    horizontalScrollBar()->setValue(0);
    verticalScrollBar()->setValue(0);
}
QMovie *GraphicsViewer::get_gif_movie()
{
    return gif_movie;
}
#ifdef USE_PDF
PdfViewer::PdfViewer(QWidget *parent)
    : QPdfView(parent)
{
    setAcceptDrops(false);
    setPageMode(QPdfView::PageMode::MultiPage);
    setZoomMode(QPdfView::ZoomMode::CustomZoom);
    setCursor(Qt::ArrowCursor);
    setAttribute(Qt::WA_TranslucentBackground);
    setStyleSheet("background: transparent;");
    setAutoFillBackground(false);
    if (viewport())
    {
        viewport()->setStyleSheet("background: transparent;");
        viewport()->setAutoFillBackground(false);
    }
}
void PdfViewer::resetZoom()
{
    if (!document() || document()->pageCount() == 0)
    {
        return;
    }
    QSizeF pageSizeF = document()->pageSize(0);
    if (pageSizeF.isEmpty())
    {
        return;
    }
    QSize pageSize = pageSizeF.toSize();
    double scaleX = static_cast<double>(viewport()->width()) / pageSize.width();
    double scaleY = static_cast<double>(viewport()->height()) / pageSize.height();
    qreal newZoom = qMin(scaleX, scaleY);
    newZoom = qBound(0.1, newZoom, 10.0);
    if (qFuzzyCompare(newZoom, zoomFactor()))
    {
        return;
    }
    setUpdatesEnabled(false);
    setZoomFactor(newZoom);
    horizontalScrollBar()->setValue(0);
    verticalScrollBar()->setValue(0);
    setUpdatesEnabled(true);
    update();
}
void PdfViewer::wheelEvent(QWheelEvent *event)
{
    if (!document() || document()->pageCount() == 0)
    {
        event->ignore();
        return;
    }
    qreal zoomDelta = 1.1;
    if (event->angleDelta().y() < 0)
    {
        zoomDelta = 1.0 / zoomDelta;
    }
    QPoint viewPos = event->position().toPoint();
    qreal oldZoom = zoomFactor();
    QPointF docPos;
    int hScrollValue = horizontalScrollBar()->value();
    int vScrollValue = verticalScrollBar()->value();
    docPos.rx() = (viewPos.x() + hScrollValue) / oldZoom;
    docPos.ry() = (viewPos.y() + vScrollValue) / oldZoom;
    qreal newZoom = oldZoom * zoomDelta;
    newZoom = qBound(0.1, newZoom, 10.0);
    if (qFuzzyCompare(newZoom, oldZoom))
    {
        event->accept();
        return;
    }
    bool updatesWereEnabled = updatesEnabled();
    setUpdatesEnabled(false);
    setZoomFactor(newZoom);
    int newHScrollValue = static_cast<int>(docPos.x() * newZoom - viewPos.x());
    int newVScrollValue = static_cast<int>(docPos.y() * newZoom - viewPos.y());
    newHScrollValue = qBound(0, newHScrollValue, horizontalScrollBar()->maximum());
    newVScrollValue = qBound(0, newVScrollValue, verticalScrollBar()->maximum());
    horizontalScrollBar()->setValue(newHScrollValue);
    verticalScrollBar()->setValue(newVScrollValue);
    setUpdatesEnabled(updatesWereEnabled);
    update();
    event->accept();
}
void PdfViewer::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_panning = true;
        m_lastPanPoint = event->pos();
        setCursor(Qt::ClosedHandCursor);
        event->accept();
    }
}
void PdfViewer::mouseMoveEvent(QMouseEvent *event)
{
    if (m_panning)
    {
        QPoint delta = event->pos() - m_lastPanPoint;
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        m_lastPanPoint = event->pos();
        event->accept();
    }
}
void PdfViewer::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_panning = false;
        setCursor(Qt::ArrowCursor);
        event->accept();
    }
}
#endif
QAction *Preview_File_Widget::get_prevAction()
{
    return prevAction;
}
QAction *Preview_File_Widget::get_nextAction()
{
    return nextAction;
}
QMenu *Preview_File_Widget::get_textEdit_View_Mode_Menu()
{
    return textEdit_View_Mode_Menu;
}
QAction *Preview_File_Widget::get_textEdit_Mode_TEXT()
{
    return textEdit_Mode_TEXT;
}
QAction *Preview_File_Widget::get_textEdit_Mode_HTML()
{
    return textEdit_Mode_HTML;
}
QAction *Preview_File_Widget::get_textEdit_Mode_MARKDOWN()
{
    return textEdit_Mode_MARKDOWN;
}
QAction *Preview_File_Widget::get_textEdit_Mode_SVG()
{
    return textEdit_Mode_SVG;
}
QAction *Preview_File_Widget::get_textEdit_Mode_HEX()
{
    return textEdit_Mode_HEX;
}
QAction *Preview_File_Widget::get_prevPage()
{
    return prevPage;
}
QAction *Preview_File_Widget::get_nextPage()
{
    return nextPage;
}
QAction *Preview_File_Widget::get_reset_size_action()
{
    return reset_size_action;
}
QAction *Preview_File_Widget::get_auto_play_action()
{
    return auto_play_action;
}
QAction *Preview_File_Widget::get_force_read_action()
{
    return force_read_action;
}
QAction *Preview_File_Widget::get_play_action()
{
    return play_action;
}
QAction *Preview_File_Widget::get_stop_action()
{
    return stop_action;
}
QPushButton *Preview_File_Widget::get_prevButton()
{
    return prevButton;
}
QPushButton *Preview_File_Widget::get_nextButton()
{
    return nextButton;
}
QPushButton *Preview_File_Widget::get_prevPageButton()
{
    return prevPageButton;
}
QPushButton *Preview_File_Widget::get_nextPageButton()
{
    return nextPageButton;
}
QPushButton *Preview_File_Widget::get_playButton()
{
    return playButton;
}
QPushButton *Preview_File_Widget::get_stopButton()
{
    return stopButton;
}
QPushButton *Preview_File_Widget::get_force_read_Button()
{
    return force_read_Button;
}
QTextEdit *Preview_File_Widget::get_m_textEdit()
{
    return m_textEdit;
}
QComboBox *Preview_File_Widget::get_m_textModeCombo()
{
    return m_textModeCombo;
}
QPdfDocument *Preview_File_Widget::get_m_pdfDocument()
{
#ifdef USE_PDF
    return m_pdfDocument;
#else
    return nullptr;
#endif
}
PdfViewer_Interface *Preview_File_Widget::get_m_pdfViewer()
{
#ifdef USE_PDF
    return m_pdfViewer;
#else
    return nullptr;
#endif
}
GraphicsViewer_Interface *Preview_File_Widget::get_m_imageViewer()
{
    return m_imageViewer;
}
QMediaPlayer *Preview_File_Widget::get_m_mediaPlayer()
{
    return m_mediaPlayer;
}
GraphicsViewer_Interface *Preview_File_Widget::get_m_videoViewer()
{
    return m_videoViewer;
}
QPdfView *Preview_File_Widget::get_m_pdfViewer_as_QPdfView()
{
#ifdef USE_PDF
    return m_pdfViewer;
#else
    return nullptr;
#endif
}
QGraphicsView *Preview_File_Widget::get_m_imageViewer_as_QGraphicsView()
{
    return m_imageViewer;
}
QGraphicsView *Preview_File_Widget::get_m_videoViewer_as_QGraphicsView()
{
    return m_videoViewer;
}
QWidget *Preview_File_Widget::get_m_infoWidget()
{
    return m_infoWidget;
}
Media_WidgetAction_Interface *Preview_File_Widget::get_media_control_action()
{
    return media_control_action;
}
#include <QPluginLoader>
void Preview_File_Widget::load_plugin(QString filepath)
{
    QPluginLoader *plugin_loader = new QPluginLoader(this);
    if (filepath.isEmpty()) return;
    plugin_loader->setFileName(filepath);
    if (!plugin_loader->load())
    {
        qDebug() << tr("插件导入失败:") << plugin_loader->errorString();
        return;
    }
    QObject *pluginInstance = plugin_loader->instance();
    if (!pluginInstance)
    {
        plugin_loader->unload();
        return;
    }
    Ext_Preview_PluginInterface *plugin_interface = qobject_cast<Ext_Preview_PluginInterface *>(pluginInstance);
    if (plugin_interface)
    {
        if (is_Ext_plugin(plugin_loader))
        {
            if (Preview_File_Widget::Contains_Ext_Plugin(plugin_interface->Ext_Name, "Easy_Desktop_Preview"))
            {
                if (plugin_interface->Plugin_Version >= P_Version{0, 0, 1})
                {
                    plugin_interface->init(this, this->get_self());
                    plugin_interface->your_plugin_loader = plugin_loader;
                    plugin_interface->plugin_path = filepath;
                    plugin_interface->inited = true;
                    if (m_theme_color)
                    {
                        plugin_interface->update_style(*m_theme_color, *m_theme_background_color, *m_theme_text_color, *m_select_text_color, *m_disabled_text_color, *m_checked_icon_path);
                    }
                }
                preview_file_plugin_list << plugin_interface;
            }
        }
    }
}
bool Preview_File_Widget::is_Ext_plugin(QPluginLoader *plugin_loader)
{
    QJsonObject root_obj = plugin_loader->metaData();
    if (!root_obj.contains("MetaData")) return false;
    QJsonValue meta_value = root_obj.value("MetaData");
    if (!meta_value.isObject()) return false;
    QJsonObject meta_obj = meta_value.toObject();
    return meta_obj.contains("Ext_Preview_Plugin");
}
bool Preview_File_Widget::Contains_Ext_Plugin(QString Ext_name, QString plugin_controller_name)
{
    QStringList ext_list = Ext_name.split("|");
    return ext_list.contains(plugin_controller_name);
}
