QT += core gui widgets multimedia multimediawidgets x11extras
LIBS += -lX11 -lXtst
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
    core/my_program_container.cpp

HEADERS += \
    desktop_background.h \
    all_control.h \
    desktop_main.h \
    setting_widget.h \
    keyscan.h \
    media_widgetaction.h \
    core/my_clock.h \
    core/basic_widget.h \
    core/widget_control.hpp \
    core/my_lineedit.h \
    core/my_label.h \
    core/process_widget.h \
    core/file_widget.h \
    core/my_process_carrier.h \
    core/my_program_container.h

RESOURCES += \
    base.qrc
