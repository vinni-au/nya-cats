#-------------------------------------------------
#
# Project created by QtCreator 2012-03-08T16:40:25
#
#-------------------------------------------------

QT       += core gui xml

TARGET = nya-cats
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    core/nslot.cpp \
    core/nframe.cpp \
    core/nkbmanager.cpp \
    editor/lgen2objectpropertieseditor.cpp \
    editor/lgen2diagrameditor.cpp \
    core/nframenetmodel.cpp \
    core/nfaset.cpp \
    core/nproc.cpp \
    core/domains/domainnode.cpp \
    core/domains/domainmodel.cpp \
    core/domains/domain.cpp \
    core/domains/validator.cpp

HEADERS  += mainwindow.hpp \
    core/nslot.h \
    core/nframe.h \
    core/nkbmanager.h \
    editor/lgen2objectpropertieseditor.hpp \
    editor/lgen2diagrameditor.hpp \
    core/nframenetmodel.h \
    core/nfaset.h \
    core/nproc.h \
    core/domains/domainnode.h \
    core/domains/domainmodel.h \
    core/domains/domain.h \
    core/domains/validator.h

FORMS    += mainwindow.ui








