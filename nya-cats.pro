#-------------------------------------------------
#
# Project created by QtCreator 2012-03-08T16:40:25
#
#-------------------------------------------------

QT       += core gui

TARGET = nya-cats
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    core/nslot.cpp \
    core/nframe.cpp \
    core/nkbmanager.cpp \
    core/nframenetmode.cpp \
    ui/nobjectpropertieseditor.cpp \
    editor/lgen2objectpropertieseditor.cpp \
    editor/lgen2diagrameditor.cpp

HEADERS  += mainwindow.hpp \
    core/nslot.h \
    core/nframe.h \
    core/nkbmanager.h \
    core/nframenetmode.h \
    ui/nobjectpropertieseditor.h \
    editor/lgen2objectpropertieseditor.hpp \
    editor/lgen2diagrameditor.hpp

FORMS    += mainwindow.ui
