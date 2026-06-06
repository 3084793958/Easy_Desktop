#ifndef WALLPAPER_PLUGIN_H
#define WALLPAPER_PLUGIN_H

#include "interfaces/easy_desktop_class.h"

#include <QColor>
#include <QSize>
#include "interfaces/wallpaper/wallpaper_info_def.h"

class Ext_Wallpaper_PluginInterface
{
public:
    //插件需要在json中说明自己支持Ext_Wallpaper_Plugin
    //格式:   "Ext_Wallpaper_Plugin": ""
    //ADD
    const QString Ext_Name = "Easy_Desktop_Wallpaper";
    const QString Ext_Spec = "";
    //ADD

    P_Version Plugin_Version{0, 0, 1};//不使用const,用于欺骗Easy_Desktop

    ///
    /// \brief inited
    /// Desktop_Background会处理,你不需要管这个变量
    ///
    bool inited = false;

    //0.0.1
    ///
    /// \brief ~Ext_Wallpaper_PluginInterface()
    /// 就是正常的析构函数
    ///
    virtual ~Ext_Wallpaper_PluginInterface() {}
    ///
    /// \brief RemovePlugin
    /// 移除该插件,你需要做的是停止控件/释放内存[必须实现]
    ///
    virtual void RemovePlugin() = 0;
    ///
    /// \brief init(const Path_Info &info, P_Sender *send_position_sender)
    /// 初始化时传递用户设定的信息,其中,info包含wallpaper信息,send_position_sender为更新进度条的sender,使用Send_Data(QList<QVariant>() << int value << QString text)
    /// \param info
    /// \paramsend_position_sender
    ///
    virtual void init(const Path_Info &info, P_Sender *send_position_sender)
    {
        (void) info;
        (void) send_position_sender;
    }
    ///
    /// \brief update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, const QString & checked_icon_path)
    /// 设置style
    /// \param theme_color
    /// \param theme_background_color
    /// \param theme_text_color
    /// \param select_text_color
    /// \param disabled_text_color
    /// \param checked_icon_path
    ///
    virtual void update_style(QColor theme_color, QColor theme_background_color, QColor theme_text_color, QColor select_text_color, QColor disabled_text_color, const QString & checked_icon_path)
    {
        (void) theme_color;
        (void) theme_background_color;
        (void) theme_text_color;
        (void) select_text_color;
        (void) disabled_text_color;
        (void) checked_icon_path;
    }
    ///
    /// \brief mouseMoveEvent(int mouse_x, int mouse_y)
    /// 当鼠标运动时,传递鼠标信息
    /// \param mouse_x
    /// \param mouse_y
    ///
    virtual void mouseMoveEvent(int mouse_x, int mouse_y)
    {
        (void) mouse_x;
        (void) mouse_y;
    }
    ///
    /// \brief play
    /// 播放时触发
    ///
    virtual void play()
    {}
    ///
    /// \brief pause
    /// 暂停时触发
    ///
    virtual void pause()
    {}
    ///
    /// \brief setPosition
    /// 进度条更改时触发
    /// \param value
    ///
    virtual void setPosition(int value)
    {
        (void) value;
    }
    ///
    /// \brief setVolume
    /// 音量更改时触发
    /// \param value
    ///
    virtual void setVolume(int value)
    {
        (void) value;
    }
    ///
    /// \brief setSpeed
    /// 倍速更改时触发
    /// \param value
    ///
    virtual void setSpeed(int value)
    {
        (void) value;
    }
    ///
    /// \brief sizeChange
    /// 大小改变时触发
    /// \param size
    ///
    virtual void sizeChange(QSize size)
    {
        (void) size;
    }
    ///
    /// \brief wallpaperItem
    /// 给出一个wallpaper的实现,Easy_Desktop会将其渲染[必须实现]
    /// \return
    ///
    virtual QWidget *wallpaperItem() = 0;
};

QT_BEGIN_NAMESPACE

Q_DECLARE_INTERFACE(Ext_Wallpaper_PluginInterface, "com.Easy.Desktop.Ext_Wallpaper_PluginInterface")

QT_END_NAMESPACE

#endif // WALLPAPER_PLUGIN_H
