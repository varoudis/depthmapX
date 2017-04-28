#-------------------------------------------------
#
# Project created by QtCreator 2017-02-09T16:19:46
#
#-------------------------------------------------
include(../defaults.pri)


QT       -= qt
CONFIG   -= qt
CONFIG   -= app_bundle
DEFINES       += _DEPTHMAP
TARGET = salalib
TEMPLATE = lib
CONFIG        += staticlib c++11

DEFINES += SALALIB_LIBRARY

SOURCES += \
    attributes.cpp \
    axialmap.cpp \
    connector.cpp \
    datalayer.cpp \
    idepthmap.cpp \
    idepthmapx.cpp \
    isovist.cpp \
    MapInfoData.cpp \
    mgraph.cpp \
    nagent.cpp \
    ngraph.cpp \
    ntfp.cpp \
    pointdata.cpp \
    salaprogram.cpp \
    shapemap.cpp \
    spacepix.cpp \
    sparksieve2.cpp \
    tigerp.cpp \
    topomet.cpp \
    vertex.cpp \
    entityparsing.cpp

HEADERS += \
    attributes.h \
    axialmap.h \
    connector.h \
    datalayer.h \
    fileproperties.h \
    idepthmapx.h \
    isovist.h \
    MapInfoData.h \
    mgraph.h \
    nagent.h \
    ngraph.h \
    ntfp.h \
    pointdata.h \
    salaprogram.h \
    shapemap.h \
    spacepix.h \
    sparksieve2.h \
    tigerp.h \
    topomet.h \
    vertex.h \
    idepthmap.h \
    entityparsing.h

DISTFILES += \
    salascript-tests.txt
