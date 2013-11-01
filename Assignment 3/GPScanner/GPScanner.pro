#-------------------------------------------------
#
# Project created by QtCreator 2013-10-31T21:37:29
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GPScanner
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    gps-utils.cpp \
    gpsrint.cpp

HEADERS  += mainwindow.h \
    top.h

FORMS    += mainwindow.ui
