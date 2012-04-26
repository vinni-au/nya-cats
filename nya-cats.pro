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
    editor/arrow.cpp \
    editor/diagramitem.cpp \
    editor/diagramscene.cpp \
    core/frame_model/nframenetmodel.cpp \
    core/nfaset.cpp \
    core/nproc.cpp \
    core/domains/domainnode.cpp \
    core/domains/domainmodel.cpp \
    core/domains/domain.cpp \
    core/validator.cpp \
    ui/mylistview.cpp \
    ui/domainwnd.cpp \
    ui/saver.cpp \
    core/production/nproduction.cpp \
    core/production/rulenode.cpp \
    core/production/rulemodel.cpp \
    core/production/rule.cpp \
    core/production/expr.cpp \
    ui/ruleswnd.cpp \
    ui/mycombobox.cpp \
    ui/labeledtextbox.cpp \
    ui/expreditor.cpp \
    ui/frameeditorwnd.cpp \
    ui/sloteditorwnd.cpp \
    core/frame_model/nframenode.cpp \
    ui/kbeditorwindow.cpp \
    visualize/view.cpp \
    visualize/visualizer.cpp \
    visualize/gamescene.cpp \
    visualize/grid.cpp \
    visualize/cell.cpp \
    visualize/gameitemfactory.cpp \
    visualize/gameitem.cpp \
    core/production/nproductionmlv.cpp \
    mlv/mlv.cpp

HEADERS  += mainwindow.hpp \
    core/nslot.h \
    core/nframe.h \
    core/nkbmanager.h \
    editor/lgen2objectpropertieseditor.hpp \
    editor/lgen2diagrameditor.hpp \
    editor/arrow.hpp \
    editor/diagramitem.hpp \
    editor/diagramscene.hpp \
    core/frame_model/nframenetmodel.h \
    core/nfaset.h \
    core/nproc.h \
    core/domains/domainnode.h \
    core/domains/domainmodel.h \
    core/domains/domain.h \
    core/validator.h \
    ui/mylistview.h \
    ui/domainwnd.h \
    ui/saver.h \
    core/production/nproduction.h \
    core/production/rulenode.h \
    core/production/rulemodel.h \
    core/production/rule.h \
    core/production/expr.h \
    ui/ruleswnd.h \
    ui/mycombobox.h \
    ui/labeledtextbox.h \
    ui/expreditor.h \
    ui/frameeditorwnd.h \
    ui/sloteditorwnd.h \
    core/frame_model/nframenode.h \
    ui/kbeditorwindow.hpp \
    visualize/view.h \
    visualize/visualizer.h \
    visualize/gamescene.h \
    visualize/grid.h \
    visualize/cell.h \
    visualize/gameitemfactory.h \
    visualize/gameitem.h \
    core/production/nproductionmlv.h \
    mlv/mlv.h

FORMS    += mainwindow.ui \
    ui/frameeditorwnd.ui \
    ui/sloteditorwnd.ui \
    ui/kbeditorwindow.ui

RESOURCES += \
    res.qrc


RC_FILE = resIcon.rc










































































