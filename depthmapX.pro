TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    genlib \
    salalib \
    settingsdialog \
    depthmapX \
    GuiUnitTest \
    GuiApp \
    depthmapXcli \
    cliTest \
    salaTest \
    genlibTest
GuiApp.depends = depthmapX genlib salalib settingsdialog
