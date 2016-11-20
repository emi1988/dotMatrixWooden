#-------------------------------------------------
#
# Project created by QtCreator 2016-03-21T22:21:27
#
#-------------------------------------------------

QT       += core gui

#for qt5 or greater use :
#QT += serialport

CONFIG += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = dotMatrix
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    myDefs.h

FORMS    += mainwindow.ui

LIBS += -L/usr/local/lib -lwiringPi

#unix:!macx: LIBS += -L$$PWD/../../../../../../usr/local/lib/ -lwiringPi

INCLUDEPATH += $$PWD/../../../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../../../usr/local/include
