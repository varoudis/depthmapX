include(../defaults.pri)
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    commandlineparser.cpp \
    runmethods.cpp \
    radiusconverter.cpp

HEADERS += \
    commandlineparser.h \
    runmethods.h \
    radiusconverter.h

win32:Release:LIBS += -L../genlib/release -L../salalib/release
win32:Debug:LIBS += -L../genlib/debug -L../salalib/debug
!win32:LIBS += -L../genlib -L../salalib

LIBS += -lsalalib -lgenlib
