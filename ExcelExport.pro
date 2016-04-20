#-------------------------------------------------
#
# Project created by QtCreator 2015-08-28T15:06:30
#
#-------------------------------------------------

QT       += core gui xlsx xml axcontainer sql
CONFIG += c++11 thread
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ExcelExport
TEMPLATE = app


SOURCES += main.cpp\
        excelexport.cpp \
    global.cpp \
    fontchina.cpp \
    detailxmlwidget.cpp \
    detailexcelwidget.cpp \
    sourcelistwidget.cpp \
    fielddialog.cpp \
    qmessageinputdialog.cpp \
    baseselectlist.cpp \
    batchgeneratedialog.cpp \
    qsqlmanager.cpp \
    filterdialog.cpp \
    batchgeneratethread.cpp \
    batchfilterthread.cpp

HEADERS  += excelexport.h \
    global.h \
    fontchina.h \
    detailxmlwidget.h \
    detailexcelwidget.h \
    sourcelistwidget.h \
    fielddialog.h \
    qmessageinputdialog.h \
    baseselectlist.h \
    batchgeneratedialog.h \
    qsqlmanager.h \
    filterdialog.h \
    batchgeneratethread.h \
    batchfilterthread.h

FORMS    += excelexport.ui \
    fielddialog.ui \
    qmessageinputdialog.ui \
    batchgeneratedialog.ui \
    filterdialog.ui

