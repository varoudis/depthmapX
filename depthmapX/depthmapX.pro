include(../defaults.pri)
include(dialogs/dialogs.pri)

QT            += core gui opengl widgets
DEFINES       += _DEPTHMAP
TEMPLATE      = lib
CONFIG        += staticlib
TARGET        = depthmapX
HEADERS       += depthmapView.h \
                GraphDoc.h \
                indexWidget.h \
                mainwindow.h \
                mdichild.h \
                treeWindow.h \
                3DView.h \
                PlotView.h \
                tableView.h \
    compatibilitydefines.h \
    mainwindowfactory.h \
    viewhelpers.h \
    version.h \
    settings.h \
    settingsimpl.h \
    gllinesuniform.h \
    glview.h \
    gllines.h \
    glrastertexture.h \
    glpolygons.h \
    glutriangulator.h \
    gltrianglesuniform.h \
    glpointmap.h \
    glshapegraph.h \
    glshapemap.h \
    gldynamicrect.h \
    gldynamicline.h \
    mapview.h

SOURCES       += depthmapView.cpp \
                GraphDoc.cpp \
                indexWidget.cpp \
                mainwindow.cpp \
                mdichild.cpp \
                renderthread.cpp \
                treeWindow.cpp \
                3DView.cpp \
                PlotView.cpp \
                tableView.cpp \
    mainwindowfactory.cpp \
    viewhelpers.cpp \
    settingsimpl.cpp \
    gllinesuniform.cpp \
    glview.cpp \
    gllines.cpp \
    glrastertexture.cpp \
    glpolygons.cpp \
    glutriangulator.cpp \
    gltrianglesuniform.cpp \
    glpointmap.cpp \
    glshapegraph.cpp \
    glshapemap.cpp \
    gldynamicrect.cpp \
    gldynamicline.cpp \
    mapview.cpp

RESOURCES     += resource.qrc

OTHER_FILES += \
    Libs/include/generic/lgpl.txt

QMAKE_CXXFLAGS_WARN_ON =

FORMS += \
    UI/TopoMetDlg.ui \
    UI/SegmentAnalysisDlg.ui \
    UI/RenameObjectDlg.ui \
    UI/PushDialog.ui \
    UI/PromptReplace.ui \
    UI/OptionsDlg.ui \
    UI/NewLayerDlg.ui \
    UI/MakeOptionsDlg.ui \
    UI/MakeLayerDlg.ui \
    UI/LicenceDialog.ui \
    UI/LayerChooserDlg.ui \
    UI/IsovistPathDlg.ui \
    UI/InsertColumnDlg.ui \
    UI/GridDialog.ui \
    UI/FindLocDlg.ui \
    UI/FilePropertiesDlg.ui \
    UI/FewestLineOptionsDlg.ui \
    UI/EditConnectionsDlg.ui \
    UI/DepthmapOptionsDlg.ui \
    UI/DepthmapAlert.ui \
    UI/ConvertShapesDlg.ui \
    UI/ColumnPropertiesDlg.ui \
    UI/ColourScaleDlg.ui \
    UI/AxialAnalysisOptionsDlg.ui \
    UI/AttributeSummary.ui \
    UI/AttributeChooserDlg.ui \
    UI/AgentAnalysisDlg.ui \
    UI/AboutDlg.ui \
    UI/licenseagreement.ui

QMAKE_CXXFLAGS += -DAPP_DATE=\\\"`date +'\"%a_%b_%d,_%Y\"'`\\\"
QMAKE_CXXFLAGS += -DAPP_GIT_BRANCH=\\\"`git rev-parse --abbrev-ref HEAD`\\\"
QMAKE_CXXFLAGS += -DAPP_GIT_COMMIT=\\\"`git log --pretty=format:'%h' -n 1`\\\"
