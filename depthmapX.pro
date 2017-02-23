TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    genlib \
    salalib \
    depthmapX \
    GuiUnitTest \
    GuiApp \
    depthmapXcli \
    cliTest
GuiApp.depends = depthmapX genlib salalib
