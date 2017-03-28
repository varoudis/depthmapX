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
    ../depthmapXcli/radiusconverter.cpp \
    testsimpletimer.cpp \
    testvgaparser.cpp \
    ../depthmapXcli/vgaparser.cpp \
    testargumentholder.cpp \
    testperformancewriter.cpp

HEADERS += \
    ../depthmapXcli/commandlineparser.h \
    ../depthmapXcli/radiusconverter.h \
    ../depthmapXcli/simpletimer.h \
    ../depthmapXcli/vgaparser.h \
    argumentholder.h
