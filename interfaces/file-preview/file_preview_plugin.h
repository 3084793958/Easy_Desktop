#ifndef FILE_PREVIEW_PLUGIN_H
#define FILE_PREVIEW_PLUGIN_H

#include "interfaces/easy_desktop_class.h"
#include "preview_file_interface.h"

#include <QColor>

class QPluginLoader;

class Ext_Preview_PluginInterface
{
public:
    //插件需要在json中说明自己支持Ext_Preview_Plugin
    //格式:   "Ext_Preview_Plugin": ""
    //ADD
    const QString Ext_Name = "Easy_Desktop_Preview";
    const QString Ext_Spec = "";
    //ADD

    P_Version Plugin_Version{0, 0, 3};//不使用const,用于欺骗Easy_Desktop

    ///
    /// \brief inited
    /// Preview_File_Widget会处理,你不需要管这个变量
    ///
    bool inited = false;
    ///
    /// \brief your_plugin_loader
    /// Preview_File_Widget会处理,你不需要管这个变量
    ///
    QPluginLoader *your_plugin_loader = nullptr;
    ///
    /// \brief plugin_path
    /// Preview_File_Widget会处理,你不需要管这个变量
    ///
    QString plugin_path = "";

    //0.0.1
    ///
    /// \brief ~Ext_Preview_PluginInterface()
    /// 就是正常的析构函数
    ///
    virtual ~Ext_Preview_PluginInterface() {}
    ///
    /// \brief RemovePlugin
    /// 移除该插件,你需要做的是停止控件/释放内存[必须实现]
    ///
    virtual void RemovePlugin() = 0;
    ///
    /// \brief Plugin_Name
    /// 插件名称[必须实现]
    /// \return
    ///
    virtual QString Plugin_Name() = 0;
    ///
    /// \brief init(Preview_File_Interface *ptr, QWidget *carrier)
    /// 初始化时传递指针.ptr:Preview_File_Widget的简化接口,通过接口获取控件,基本够用了 carrier:控件的容器
    /// \param ptr
    /// \param carrier
    ///
    ///
    virtual void init(Preview_File_Interface *ptr, QWidget *carrier)
    {
        (void) ptr;
        (void) carrier;
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
    /// \brief clear()
    /// 当切换预览文件时,该函数会被调用[必须实现]
    /// clear()中不能调用clearCurrentPreview(),这是个死循环
    ///
    virtual void clear() = 0;
    ///
    /// \brief previewFile
    /// \param info
    /// 设置需要预览的文件,返回是否预览[必须实现]
    /// \return
    ///
    virtual bool previewFile(QFileInfo &info) = 0;
    ///
    /// \brief previewItem
    /// 给出预览体的指针,以便渲染.默认返回nullptr.如果使用Preview_File_Widget的默认控件,返回nullptr即可
    /// \return
    ///
    virtual QWidget *previewItem()
    {
        return nullptr;
    }
    ///
    /// \brief willThrowFileToNextPreview()
    /// 当previewFile(QFileInfo &info)返回true时,才会判定.默认返回false.若为true,会将文件扔给下一个预览体去preview
    /// \return
    ///
    virtual bool willThrowFileToNextPreview()
    {
        return false;
    }
    ///
    /// \brief resetZoom
    /// 重置缩放,对于类似图像查看器的预览体使用
    ///
    virtual void resetZoom()
    {}
    //0.0.1

    //0.0.2
    ///
    /// \brief init_V_0_0_2
    /// 用于V0.0.2版本的拓展,给你一个V0.0.2的Interface,可调用V0.0.2支持的函数
    /// \param ptr
    ///
    virtual void init_V_0_0_2(Preview_File_Interface_V_0_0_2 *ptr)
    {
        (void) ptr;
    }
    //0.0.2

    //0.0.3
    ///
    /// \brief init_V_0_0_3
    /// 用于V0.0.3版本的拓展,给你一个V0.0.3的Interface,可调用V0.0.3支持的函数.
    /// \param ptr
    ///
    virtual void init_V_0_0_3(Preview_File_Interface_V_0_0_3 *ptr)
    {
        (void) ptr;
    }
    ///
    /// \brief Trans_Update
    /// 告诉插件:翻译动态更新了
    ///
    virtual void Trans_Update()
    {}
    //0.0.3
};

QT_BEGIN_NAMESPACE

Q_DECLARE_INTERFACE(Ext_Preview_PluginInterface, "com.Easy.Desktop.Ext_Preview_PluginInterface")

QT_END_NAMESPACE

#endif // FILE_PREVIEW_PLUGIN_H
