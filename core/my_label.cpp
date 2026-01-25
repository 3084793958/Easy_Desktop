#include "my_label.h"
#include <QX11Info>
#include <X11/Xlib.h>
void My_Label::X11_Rasie()
{
    Window win_Id = static_cast<Window>(winId);
    Display *display = QX11Info::display();
    XRaiseWindow(display, win_Id);
    XFlush(display);
}
#undef CursorShape
void My_Label::set_WinId(WId m_winId)
{
    winId = m_winId;
}
QSize My_Label::get_Image_Size(QString path)
{
    if (path.isEmpty())
    {
        return QSize(0,0);
    }
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
My_Label::My_Label(QWidget *parent)
    :Basic_Widget(parent)
{
    this->main_label->setMovie(movie);
    this->main_label->setText(">(O^<)<~GET!");//BASIC
    menu->addAction(set_image);
    menu->addAction(set_text);
    menu->addAction(set_font);
    menu->addAction(set_text_color);
    set_out_line_menu->addAction(set_out_line_width);
    set_out_line_menu->addAction(set_out_line_color);
    menu->addMenu(set_out_line_menu);
    menu->addAction(clear_label);
    set_dbus_service->addAction(dbus_setup_action);
    dbus_setup_action->setIcon(QIcon(":/base/this.svg"));
    dbus_setup_action->setIconVisibleInMenu(false);
    set_dbus_service->addAction(set_dbus_info);
    menu->addMenu(set_dbus_service);
    Basic_Widget::basic_context(menu);
    main_label->setMouseTracking(true);
    main_label->setAlignment(Qt::AlignCenter);
    main_label->outlineWidth = 5;
    main_label->outlineColor = Qt::GlobalColor::black;
    main_label->text_color = Qt::GlobalColor::white;
    main_label->setStyleSheet("background:rgba(0,0,0,0)");
    main_label->setFont(QFontDatabase::systemFont(QFontDatabase::FixedFont));
    connect(this, &Basic_Widget::size_changed, this, [=](QSize size)
    {
        this->main_label->resize(size);
        auto_set_font_size();
        if (image_size.isValid())
        {
            float k_width = float(size.width()) / image_size.width();
            float k_height = float(size.height()) / image_size.height();
            if (k_width < k_height)
            {
                movie->setScaledSize(QSize(size.width(), int(image_size.height() * k_width)));
            }
            else
            {
                movie->setScaledSize(QSize(int(image_size.width() * k_height), size.height()));
            }
            this->main_label->setMovie(movie);
            if (movie->fileName() != nullptr)
            {
                movie->setPaused(true);
                movie->jumpToNextFrame();//刷新+配平
                movie->setPaused(false);
            }
        }
    });
    resize(200, 100);
    show();
    dbus.disconnect("", "", "", "", this, nullptr);
}

My_Label::~My_Label()
{
    dbus.disconnect("", "", "", "", this, nullptr);
    if (my_label_list)
    {
        my_label_list->removeOne(this);
    }
}
void My_Label::contextMenuEvent(QContextMenuEvent *event)
{
    QAction *know_what = menu->exec(mapToGlobal(event->pos()));
    if (know_what == set_image)
    {
        QString filename = QFileDialog::getOpenFileName(nullptr, "获取图像", QDir::homePath(), "图像文件(*.png *.jpg *.jpeg *.svg *.gif *.bmp);;所有文件(*.*)");
        My_Label::X11_Rasie();
        if (filename.isEmpty())
        {
            return;
        }
        image_size = get_Image_Size(filename);
        if (image_size.isValid())
        {
            movie->setFileName(filename);
            float k_width = float(this->main_label->width()) / image_size.width();
            float k_height = float(this->main_label->height()) / image_size.height();
            if (k_width < k_height)
            {
                movie->setScaledSize(QSize(this->main_label->width(), int(image_size.height() * k_width)));
            }
            else
            {
                movie->setScaledSize(QSize(int(image_size.width() * k_height), this->main_label->height()));
            }
            this->main_label->setText("");
            this->main_label->setMovie(movie);
            movie->start();
        }
    }
    else if (know_what == set_text)
    {
        QInputDialog dialog;
        dialog.setParent(nullptr);
        dialog.setWindowTitle("获取文字");
        dialog.setLabelText("输入文字");
        dialog.setTextValue(this->main_label->text());
        dialog.setInputMode(QInputDialog::TextInput);
        dialog.setOption(QInputDialog::UsePlainTextEditForTextInput);
        QFont final_font = this->main_label->font();
        final_font.setPointSize(dialog.font().pointSize());
        dialog.setFont(final_font);
        if (dialog.exec() != QDialog::Accepted)
        {
            return;
        }
        movie->setFileName("");
        image_size = QSize(-1, -1);
        this->main_label->setMovie(nullptr);
        this->main_label->setText(dialog.textValue());
        auto_set_font_size();
    }
    else if (know_what == set_font)
    {
        bool ok = false;
        QFont font = QFontDialog::getFont(&ok, main_label->font(), nullptr);
        if (!ok)
        {
            return;
        }
        main_label->setFont(font);
        auto_set_font_size();
    }
    else if (know_what == set_text_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(this->main_label->text_color);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle("获取颜色");
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        this->main_label->text_color = colorDialog.currentColor();
    }
    else if (know_what == set_out_line_color)
    {
        QColorDialog colorDialog;
        colorDialog.setOption(QColorDialog::ShowAlphaChannel);
        colorDialog.setCurrentColor(this->main_label->outlineColor);
        colorDialog.setParent(nullptr);
        colorDialog.setWindowTitle("获取颜色");
        if (colorDialog.exec() != QDialog::Accepted)
        {
            return;
        }
        this->main_label->outlineColor = colorDialog.currentColor();
    }
    else if (know_what == set_out_line_width)
    {
        bool ok = false;
        int out_line_width = QInputDialog::getInt(nullptr, "获取数值", "描边大小", this->main_label->outlineWidth, 1, 2147483647, 1, &ok);
        if (!ok)
        {
            return;
        }
        this->main_label->outlineWidth = out_line_width;
        auto_set_font_size();
    }
    else if (know_what == clear_label)
    {
        movie->setFileName("");
        image_size = QSize(-1, -1);
        this->main_label->setMovie(nullptr);
        this->main_label->setText("");
    }
    else if (know_what == dbus_setup_action)
    {
        dbus_setup_action->setIconVisibleInMenu(!dbus_setup_action->isIconVisibleInMenu());
        dbus_setup = dbus_setup_action->isIconVisibleInMenu();
    }
    else if (know_what == set_dbus_info)
    {
        QString m_dbus_service = "";
        QString m_dbus_path = "";
        QString m_dbus_interface = "";
        QString m_dbus_name = "";
        bool ok = false;
        m_dbus_service = QInputDialog::getText(nullptr, "获取service", "获取service(dbus服务名)", QLineEdit::Normal, dbus_service, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        m_dbus_path = QInputDialog::getText(nullptr, "获取path", "获取path(dbus路径)", QLineEdit::Normal, dbus_path, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        m_dbus_interface = QInputDialog::getText(nullptr, "获取interface", "获取interface(dbus接口)", QLineEdit::Normal, dbus_interface, &ok);
        if (!ok)
        {
            return;
        }
        ok = false;
        m_dbus_name = QInputDialog::getText(nullptr, "获取name", "获取name(dbus信号名)", QLineEdit::Normal, dbus_name, &ok);
        if (!ok)
        {
            return;
        }
        dbus_service = m_dbus_service;
        dbus_path = m_dbus_path;
        dbus_interface = m_dbus_interface;
        dbus_name = m_dbus_name;
        dbus.disconnect("", "", "", "", this, nullptr);
        dbus.connect(dbus_service, dbus_path, dbus_interface, dbus_name, this, SLOT(DBusMessageReceived(QDBusMessage)));
    }
    else
    {
        Basic_Widget::basic_action_func(know_what);
    }
}
void My_Label::DBusMessageReceived(QDBusMessage message)
{
    if (!dbus_setup)
    {
        return;
    }
    QList<QVariant> args = message.arguments();
    for (QVariant arg : args)
    {
        if (arg.canConvert<QString>())
        {
            this->main_label->setText(arg.toString());
            auto_set_font_size();
            break;
        }
    }
}
void My_Label::auto_set_font_size()
{
    if (main_label->text().isEmpty())
    {
        return;
    }
    QFont font = main_label->font();
    const QRect label_rect = main_label->contentsRect();
    int font_size = 1;
    int step = 5;
    while (step > 0 && font_size < 250)
    {
        font.setPointSize(font_size);
        QFontMetrics fm(font);
        QRect text_rect = fm.boundingRect(label_rect, static_cast<int>(main_label->alignment() | Qt::TextWordWrap), main_label->text());
        if (text_rect.height() <= label_rect.height() && text_rect.width() <= label_rect.width())
        {
            font_size += step;
        }
        else
        {
            font_size -= step;
            step /= 2;
        }
    }
    font.setPointSize(std::max(1, font_size - 1));
    main_label->setFont(font);
}
void My_Label::save(QSettings *settings)
{
    Basic_Widget::save(settings);
    settings->setValue("image_path", movie->fileName());
    settings->setValue("label_text", main_label->text());
    settings->setValue("text_color", main_label->text_color.rgba());
    settings->setValue("text_font", main_label->font());
    settings->setValue("out_line_width", main_label->outlineWidth);
    settings->setValue("out_line_color", main_label->outlineColor.rgba());
    settings->setValue("dbus_service", dbus_service);
    settings->setValue("dbus_path", dbus_path);
    settings->setValue("dbus_interface", dbus_interface);
    settings->setValue("dbus_name", dbus_name);
    settings->setValue("dbus_setup", dbus_setup);
}
void My_Label::load(QSettings *settings)
{
    Basic_Widget::load(settings);
    QString image_path = settings->value("image_path", "").toString();
    QString label_text = settings->value("label_text", "").toString();
    image_size = get_Image_Size(image_path);
    if (label_text.isEmpty())
    {
        movie->setFileName(image_path);
        float k_width = float(this->main_label->width()) / image_size.width();
        float k_height = float(this->main_label->height()) / image_size.height();
        if (k_width < k_height)
        {
            movie->setScaledSize(QSize(this->main_label->width(), int(image_size.height() * k_width)));
        }
        else
        {
            movie->setScaledSize(QSize(int(image_size.width() * k_height), this->main_label->height()));
        }
        this->main_label->setText("");
        this->main_label->setMovie(movie);
        if (movie->fileName() != nullptr)
        {
            movie->start();
        }
    }
    else
    {
        this->main_label->setText(label_text);
    }
    main_label->text_color = QColor::fromRgba(settings->value("text_color", QColor(255, 255, 255, 255).rgba()).toUInt());
    main_label->setFont(settings->value("text_font", QFontDatabase::systemFont(QFontDatabase::FixedFont)).value<QFont>());
    main_label->outlineWidth = settings->value("out_line_width", 5).toInt();
    main_label->outlineColor = QColor::fromRgba(settings->value("out_line_color", QColor(0, 0, 0, 255).rgba()).toUInt());
    dbus_service = settings->value("dbus_service", "").toString();
    dbus_path = settings->value("dbus_path", "").toString();
    dbus_interface = settings->value("dbus_interface", "").toString();
    dbus_name = settings->value("dbus_name", "").toString();
    dbus_setup = settings->value("dbus_setup", false).toBool();
    dbus_setup_action->setIconVisibleInMenu(dbus_setup);
    dbus.disconnect("", "", "", "", this, nullptr);
    dbus.connect(dbus_service, dbus_path, dbus_interface, dbus_name, this, SLOT(DBusMessageReceived(QDBusMessage)));
    auto_set_font_size();
}
