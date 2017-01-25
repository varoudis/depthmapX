TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    depthmapX \
    GuiApp 
GuiApp.depends = depthmapX
