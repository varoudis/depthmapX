TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    genlib \
    salalib \
    configdialog \
    depthmapX \
    GuiUnitTest \
    GuiApp \
    depthmapXcli \
    cliTest \
    salaTest \
    genlibTest
GuiApp.depends = depthmapX genlib salalib configdialog
