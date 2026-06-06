#include "my_icon_provider.h"

#include <QImageReader>
#include <QMimeDatabase>
#include <QSettings>

QIcon My_Icon_Provider::icon(QFileIconProvider::IconType type) const
{
    return QFileIconProvider::icon(type);
}
QSize My_Icon_Provider::get_Image_Size(QString path) const
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
QIcon My_Icon_Provider::icon(const QFileInfo &info) const
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
            if (!icon.isNull() && My_Icon_Provider::get_Image_Size(info.filePath()) != QSize(0, 0))
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
