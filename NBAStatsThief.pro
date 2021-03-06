#-------------------------------------------------
#
# Project created by QtCreator 2015-02-21T17:07:42
#
#-------------------------------------------------

QT       += core gui network webkit webkitwidgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = NBAStatsThief
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    statsloader.cpp \
    gamemodel.cpp \
    pageparser.cpp \
    dbmanager.cpp

HEADERS  += mainwindow.h \
    statsloader.h \
    gamemodel.h \
    pageparser.h \
    dbmanager.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
