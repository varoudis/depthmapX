include(../defaults.pri)
TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
INCLUDEPATH += ../ThirdParty/Catch

SOURCES += main.cpp \
    ../depthmapXcli/commandlineparser.cpp \
    testcommandlineparser.cpp \
    testradiusconverter.cpp \
    ../depthmapXcli/radiusconverter.cpp

HEADERS += \
    ../depthmapXcli/commandlineparser.h \
    ../depthmapXcli/radiusconverter.h
