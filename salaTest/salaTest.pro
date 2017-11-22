include(../defaults.pri)
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += ../ThirdParty/Catch

SOURCES += main.cpp \
    testentityparsing.cpp \
    testpointmap.cpp \
    testlinkutils.cpp \
    testgridproperties.cpp \
    testisovistdef.cpp \
    testmgraph.cpp \
    testshapegraphs.cpp \
    teststructsizes.cpp \
    testshapemaps.cpp \
    testgeometrygenerators.cpp \
    testsparksieve.cpp

win32:Release:LIBS += -L../genlib/release -L../salalib/release
win32:Debug:LIBS += -L../genlib/debug -L../salalib/debug
!win32:LIBS += -L../genlib -L../salalib

LIBS += -lsalalib -lgenlib
