#-------------------------------------------------
#
# Project created by QtCreator 2016-02-06T22:15:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4):

QMAKE_LFLAGS += /MANIFESTUAC:"level='requireAdministrator' "

TARGET = AutoSwitchHelper
TEMPLATE = app

SOURCES += main.cpp

win32:RC_FILE = resources.rc

HEADERS += \
    version.h
