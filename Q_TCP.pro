#-------------------------------------------------
#
# Project created by QtCreator 2019-03-08T10:26:46
#
#-------------------------------------------------

QT       += core gui xml sql
QT       +=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Q_TCP
TEMPLATE = app
DESTDIR = ../Bin


SOURCES += main.cpp\
        widget.cpp \
    selection.cpp \
    ConfigureXml.cpp

HEADERS  += widget.h \
    base.h \
    selection.h \
    ConfigureXml.h

FORMS    += widget.ui \
    selection.ui
CONFIG += c++11
