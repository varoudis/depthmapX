include(../defaults.pri)

QT         += core gui opengl widgets
DEFINES    += _DEPTHMAP
TEMPLATE   = app
TARGET     =depthmapX

SOURCES    = main.cpp


Release:LIBS += -L../depthmapX/release
Debug:LIBS += -L../depthmapX/debug
LIBS += -ldepthmapX

!win32:!macx:LIBS += -L/usr/lib/i386-linux-gnu/

!win32:!macx:LIBS = -lGL -lGLU

win32:LIBS += -lOpenGl32 -lglu32 -lgdi32


