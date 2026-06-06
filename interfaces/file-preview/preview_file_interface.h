#ifndef PREVIEW_FILE_INTERFACE_H
#define PREVIEW_FILE_INTERFACE_H

class QFileInfo;
class QAction;
class QMenu;
class QPushButton;

class QTextEdit;
class QComboBox;
class QPdfDocument;
class QMediaPlayer;

class QWidget;

#include "interfaces/media-widget-action/media_widgetaction_interface.h"

class QImage;
class QPixmap;
class QGraphicsItem;
class QMovie;
class GraphicsViewer_Interface
{
public:
    virtual ~GraphicsViewer_Interface() {}
    virtual void setImage(const QImage &image) = 0;
    virtual void setPixmap(const QPixmap &pixmap) = 0;
    virtual void setGif(const QFileInfo &info) = 0;
    virtual void setGraphicsItem(QGraphicsItem *item) = 0;
    virtual void clear() = 0;
    virtual void resetZoom() = 0;
    virtual QMovie *get_gif_movie() = 0;
};

class PdfViewer_Interface
{
public:
    virtual ~PdfViewer_Interface() {}
    virtual void resetZoom() = 0;
};

class Zip_TreeView_Carrier_Interface
{
public:
    virtual ~Zip_TreeView_Carrier_Interface() {}
    virtual void setupTar(const QFileInfo &info) = 0;
    virtual void clear() = 0;
};

class QPdfView;
class QGraphicsView;

class Preview_File_Interface
{
public:
    virtual ~Preview_File_Interface() {}
public:
    virtual void onPrevClicked() = 0;
    virtual void onNextClicked() = 0;
public:
    virtual void updateCurrentPreview() = 0;
    virtual void clearCurrentPreview() = 0;
    virtual void setupTextPreview(const QFileInfo &info) = 0;
    virtual void setupPdfPreview(const QFileInfo &info) = 0;
    virtual void setupImagePreview(const QFileInfo &info) = 0;
    virtual void setupVideoPreview(const QFileInfo &info) = 0;
    virtual void setupAudioPreview(const QFileInfo &info) = 0;
    virtual void setupSvgPreview(const QFileInfo &info) = 0;
    virtual void setupFontPreview(const QFileInfo &info) = 0;
public:
    virtual void prevPdfPage() = 0;
    virtual void nextPdfPage() = 0;
    virtual void force_read_file() = 0;
public:
    virtual QAction *get_prevAction() = 0;
    virtual QAction *get_nextAction() = 0;
    virtual QMenu *get_textEdit_View_Mode_Menu() = 0;
    virtual QAction *get_textEdit_Mode_TEXT() = 0;
    virtual QAction *get_textEdit_Mode_HTML() = 0;
    virtual QAction *get_textEdit_Mode_MARKDOWN() = 0;
    virtual QAction *get_textEdit_Mode_SVG() = 0;
    virtual QAction *get_textEdit_Mode_HEX() = 0;
    virtual QAction *get_prevPage() = 0;
    virtual QAction *get_nextPage() = 0;
    virtual QAction *get_reset_size_action() = 0;
    virtual QAction *get_auto_play_action() = 0;
    virtual QAction *get_force_read_action() = 0;
    virtual QAction *get_play_action() = 0;
    virtual QAction *get_stop_action() = 0;
    virtual Media_WidgetAction_Interface *get_media_control_action() = 0;
public:
    virtual QPushButton *get_prevButton() = 0;
    virtual QPushButton *get_nextButton() = 0;
    virtual QPushButton *get_prevPageButton() = 0;
    virtual QPushButton *get_nextPageButton() = 0;
    virtual QPushButton *get_playButton() = 0;
    virtual QPushButton *get_stopButton() = 0;
    virtual QPushButton *get_force_read_Button() = 0;

    virtual QTextEdit *get_m_textEdit() = 0;
    virtual QComboBox *get_m_textModeCombo() = 0;
    virtual QPdfDocument *get_m_pdfDocument() = 0;
    virtual PdfViewer_Interface *get_m_pdfViewer() = 0;
    virtual GraphicsViewer_Interface *get_m_imageViewer() = 0;
    virtual QMediaPlayer *get_m_mediaPlayer() = 0;
    virtual GraphicsViewer_Interface *get_m_videoViewer() = 0;

    virtual QPdfView *get_m_pdfViewer_as_QPdfView() = 0;
    virtual QGraphicsView *get_m_imageViewer_as_QGraphicsView() = 0;
    virtual QGraphicsView *get_m_videoViewer_as_QGraphicsView() = 0;

    virtual QWidget *get_m_infoWidget() = 0;
public:
    virtual void Set_Speed(int value) = 0;
    virtual void Set_Volume(int value) = 0;
    virtual void Set_Position(int value) = 0;
};

class Preview_File_Interface_V_0_0_2
{
public:
    virtual ~Preview_File_Interface_V_0_0_2() {}
    virtual void setupTarPreview(const QFileInfo &info) = 0;
    virtual Zip_TreeView_Carrier_Interface *get_m_zip_treeview() = 0;
    virtual QWidget *get_m_zip_treeview_as_QWidget() = 0;
};

#endif // PREVIEW_FILE_INTERFACE_H
