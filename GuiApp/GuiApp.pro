include(../defaults.pri)

QT         += core gui opengl widgets
DEFINES    += _DEPTHMAP
TEMPLATE   = app
TARGET     = depthmapX
ICON       = icons/depthmapX.icns
SOURCES    = main.cpp

win32:RC_ICONS += icons/depthmapX.ico

win32:Release:LIBS += -L../depthmapX/release -L../genlib/release -L../salalib/release -L../configdialog/release
win32:Debug:LIBS += -L../depthmapX/debug -L../genlib/debug -L../salalib/debug -L../configdialog/debug
!win32:LIBS += -L../depthmapX -L../genlib -L../salalib -L../configdialog

LIBS += -ldepthmapX -lsalalib -lgenlib -lconfigdialog

!win32:!macx:LIBS += -L/usr/lib/i386-linux-gnu/

!win32:!macx:LIBS += -lGL -lGLU


win32:LIBS += -lOpenGl32 -lglu32 -lgdi32

HEADERS += \
    coreapplication.h


