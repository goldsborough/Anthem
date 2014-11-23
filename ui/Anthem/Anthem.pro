#-------------------------------------------------
#
# Project created by QtCreator 2014-09-11T17:44:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Anthem
TEMPLATE = app


SOURCES += main.cpp \
    qcustomplot/qcustomplot.cpp \
    MainWindow.cpp

HEADERS  += \
    qcustomplot/qcustomplot.h \
    MainWindow.h

FORMS    += anthem.ui
