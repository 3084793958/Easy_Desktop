#ifndef ZIP_TREEVIEW_H
#define ZIP_TREEVIEW_H
#include <QTreeView>
#include <QFileInfo>
#include <QProcess>
#include <QStandardItemModel>
#include <QPointer>
#include <QSettings>
#include "core/module/my_treeview_delegate.h"
#include <QSortFilterProxyModel>
#include <QRubberBand>
#include <QTimer>
#include <QMenu>
#include <QAction>

#include <QLabel>
#include <QStatusBar>

#include "interfaces/file-preview/preview_file_interface.h"

#include "core/tools/trans_action.h"

class Zip_TreeView_ProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    explicit Zip_TreeView_ProxyModel(QObject *parent = nullptr);
protected:
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;
};

class Zip_TreeView : public QTreeView
{
    Q_OBJECT
public:
    struct Paths_File_Info
    {
        QString name = "";
        QString date = "";
        QString time = "";
        QString length = "";
        Paths_File_Info(QString m_name = "", QString m_date = "", QString m_time = "", QString m_length = "");
    };
    explicit Zip_TreeView(QWidget *parent = nullptr);
    ~Zip_TreeView() override;
    void setupTar(const QFileInfo &info);
    void clear();
    static void buildTreeModelFromPaths(QStandardItemModel *model, const QList<Paths_File_Info> &paths);
    void load(QSettings *settings, QString Token);
    void save(QSettings *settings, QString Token);
    static QString formatSize(qint64 bytes);
    static Zip_TreeView * catch_ptr;
signals:
    void loadingFinished(bool success);
private slots:
    void onProcessFinished();
    void onProcessError();
private:
    void startArchiveListing(const QFileInfo &info);
    void parseZipOutput(const QByteArray &output);
    void parseTarOutput(const QByteArray &output);
    void buildTreeModel(const QList<Paths_File_Info> &paths);

    QStandardItemModel *m_model = nullptr;
    QPointer<QProcess> m_currentProcess = nullptr;
    QString m_currentArchivePath = "";
    Zip_TreeView_ProxyModel *proxyModel = new Zip_TreeView_ProxyModel(this);
private:
    QModelIndex proposed_action_index;
    QColor hover_color = QColor(227, 242, 253, 255);
    QColor select_color = QColor(0, 170, 255, 255);
    int radius = 10;
    int column_width1 = 150;
    int column_width2 = 150;
    int column_width3 = 150;
    int column_width4 = 150;
    int header_visual_index1 = 0;
    int header_visual_index2 = 1;
    int header_visual_index3 = 2;
    int header_visual_index4 = 3;
    int sort_section = 0;
    bool sort_order = true;
    char m_padding[7];
    My_TreeView_Delegate *my_delegate = new My_TreeView_Delegate(this, &hover_color, &select_color, &radius, &proposed_action_index);
private:
    QMenu *menu = new QMenu(this);
    QAction *copy_name_action = new Trans_Action(tr("复制名称"), "复制名称", this->metaObject()->className(), this);
    QAction *set_show_status_bar = new Trans_Action(tr("显示状态栏"), "显示状态栏", this->metaObject()->className(), this);
    QAction *set_show_status_bar_text_color = new Trans_Action(tr("设置状态栏字体颜色"), "设置状态栏字体颜色", this->metaObject()->className(), this);
    QAction *set_icon_size_action = new Trans_Action(tr("图标大小"), "图标大小", this->metaObject()->className(), this);
    QAction *set_font_action = new Trans_Action(tr("字体"), "字体", this->metaObject()->className(), this);
    QAction *set_hover_color = new Trans_Action(tr("悬停颜色"), "悬停颜色", this->metaObject()->className(), this);
    QAction *set_select_color = new Trans_Action(tr("选择颜色"), "选择颜色", this->metaObject()->className(), this);
    QAction *set_select_radius = new Trans_Action(tr("圆角大小"), "圆角大小", this->metaObject()->className(), this);

    //shortcut
    QAction *shortcut_copy_name_action = new Trans_Action(tr("复制名称"), "复制名称", this->metaObject()->className(), this);
private:
    virtual void wheelEvent(QWheelEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;


protected:
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void enterEvent(QEvent *event) override;
private:
    QRubberBand *m_rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    QPoint origin_pos = QPoint();
    bool setup_rubber = false;
    char my_padding[7];
public:
    QStatusBar *m_statusBar = new QStatusBar(this);
    QLabel *statusLabel = new QLabel(this);
    QColor statusBar_text_color = QColor(50, 50, 50, 255);
    void updateStatusBar_style();
    void updateStatusBar();
    void set_icon(QString checked_icon_path);
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
};

class Zip_TreeView_Carrier : public QWidget, public Zip_TreeView_Carrier_Interface
{
    Q_OBJECT
public:
    explicit Zip_TreeView_Carrier(QWidget *parent = nullptr);
    virtual void setupTar(const QFileInfo &info) override;
    virtual void clear() override;
    void load(QSettings *settings, QString Token);
    void save(QSettings *settings, QString Token);
    ~Zip_TreeView_Carrier() override;
    void set_icon(QString checked_icon_path);
signals:
    void loadingFinished(bool success);
private:
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;
private:
    Zip_TreeView *m_zip_treeview = new Zip_TreeView(this);
};

#endif // ZIP_TREEVIEW_H
