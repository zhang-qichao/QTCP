#-------------------------------------------------
#
# Project created by QtCreator 2019-03-08T11:32:02
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Q_TcpClient
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    ConfigureXml.cpp

HEADERS  += widget.h \
    base.h \
    ConfigureXml.h

FORMS    += widget.ui
CONFIG += c++11
