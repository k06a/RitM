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
    QIconTableWidget.cpp \
    QIconTableWidgetItem.cpp \
    TableItem.cpp \
    MimeData.cpp

HEADERS  += mainwindow.h \
    QIconTableWidget.h \
    QIconTableWidgetItem.h \
    TableItem.h \
    MimeData.h

FORMS    += mainwindow.ui

OTHER_FILES +=

RESOURCES += \
    res.qrc
