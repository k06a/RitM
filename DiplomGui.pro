#-------------------------------------------------
#
# Project created by QtCreator 2011-04-04T20:17:56
#
#-------------------------------------------------

QT       += core gui svg

TARGET = DiplomGui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    ProcListWidget.cpp \
    ProcTableWidget.cpp \
    ProcMimeData.cpp \
    ProcRubberBand.cpp \
    ProcTableWidgetItem.cpp \
    ModuleHolder.cpp \
    ProcCommands.cpp

HEADERS  += mainwindow.h \
    ProcListWidget.h \
    ProcMimeData.h \
    ProcRubberBand.h \
    ProcTableWidget.h \
    ProcTableWidgetItem.h \
    ModuleHolder.h \
    ProcCommands.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    res.qrc
