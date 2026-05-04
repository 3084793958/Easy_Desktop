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

#DEFINES += USE_DTK
DEFINES += USE_CHART
QT += core gui widgets multimedia multimediawidgets x11extras dbus pdf pdfwidgets svg
contains(DEFINES, USE_DTK) {
    QT += dtkcore dtkgui dtkwidget
}
contains(DEFINES, USE_CHART) {
    QT += charts
}
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
    interfaces/pluginsiteminterface.cpp \
    core/tools/my_x11_libs.cpp \
    core/tools/file_control.cpp \
    core/tools/multilinetextinputdialog.cpp \
    core/tools/preview_file_widget.cpp
contains(DEFINES, USE_CHART) {
    SOURCES += \
        core/chart/basic_chart.cpp \
        core/chart/cpu_chart.cpp \
        core/chart/ram_chart.cpp \
        core/chart/net_chart.cpp \
        core/chart/disk_chart.cpp \
        core/chart/pulseaudio_chart.cpp
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
    interfaces/constants.h \
    interfaces/pluginproxyinterface.h \
    interfaces/pluginsiteminterface.h \
    core/tools/my_x11_libs.h \
    core/tools/file_control.h \
    core/tools/multilinetextinputdialog.h \
    core/tools/preview_file_widget.h

contains(DEFINES, USE_CHART) {
    HEADERS += \
        core/chart/basic_chart.h \
        core/chart/cpu_chart.h \
        core/chart/ram_chart.h \
        core/chart/net_chart.h \
        core/chart/disk_chart.h \
        core/chart/pulseaudio_chart.h
}

RESOURCES += \
    base.qrc

TRANSLATIONS += translations/Original.ts
