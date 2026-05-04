#include "preview_file_widget.h"
#include <QSvgRenderer>

Preview_File_Widget::Preview_File_Widget(QWidget *parent, QAction *m_preview_action)
    :Basic_Widget(parent)
    ,preview_action(m_preview_action)
{
    hide();
    m_textEdit->hide();
    //m_textEdit->setReadOnly(true); //玩就玩吧,反正改不了
    m_imageViewer->hide();
    m_pdfViewer->hide();
    m_videoWidget->hide();
    m_audioWidget->hide();
    this->auto_close = false;
    menu->addAction(prevAction);
    menu->addAction(nextAction);
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
        m_textEdit->resize(size - QSize(10, 150));
        m_imageViewer->resize(size - QSize(10, 150));
        m_pdfViewer->resize(size - QSize(10, 150));
    });
    prevButton->setEnabled(false);
    nextButton->setEnabled(false);

    prevButton->resize(30, 30);
    prevButton->setFocusPolicy(Qt::NoFocus);
    prevButton->setStyleSheet("QPushButton{border-radius:10px 10px;background:rgba(255,255,255,150)}"
                              "QPushButton:hover{border-radius:10px 10px;background:rgba(255,255,255,200)}"
                              "QPushButton:pressed{border-radius:10px 10px;background:rgba(255,255,255,150)}");
    nextButton->resize(30, 30);
    nextButton->setFocusPolicy(Qt::NoFocus);
    nextButton->setStyleSheet("QPushButton{border-radius:10px 10px;background:rgba(255,255,255,150)}"
                              "QPushButton:hover{border-radius:10px 10px;background:rgba(255,255,255,200)}"
                              "QPushButton:pressed{border-radius:10px 10px;background:rgba(255,255,255,150)}");

    m_infoWidget->move(10, 40);
    m_infoWidget->show();

    //105
    m_textModeCombo->addItem(tr("纯文本"));
    m_textModeCombo->addItem(tr("HTML"));
    m_textModeCombo->addItem(tr("Markdown"));
    m_textModeCombo->addItem(tr("查看svg"));
    m_textModeCombo->setCurrentIndex(0);
    m_textModeCombo->hide();
    m_textEdit->move(5, 145);
    m_imageViewer->move(5, 145);
    m_pdfViewer->move(5, 145);
    clearCurrentPreview();

    connect(prevButton, &QPushButton::clicked, this, &Preview_File_Widget::onPrevClicked);
    connect(nextButton, &QPushButton::clicked, this, &Preview_File_Widget::onNextClicked);
    connect(m_textModeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=]
    {
        clearCurrentPreview();
        setupTextPreview(currentFileInfos[currentIndex]);
    });
    resize(350, 400);
}
Preview_File_Widget::~Preview_File_Widget()
{}
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
}
void Preview_File_Widget::load(QSettings *settings, QString Token)
{
    Basic_Widget::load(settings, Token);
    m_textEdit->H_load_no_text(settings, Token + "preview_textedit_");
}
void Preview_File_Widget::set_icon(QString checked_icon_path)
{
    Basic_Widget::set_icon(checked_icon_path);//预留点,可能不会添加,这段话是在构造这个类的时候写的.
}
void Preview_File_Widget::updatePreview(QStringList selectionFileList, QString parent_dir)
{
    if (!isVisible())
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
    clearCurrentPreview();
    if (currentFileInfos.isEmpty())
    {
        QFileInfo dirInfo(m_parent_dir);
        m_infoWidget->Set_Data(dirInfo);
        return;
    }
    QFileInfo &info = currentFileInfos[currentIndex];
    m_infoWidget->Set_Data(info);
    ContentType type = getContentType(info);
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
        setupPdfPreview(info);
        break;
    }
    case ContentType::TypeFolder:
    {
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
    m_textEdit->hide();
    m_textEdit->clear();
    m_textModeCombo->hide();
    //此处本应将m_textModeCombo的currentIndex设为0,但考虑到连续性,故先在此放置标记
    m_imageViewer->hide();
    m_imageViewer->clear();
    m_pdfViewer->hide();
    m_pdfViewer->clear();

    m_videoWidget->hide();
    m_audioWidget->hide();
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
    m_pdfDocument->load(info.filePath());
    if (m_pdfDocument->pageCount() == 0)
    {
        m_textEdit->setPlainText(tr("无法加载 PDF文件或文件为空"));
        m_textEdit->show();
        return;
    }
    QSize pageSize = m_pdfDocument->pageSize(0).toSize();
    if (pageSize.isEmpty())
    {
        pageSize = QSize(500, 500);
    }
    QImage image(pageSize, QImage::Format_ARGB32);
    image.fill(Qt::white);
    image = m_pdfDocument->render(0, pageSize);
    QPainter painter(&image);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.end();
    m_pdfViewer->setImage(image);
    m_pdfViewer->show();
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
{}
void Preview_File_Widget::setupAudioPreview(const QFileInfo &info)
{}
void Preview_File_Widget::setupSvgPreview(const QFileInfo &info)
{
    auto Item = new QGraphicsSvgItem(info.filePath());
    if (!Item->renderer()->isValid())
    {
        Item->deleteLater();
        Item = nullptr;
        m_textEdit->setPlainText(tr("无法加载SVG文件:%1").arg(info.fileName()));
        m_textEdit->show();
        return;
    }
    m_imageViewer->setGraphicsItem(Item);
    m_imageViewer->show();
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
    else
    {
        Basic_Widget::basic_action_func(know_what);
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
    QString suffix = info.suffix().toLower();
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
}
Info_Widget::Info_Widget(QWidget *parent)
    :QWidget(parent)
{
    setStyleSheet("border-radius: 7px; background:rgba(255,255,255,25)");
    icon_showing->setStyleSheet("background:rgba(255,255,255,50);border-radius: 10px;");
    icon_showing->move(5, 5);
}
void Info_Widget::Set_Data(QFileInfo &info)
{
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
        sizeStr = tr("文件夹");
    }
    else
    {
        sizeStr = Info_Widget::formatSize(size);
    }
    size_label->setText(tr("大小:%1").arg(sizeStr));

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
GraphicsViewer::GraphicsViewer(QWidget *parent)
    :QGraphicsView(parent)
{
    setScene(m_scene);
    setDragMode(QGraphicsView::NoDrag);
    setTransformationAnchor(QGraphicsView::AnchorViewCenter);
    setResizeAnchor(QGraphicsView::AnchorViewCenter);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    setStyleSheet("background: transparent; border: none;");
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
    m_item = m_scene->addPixmap(firstFrame);
    m_scene->setSceneRect(m_item->boundingRect());
    fitInView(m_item, Qt::KeepAspectRatio);
    resetTransform();
    m_currentScale = get_scaled(firstFrame.size(), this->size());
    scale(m_currentScale, m_currentScale);
    connect(gif_movie, &QMovie::frameChanged, this, [=]
    {
        m_item->setPixmap(gif_movie->currentPixmap());
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
