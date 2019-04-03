TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    mgraph440 \
    mgraph440Test \
    genlib \
    salalib \
    depthmapX \
    GuiUnitTest \
    GuiApp \
    depthmapXcli \
    cliTest \
    salaTest \
    genlibTest \
    depthmapXTest
GuiApp.depends = depthmapX genlib mgraph440 salalib
