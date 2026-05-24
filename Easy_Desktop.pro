#***********************************************************************#
# Easy_Desktop.pro                                                      #
#                              GPL v3.0                                 #
#                  This file is part of: Easy_Desktop                   #
#          <https://github.com/3084793958/Easy_Desktop.git>             #
#***********************************************************************#
# Copyright (C) 2025-present github.com:3084793958                      #
#                                                                       #
# This program is free software: you can redistribute it and/or modify  #
# it under the terms of the GNU General Public License as published by  #
# the Free Software Foundation, either version 3 of the License, or     #
# (at your option) any later version.                                   #
#                                                                       #
# This program is distributed in the hope that it will be useful,       #
# but WITHOUT ANY WARRANTY; without even the implied warranty of        #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
# GNU General Public License for more details.                          #
#                                                                       #
# You should have received a copy of the GNU General Public License     #
# along with this program.  If not, see <https://www.gnu.org/licenses/>.#

#DEFINES += USE_DTK #使用DTK #dde-dock插件V2.0.0需要DTK支持,该项需要开启
DEFINES += USE_CHART #使用QChart

QT += core gui widgets multimedia multimediawidgets x11extras dbus pdf pdfwidgets svg
contains(DEFINES, USE_DTK) {
    QT += dtkcore dtkgui dtkwidget
}
contains(DEFINES, USE_CHART) {
    QT += charts
}

CONFIG += link_pkgconfig
PKGCONFIG += librsvg-2.0

LIBS += -lX11 -lXtst -lpulse
CONFIG += c++11
TARGET = Easy_Desktop
TEMPLATE = app
SOURCES += \
    main.cpp \
    desktop_background.cpp \
    all_control.cpp \
    desktop_main.cpp \
    setting_widget.cpp \
    keyscan.cpp \
    media_widgetaction.cpp \
    core/my_clock.cpp \
    core/basic_widget.cpp \
    core/my_lineedit.cpp \
    core/my_label.cpp \
    core/process_widget.cpp \
    core/file_widget.cpp \
    core/my_process_carrier.cpp \
    core/my_program_container.cpp \
    experimental_settings.cpp \
    core/file_tree.cpp \
    core/plugin_widget.cpp \
    core/tools/my_x11_libs.cpp \
    core/tools/file_control.cpp \
    core/tools/multilinetextinputdialog.cpp \
    core/module/preview_file_widget.cpp \
    core/tools/my_rsvg_support.cpp \
    interfaces/easy_desktop_class.cpp \
    core/module/plugincontroller.cpp \
    core/table_tree.cpp

contains(DEFINES, USE_CHART) {
    SOURCES += \
        core/chart/basic_chart.cpp \
        core/chart/cpu_chart.cpp \
        core/chart/ram_chart.cpp \
        core/chart/net_chart.cpp \
        core/chart/disk_chart.cpp \
        core/chart/pulseaudio_chart.cpp
}
contains(DEFINES, USE_DTK) {
    SOURCES += \
        core/module/plugincontroller_v_2_0_0.cpp
}

HEADERS += \
    desktop_background.h \
    all_control.h \
    desktop_main.h \
    setting_widget.h \
    keyscan.h \
    media_widgetaction.h \
    core/my_clock.h \
    core/basic_widget.h \
    core/tools/widget_control.hpp \
    core/my_lineedit.h \
    core/my_label.h \
    core/process_widget.h \
    core/file_widget.h \
    core/my_process_carrier.h \
    core/my_program_container.h \
    experimental_settings.h \
    core/file_tree.h \
    core/plugin_widget.h \
    interfaces/dde-dock/v1.2.3/constants.h \
    interfaces/dde-dock/v1.2.3/pluginproxyinterface.h \
    interfaces/dde-dock/v1.2.3/pluginsiteminterface.h \
    core/tools/my_x11_libs.h \
    core/tools/file_control.h \
    core/tools/multilinetextinputdialog.h \
    core/module/preview_file_widget.h \
    core/tools/my_rsvg_support.h \
    interfaces/file-preview/file_preview_plugin.h \
    interfaces/easy_desktop_class.h \
    interfaces/file-preview/preview_file_interface.h \
    core/module/plugincontroller.h \
    interfaces/media-widget-action/media_widgetaction_interface.h \
    core/table_tree.h

contains(DEFINES, USE_CHART) {
    HEADERS += \
        core/chart/basic_chart.h \
        core/chart/cpu_chart.h \
        core/chart/ram_chart.h \
        core/chart/net_chart.h \
        core/chart/disk_chart.h \
        core/chart/pulseaudio_chart.h
}

contains(DEFINES, USE_DTK) {
    HEADERS += \
        interfaces/dde-dock/v2.0.0/pluginproxyinterface_v2.0.0.h \
        interfaces/dde-dock/v2.0.0/pluginsiteminterface_v2.0.0.h \
        core/module/plugincontroller_v_2_0_0.h
}


RESOURCES += \
    base.qrc

TRANSLATIONS += translations/Original.ts
