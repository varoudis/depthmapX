include(../defaults.pri)

QT       += widgets
DEFINES += _DEPTHMAP

TARGET = configdialog
TEMPLATE = lib

CONFIG += staticlib c++11

DEFINES += CONFIGDIALOG_LIBRARY

SOURCES += configdialog.cpp \
    generalpage.cpp \
    interfacepage.cpp

HEADERS += configdialog.h \
    generalpage.h \
    interfacepage.h \
    settingspage.h

RESOURCES += \
    configdialog.qrc
