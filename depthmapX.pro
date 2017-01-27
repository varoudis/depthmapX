TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    depthmapX \
    GuiApp \ 
    GuiUnitTest
GuiApp.depends = depthmapX
