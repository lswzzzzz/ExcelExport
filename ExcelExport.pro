#-------------------------------------------------
#
# Project created by QtCreator 2015-08-28T15:06:30
#
#-------------------------------------------------

QT       += core gui xlsx xml axcontainer
CONFIG += c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExcelExport
TEMPLATE = app


SOURCES += main.cpp\
        excelexport.cpp \
    global.cpp \
    fontchina.cpp \
    detailxmlwidget.cpp \
    detailexcelwidget.cpp \
    sourcelistwidget.cpp

HEADERS  += excelexport.h \
    global.h \
    fontchina.h \
    detailxmlwidget.h \
    detailexcelwidget.h \
    sourcelistwidget.h

FORMS    += excelexport.ui
