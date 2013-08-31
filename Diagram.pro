#-------------------------------------------------
#
# Project created by QtCreator 2013-08-31T10:55:27
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Diagram
TEMPLATE = app


SOURCES += main.cpp\
        diagramwindow.cpp \
    node.cpp \
    link.cpp \
    propertiesdialog.cpp

HEADERS  += diagramwindow.h \
    node.h \
    link.h \
    propertiesdialog.h

RESOURCES += \
    images.qrc
