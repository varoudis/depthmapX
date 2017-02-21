TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    genlib \
    salalib \
    depthmapX \
    GuiUnitTest \
    GuiApp
GuiApp.depends = depthmapX genlib salalib
