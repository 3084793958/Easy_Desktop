#include "experimental_settings.h"
#include <QX11Info>
#include <X11/Xlib.h>
void Experimental_Settings::X11_Raise()
{
    if (!has_been_set)
    {
        return;
    }
    Window win_Id = static_cast<Window>(winId);
    Display *display = QX11Info::display();
    XRaiseWindow(display, win_Id);
    XFlush(display);
}
#undef CursorShape
Experimental_Settings::Experimental_Settings(QWidget *parent)
    :QWidget(parent)
{
    set_theme_dialog->hide();
    this->setMinimumSize(450, 235);
    this->setWindowTitle("实验设置");
    load_path_label->move(5, 5);
    load_path_label->resize(70, 40);
    load_path_edit->move(75, 5);
    load_path_edit->resize(230, 40);
    load_path_edit->setPlaceholderText(QDir::homePath() + "/.local/lib/easy_desktop/config.ini");
    set_load_path->move(310, 5);
    set_load_path->resize(85, 40);
    workspace_label->move(5, 50);
    workspace_label->resize(50, 40);
    workspace_box->move(60, 50);
    workspace_box->resize(165, 40);
    workspace_box->setRange(0, 2147483647);
    workspace_box->setValue(0);
    stay_on_top_box->move(5, 95);
    stay_on_top_box->resize(100, 40);
    stay_on_top_timer_label->move(110, 95);
    stay_on_top_timer_label->resize(100, 40);
    stay_on_top_timer_edit->setRange(10, 2147483647);
    stay_on_top_timer_edit->setValue(5000);
    stay_on_top_timer_edit->move(215, 95);
    stay_on_top_timer_edit->resize(180, 40);
    keyscan_label->move(5, 140);
    keyscan_label->resize(100, 40);
    keyscan_timer_edit->setRange(10, 2147483647);
    keyscan_timer_edit->setValue(20);
    keyscan_timer_edit->move(110, 140);
    keyscan_timer_edit->resize(180, 40);
    allow_drop_box->move(345, 140);
    allow_drop_box->resize(100, 40);
    dbus_id_label->move(230, 50);
    dbus_id_label->resize(60, 40);
    dbus_id_edit->move(290, 50);
    dbus_id_edit->resize(165, 40);
    dbus_id_edit->setRange(0, 2147483647);
    dbus_id_edit->setValue(0);
    file_open_way_label->move(5, 190);
    file_open_way_label->resize(150, 40);
    file_open_way_edit->move(160, 190);
    file_open_way_edit->resize(285 ,40);
    file_path_way_label->move(5, 240);
    file_path_way_label->resize(150, 40);
    file_path_way_edit->move(160, 240);
    file_path_way_edit->resize(285 ,40);
    file_info_way_label->move(5, 290);
    file_info_way_label->resize(150, 40);
    file_info_way_edit->move(160, 290);
    file_info_way_edit->resize(190 ,40);
    geometry_label->move(5, 340);
    geometry_label->resize(70, 40);
    always_refresh->move(80, 340);
    always_refresh->resize(90, 40);
    geometry_edit->move(175, 340);
    geometry_edit->resize(170, 40);
    set_theme_color_button->move(310, 290);
    set_theme_color_button->resize(85, 40);
    update_button->move(310, 340);
    update_button->resize(85, 40);
    resize(450, 385);
    connect(update_button, &QPushButton::clicked, this, &Experimental_Settings::send_data);
    connect(set_load_path, &QPushButton::clicked, this, [=]
    {
        QString filename = QFileDialog::getOpenFileName(nullptr, "获取配置文件", QDir::homePath(), "配置文件(*.ini);;所有文件(*.*)");
        X11_Raise();
        if (!filename.isEmpty())
        {
            load_path_edit->setText(filename);
        }
    });
    connect(set_theme_color_button, &QPushButton::clicked, this, [=]
    {
        set_theme_dialog->activateWindow();
        set_theme_dialog->show();
    });
    connect(set_theme_dialog, &Theme_Set_Dialog::Call_X11_Raise, this, &Experimental_Settings::X11_Raise);
}
void Experimental_Settings::resizeEvent(QResizeEvent *event)
{
    set_load_path->move(event->size().width() - 90, 5);
    load_path_edit->resize(event->size().width() - 170, 40);
    update_button->move(event->size().width() - 90, 340);
    set_theme_color_button->move(event->size().width() - 90, 290);
    dbus_id_label->move(event->size().width() / 2 + 5, 50);
    dbus_id_edit->move(event->size().width() / 2 + 65, 50);
    dbus_id_edit->resize(event->size().width() / 2 - 70, 40);
    stay_on_top_timer_edit->resize(event->size().width() - 220, 40);
    keyscan_timer_edit->resize(event->size().width() - 220, 40);
    allow_drop_box->move(event->size().width() - 105, 140);
    workspace_box->resize(event->size().width() / 2 - 60, 40);
    geometry_edit->resize(event->size().width() - 280, 40);
    file_open_way_edit->resize(event->size().width() - 165, 40);
    file_path_way_edit->resize(event->size().width() - 165, 40);
    file_info_way_edit->resize(event->size().width() - 260, 40);
}
void Experimental_Settings::update_data()
{
    load_path_edit->setText(*load_path);
    dbus_id_edit->setValue(*dbus_id);
    keyscan_timer_edit->setValue(*keyscan_timer);
    stay_on_top_box->setCheckState(*stay_on_top ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    stay_on_top_timer_edit->setValue(*on_top_time);
    allow_drop_box->setCheckState(*allow_drop ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    workspace_box->setValue(*workspace);
    always_refresh->setCheckState(*always_refresh_geometry ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    geometry_edit->setText(QString("%1,%2,%3,%4").arg(geometry->x()).arg(geometry->y()).arg(geometry->width()).arg(geometry->height()));
    file_open_way_edit->setText(*file_open_way_process);
    file_path_way_edit->setText(*file_open_path_process);
    file_info_way_edit->setText(*file_open_info_process);
    m_theme_color = *theme_color;
    m_theme_background_color = *theme_background_color;
    m_theme_text_color = *theme_text_color;
    m_select_text_color = *select_text_color;
    m_disabled_text_color = *disabled_text_color;
    m_checked_icon_path = *checked_icon_path;
    set_theme_dialog->update_data();
}
void Experimental_Settings::send_data()
{
    *load_path = load_path_edit->text();
    *dbus_id = dbus_id_edit->value();
    *keyscan_timer = keyscan_timer_edit->value();
    *stay_on_top = stay_on_top_box->checkState() == Qt::CheckState::Checked;
    *allow_drop = allow_drop_box->checkState() == Qt::CheckState::Checked;
    *on_top_time = stay_on_top_timer_edit->value();
    *workspace = workspace_box->value();
    *always_refresh_geometry = always_refresh->checkState() == Qt::CheckState::Checked;
    if (*always_refresh_geometry)
    {
        *geometry = QApplication::desktop()->geometry();
        geometry_edit->setText(QString("%1,%2,%3,%4").arg(geometry->x()).arg(geometry->y()).arg(geometry->width()).arg(geometry->height()));
    }
    else
    {
        QStringList list = geometry_edit->text().split(",");
        QRect screen_geometry = QApplication::desktop()->geometry();
        if (list.count() == 4)
        {
            int x = list[0].toInt();
            int y = list[1].toInt();
            int width = list[2].toInt();
            int height = list[3].toInt();
            if (width == 0 || height == 0)
            {
                screen_geometry = QRect(x, y, screen_geometry.width(), screen_geometry.height());
            }
            else
            {
                screen_geometry = QRect(x, y, width, height);
            }
        }
        *geometry = screen_geometry;
    }
    *file_open_way_process = file_open_way_edit->text();
    *file_open_path_process = file_path_way_edit->text();
    *file_open_info_process = file_info_way_edit->text();
    *theme_color = m_theme_color;
    *theme_background_color = m_theme_background_color;
    *theme_text_color = m_theme_text_color;
    *select_text_color = m_select_text_color;
    *disabled_text_color = m_disabled_text_color;
    *checked_icon_path = m_checked_icon_path;
    emit has_sended();
}
Theme_Set_Dialog::Theme_Set_Dialog(QWidget *parent, QColor *m_theme_color, QColor *m_theme_background_color, QColor *m_theme_text_color, QColor *m_select_text_color, QColor *m_disabled_text_color, QString *m_checked_icon_path)
    :QWidget(parent)
    ,theme_color(m_theme_color)
    ,theme_background_color(m_theme_background_color)
    ,theme_text_color(m_theme_text_color)
    ,select_text_color(m_select_text_color)
    ,disabled_text_color(m_disabled_text_color)
    ,checked_icon_path(m_checked_icon_path)
{
    hide();
    checked_icon_path_box->setEditable(true);
    checked_icon_path_box->addItems(QStringList() << ":/base/this.svg" << ":/base/this_white.svg" << ":/base/empty.svg" << ":/base/select.svg");
    connect(checked_icon_path_box, &QComboBox::currentTextChanged, this, [=]{*checked_icon_path = checked_icon_path_box->currentText();});
    connect(theme_color_button, &QPushButton::clicked, this, [=]{set_color(theme_color);});
    connect(theme_background_color_button, &QPushButton::clicked, this, [=]{set_color(theme_background_color);});
    connect(theme_text_color_button, &QPushButton::clicked, this, [=]{set_color(theme_text_color);});
    connect(select_text_color_button, &QPushButton::clicked, this, [=]{set_color(select_text_color);});
    connect(disabled_text_color_button, &QPushButton::clicked, this, [=]{set_color(disabled_text_color);});
    theme_color_label->move(5, 5);
    theme_color_label->resize(50, 40);
    theme_color_button->move(60, 5);
    theme_color_button->resize(40, 40);
    theme_background_color_label->move(120, 5);
    theme_background_color_label->resize(50, 40);
    theme_background_color_button->move(175, 5);
    theme_background_color_button->resize(40, 40);
    theme_text_color_label->move(235, 5);
    theme_text_color_label->resize(50, 40);
    theme_text_color_button->move(290, 5);
    theme_text_color_button->resize(40, 40);
    select_text_color_label->move(5, 50);
    select_text_color_label->resize(75, 40);
    select_text_color_button->move(80, 50);
    select_text_color_button->resize(40, 40);
    disabled_text_color_label->move(140, 50);
    disabled_text_color_label->resize(75, 40);
    disabled_text_color_button->move(215, 50);
    disabled_text_color_button->resize(40, 40);
    checked_icon_path_label->move(5, 95);
    checked_icon_path_label->resize(50, 40);
    checked_icon_path_box->move(60, 95);
    checked_icon_path_box->resize(180, 40);
    checked_icon_path_load_button->move(245, 95);
    checked_icon_path_load_button->resize(85, 40);
    connect(checked_icon_path_load_button, &QPushButton::clicked, this, [=]
    {
        QString filename = QFileDialog::getOpenFileName(nullptr, "获取图像", QDir::homePath(), "图像文件(*.png *.jpg *.jpeg *.svg *.gif *.bmp);;所有文件(*.*)");
        emit Call_X11_Raise();
        if (!filename.isEmpty())
        {
            *checked_icon_path = filename;
            update_data();
        }
    });
    this->setFocusProxy(checked_icon_path_box);
    resize(335, 150);
}
void Theme_Set_Dialog::update_data()
{
    if (theme_color)
    theme_color_button->setStyleSheet(QString("background-color:rgba(%1,%2,%3,%4)")
                                      .arg(theme_color->red()).arg(theme_color->green()).arg(theme_color->blue()).arg(theme_color->alpha()));
    if (theme_background_color)
    theme_background_color_button->setStyleSheet(QString("background-color:rgba(%1,%2,%3,%4)")
                                                 .arg(theme_background_color->red()).arg(theme_background_color->green()).arg(theme_background_color->blue()).arg(theme_background_color->alpha()));
    if (theme_text_color)
    theme_text_color_button->setStyleSheet(QString("background-color:rgba(%1,%2,%3,%4)")
                                           .arg(theme_text_color->red()).arg(theme_text_color->green()).arg(theme_text_color->blue()).arg(theme_text_color->alpha()));
    if (select_text_color)
    select_text_color_button->setStyleSheet(QString("background-color:rgba(%1,%2,%3,%4)")
                                            .arg(select_text_color->red()).arg(select_text_color->green()).arg(select_text_color->blue()).arg(select_text_color->alpha()));
    if (disabled_text_color)
    disabled_text_color_button->setStyleSheet(QString("background-color:rgba(%1,%2,%3,%4)")
                                              .arg(disabled_text_color->red()).arg(disabled_text_color->green()).arg(disabled_text_color->blue()).arg(disabled_text_color->alpha()));
    if (checked_icon_path)
    checked_icon_path_box->setCurrentText(*checked_icon_path);
}
void Theme_Set_Dialog::set_color(QColor *ptr)
{
    QColorDialog colorDialog;
    colorDialog.setOption(QColorDialog::ShowAlphaChannel);
    colorDialog.setCurrentColor(*ptr);
    colorDialog.setParent(nullptr);
    colorDialog.setWindowTitle("获取颜色");
    if (colorDialog.exec() != QDialog::Accepted)
    {
        return;
    }
    *ptr = colorDialog.currentColor();
    update_data();
}
