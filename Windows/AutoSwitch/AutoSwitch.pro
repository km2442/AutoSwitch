#-------------------------------------------------
#
# Project created by QtCreator 2015-07-05T19:11:04
#
#-------------------------------------------------

QT       += core gui sql network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = AutoSwitch
TEMPLATE = app

include(qtsingleapplication\src\qtsingleapplication.pri)

SOURCES += \
    Source/Classes/communicatorthread.cpp \
    Source/Classes/database.cpp \
    Source/Classes/databasepass.cpp \
    Source/Classes/jsonactionparser.cpp \
    Source/Classes/mybutton.cpp \
    Source/Classes/socketcommunicator.cpp \
    Source/Dialogs/firstrun.cpp \
    Source/Dialogs/help.cpp \
    Source/Dialogs/hidedialog.cpp \
    Source/Dialogs/infoswitch.cpp \
    Source/Dialogs/opinion.cpp \
    Source/Dialogs/passwordconfig.cpp \
    Source/Dialogs/passwordget.cpp \
    Source/Dialogs/restxtview.cpp \
    Source/Main/autorun.cpp \
    Source/Main/functions.cpp \
    Source/Main/main.cpp \
    Source/Main/mainwindow.cpp \
    Source/Main/settings.cpp \
    Source/Classes/databasepass.cpp

HEADERS  += \
    Source/Classes/communicatorthread.h \
    Source/Classes/database.h \
    Source/Classes/jsonactionparser.h \
    Source/Classes/mybutton.h \
    Source/Classes/socketcommunicator.h \
    Source/Dialogs/firstrun.h \
    Source/Dialogs/help.h \
    Source/Dialogs/hidedialog.h \
    Source/Dialogs/infoswitch.h \
    Source/Dialogs/opinion.h \
    Source/Dialogs/passwordconfig.h \
    Source/Dialogs/passwordget.h \
    Source/Dialogs/restxtview.h \
    Source/Main/autorun.h \
    Source/Main/functions.h \
    Source/Main/mainwindow.h \
    Source/Main/settings.h \
    Source/Main/version.h \
    Source/Main/structures.h

FORMS    += \
    Source/Dialogs/firstrun.ui \
    Source/Dialogs/help.ui \
    Source/Dialogs/hidedialog.ui \
    Source/Dialogs/infoswitch.ui \
    Source/Dialogs/opinion.ui \
    Source/Dialogs/passwordconfig.ui \
    Source/Dialogs/passwordget.ui \
    Source/Dialogs/restxtview.ui \
    Source/Main/autorun.ui \
    Source/Main/mainwindow.ui \
    Source/Main/settings.ui

RESOURCES += \ 
    conf.qrc \
    resources.qrc

win32:RC_FILE = resources.rc
