TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    GuiUnitTest \
    genlib \
    salalib \
    depthmapX \
    GuiApp
GuiApp.depends = depthmapX genlib salalib
