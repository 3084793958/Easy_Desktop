#ifndef PREVIEW_FILE_WIDGET_H
#define PREVIEW_FILE_WIDGET_H
#include "core/basic_widget.h"
#include "core/my_lineedit.h"
#include <QStackedWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QGraphicsItem>
#include <QGraphicsSvgItem>
#include <QGraphicsVideoItem>
#include "media_widgetaction.h"
#pragma push_macro("Status")
#undef Status
#include <QPdfDocument>
#include <QPdfView>
#pragma pop_macro("Status")

class Preview_File_Widget;
class GraphicsViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsViewer(QWidget *parent = nullptr, Preview_File_Widget *m_preview_ptr = nullptr);
    void setImage(const QImage &image);
    void setPixmap(const QPixmap &pixmap);
    void setGif(const QFileInfo &info);
    void setGraphicsItem(QGraphicsItem *item);
    void clear();
    static double get_scaled(QSize image, QSize size);
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    QGraphicsScene *m_scene = new QGraphicsScene(this);
    QGraphicsPixmapItem *m_item = nullptr;
    QPointF m_lastPanPoint = QPointF(0, 0);
    bool m_panning = false;
    char m_padding[7];
    qreal m_currentScale = 0.5;
    Preview_File_Widget *preview_ptr = nullptr;
public:
    QMovie *gif_movie = new QMovie(this);
    void resetZoom();
private:
    QSize m_originalSize = QSize();
};
class PdfViewer : public QPdfView
{
    Q_OBJECT
public:
    explicit PdfViewer(QWidget *parent = nullptr);
    void resetZoom();
protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
private:
    bool m_panning = false;
    char m_padding[7];
    QPoint m_lastPanPoint;
};
class Info_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Info_Widget(QWidget *parent);
    ~Info_Widget() override;
    void Set_Data(QFileInfo &info);
    static QString formatSize(qint64 bytes);
protected:
    void resizeEvent(QResizeEvent *event) override;
private:
    QLabel *icon_showing = new QLabel(this);
    QLabel *file_name_label = new QLabel(this);
    QLabel *size_label = new QLabel(this);
    QLabel *change_time_label = new QLabel(this);
    QLabel *type_name_label = new QLabel(this);
private:
    QTimer *m_sizeUpdateTimer = new QTimer(this);
    QFutureWatcher<qint64> *m_futureWatcher = new QFutureWatcher<qint64>(this);
    QString m_currentDirPath = QString();
private slots:
    void updateFolderSize();
    void onSizeCalculated();
};
class Preview_File_Widget : public Basic_Widget
{
    Q_OBJECT
public:
    enum class ContentType
    {
        TypeText,
        TypeImage,
        TypeVideo,
        TypeAudio,
        TypePdf,
        TypeFolder,
        TypeFont,
        TypeUnKnown
    };
    explicit Preview_File_Widget(QWidget *parent, QAction *m_preview_action);
    ~Preview_File_Widget() override;
    virtual void save(QSettings *settings, QString Token) override;
    virtual void load(QSettings *settings, QString Token) override;
    virtual void set_icon(QString checked_icon_path) override;
    void updatePreview(QStringList selectionFileList, QString parent_dir, bool force_update = false);
    static QIcon get_icon(const QFileInfo &info);
    static QSize get_Image_Size(QString path);
    static ContentType getContentType(const QFileInfo &info);
    void update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, QString checked_icon_path);
signals:
    void send_position(int value, QString text);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;
    void hideEvent(QHideEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
private slots:
    void onPrevClicked();
    void onNextClicked();
private:
    void updateCurrentPreview();
    void clearCurrentPreview();
    void setupTextPreview(const QFileInfo &info);
    void setupPdfPreview(const QFileInfo &info);
    void setupImagePreview(const QFileInfo &info);
    void setupVideoPreview(const QFileInfo &info);
    void setupAudioPreview(const QFileInfo &info);
    void setupSvgPreview(const QFileInfo &info);
    void setupFontPreview(const QFileInfo &info);
private:
    QAction *preview_action = nullptr;
private slots:
    void prevPdfPage();
    void nextPdfPage();
    void force_read_file();
private:
    QMenu *menu = new QMenu(this);
    QAction *prevAction = new QAction(tr("上一个"), this);
    QAction *nextAction = new QAction(tr("下一个"), this);

    QMenu *textEdit_View_Mode_Menu = new QMenu(tr("文本查看方式"), this);
    QAction *textEdit_Mode_TEXT = new QAction(tr("纯文本"), this);
    QAction *textEdit_Mode_HTML = new QAction(tr("HTML"), this);
    QAction *textEdit_Mode_MARKDOWN = new QAction(tr("Markdown"), this);
    QAction *textEdit_Mode_SVG = new QAction(tr("查看svg"), this);
    QAction *textEdit_Mode_HEX = new QAction(tr("十六进制"), this);

    QAction *prevPage = new QAction(tr("上一页"), this);
    QAction *nextPage = new QAction(tr("下一页"), this);
    QAction *reset_size_action = new QAction(tr("重置大小"), this);
    QAction *auto_play_action = new QAction(tr("音视频自动播放"), this);
    QAction *force_read_action = new QAction(tr("强制文本读取"), this);
    QAction *play_action = new QAction(tr("播放"), this);
    QAction *stop_action = new QAction(tr("暂停"), this);
    Media_WidgetAction *media_control_action = new Media_WidgetAction(this);
private:
    QPushButton *prevButton = new QPushButton(tr("<"), this->get_self());
    QPushButton *nextButton = new QPushButton(tr(">"), this->get_self());
    QPushButton *prevPageButton = new QPushButton(tr("上一页"), this->get_self());
    QPushButton *nextPageButton = new QPushButton(tr("下一页"), this->get_self());
    QPushButton *playButton = new QPushButton(tr("播放"), this->get_self());
    QPushButton *stopButton = new QPushButton(tr("暂停"), this->get_self());
    QPushButton *force_read_Button = new QPushButton(tr("强制文本读取"), this->get_self());
    QList<QFileInfo> currentFileInfos = {};
    QString m_parent_dir = "";
    int currentIndex = 0;
    int pdf_currentIndex = 0;

    Basic_TextEdit *m_textEdit = new Basic_TextEdit(this->get_self());
    QComboBox *m_textModeCombo = new QComboBox(this->get_self());

    QPdfDocument *m_pdfDocument = new QPdfDocument(this);
    PdfViewer *m_pdfViewer = new PdfViewer(this->get_self());

    GraphicsViewer *m_imageViewer = new GraphicsViewer(this->get_self(), this);

    QMediaPlayer *m_mediaPlayer = new QMediaPlayer(this);
    GraphicsViewer *m_videoViewer = new GraphicsViewer(this->get_self(), this);

    Info_Widget *m_infoWidget = new Info_Widget(this->get_self());
private:
    QTimer *holding_pos_timer = new QTimer(this);
    int holding_time = 0;
    int holding_value = 0;
    int holding_max_time = 10;
private:
    void Set_Speed(int value);
    void Set_Volume(int value);
    void Set_Position(int value);
private:
    int m_currentFontId = -1;
};

#endif // PREVIEW_FILE_WIDGET_H
