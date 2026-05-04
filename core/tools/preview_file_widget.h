#ifndef PREVIEW_FILE_WIDGET_H
#define PREVIEW_FILE_WIDGET_H
#include "core/basic_widget.h"
#include "core/my_lineedit.h"
#include <QStackedWidget>
#include <QMediaPlayer>
#include <QVideoWidget>
#include <QGraphicsItem>
#include <QGraphicsSvgItem>
#pragma push_macro("Status")
#undef Status
#include <QPdfDocument>
#include <QPdfView>
#pragma pop_macro("Status")

class GraphicsViewer : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsViewer(QWidget *parent = nullptr);
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
    QMovie *gif_movie = new QMovie(this);
};
class Info_Widget : public QWidget
{
    Q_OBJECT
public:
    explicit Info_Widget(QWidget *parent);
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
        TypeUnKnown
    };
    explicit Preview_File_Widget(QWidget *parent, QAction *m_preview_action);
    ~Preview_File_Widget() override;
    virtual void save(QSettings *settings, QString Token) override;
    virtual void load(QSettings *settings, QString Token) override;
    virtual void set_icon(QString checked_icon_path) override;
    void updatePreview(QStringList selectionFileList, QString parent_dir);
    static QIcon get_icon(const QFileInfo &info);
    static QSize get_Image_Size(QString path);
    static ContentType getContentType(const QFileInfo &info);
    void update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, QString checked_icon_path);
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
private:
    QAction *preview_action = nullptr;
private:
    QMenu *menu = new QMenu(this);
    QAction *prevAction = new QAction(tr("上一个"), this);
    QAction *nextAction = new QAction(tr("下一个"), this);
private:
    QPushButton *prevButton = new QPushButton(tr("<"), this->get_self());
    QPushButton *nextButton = new QPushButton(tr(">"), this->get_self());
    QList<QFileInfo> currentFileInfos = {};
    QString m_parent_dir = "";
    int currentIndex = 0;

    Basic_TextEdit *m_textEdit = new Basic_TextEdit(this->get_self());
    QComboBox *m_textModeCombo = new QComboBox(this->get_self());

    QPdfDocument *m_pdfDocument = new QPdfDocument(this);
    GraphicsViewer *m_pdfViewer = new GraphicsViewer(this->get_self());

    GraphicsViewer *m_imageViewer = new GraphicsViewer(this->get_self());

    QMediaPlayer *m_mediaPlayer = new QMediaPlayer(this);
    QVideoWidget *m_videoWidget = new QVideoWidget(this->get_self());
    QWidget *m_audioWidget = new QWidget(this->get_self());

    Info_Widget *m_infoWidget = new Info_Widget(this->get_self());
};

#endif // PREVIEW_FILE_WIDGET_H
