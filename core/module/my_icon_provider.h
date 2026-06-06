#ifndef MY_ICON_PROVIDER_H
#define MY_ICON_PROVIDER_H
#include <QFileIconProvider>
#include <QIcon>

class My_Icon_Provider : public QFileIconProvider
{
public:
    My_Icon_Provider() = default;
    QIcon icon(IconType type) const override;
    QIcon icon(const QFileInfo &info) const override;
    QSize get_Image_Size(QString path) const;
};

#endif // MY_ICON_PROVIDER_H
