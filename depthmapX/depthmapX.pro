include(../defaults.pri)
QT            += core gui opengl widgets
DEFINES       += _DEPTHMAP
TEMPLATE      = lib
CONFIG        += staticlib
TARGET        = depthmapX
HEADERS       = depthmapView.h \
                GraphDoc.h \
                indexWidget.h \
                mainwindow.h \
                mdichild.h \
                treeWindow.h \
                3DView.h \
                PlotView.h \
                tableView.h \
    TopoMetDlg.h \
    SegmentAnalysisDlg.h \
    RenameObjectDlg.h \
    PushDialog.h \
    PromptReplace.h \
    OptionsDlg.h \
    NewLayerDlg.h \
    MakeOptionsDlg.h \
    MakeLayerDlg.h \
    LicenceDialog.h \
    LayerChooserDlg.h \
    IsovistPathDlg.h \
    InsertColumnDlg.h \
    GridDialog.h \
    FindLocDlg.h \
    FilePropertiesDlg.h \
    FewestLineOptionsDlg.h \
    EditConnectionsDlg.h \
    DepthmapOptionsDlg.h \
    ConvertShapesDlg.h \
    ColumnPropertiesDlg.h \
    ColourScaleDlg.h \
    AxialAnalysisOptionsDlg.h \
    AttributeSummary.h \
    AttributeChooserDlg.h \
    AgentAnalysisDlg.h \
    AboutDlg.h \
    Libs/include/generic/xmlparse.h \
    Libs/include/generic/paftl.h \
    Libs/include/generic/paftl_old.h \
    Libs/include/generic/pafmath.h \
    Libs/include/generic/p2dpoly.h \
    Libs/include/generic/dxfp.h \
    Libs/include/generic/comm.h \
    Libs/include/sala/vertex.h \
    Libs/include/sala/spacepix.h \
    Libs/include/sala/shapemap.h \
    Libs/include/sala/salaprogram.h \
    Libs/include/sala/pointdata.h \
    Libs/include/sala/ngraph.h \
    Libs/include/sala/nagent.h \
    Libs/include/sala/mgraph.h \
    Libs/include/sala/idepthmapx.h \
    Libs/include/sala/fileproperties.h \
    Libs/include/sala/datalayer.h \
    Libs/include/sala/connector.h \
    Libs/include/sala/axialmap.h \
    Libs/include/sala/attributes.h \
    licenseagreement.h \
    compatibilitydefines.h

SOURCES       = depthmapView.cpp \
                GraphDoc.cpp \
                indexWidget.cpp \
                mainwindow.cpp \
                mdichild.cpp \
                renderthread.cpp \
                treeWindow.cpp \
                3DView.cpp \
                PlotView.cpp \
                tableView.cpp \
    TopoMetDlg.cpp \
    SegmentAnalysisDlg.cpp \
    RenameObjectDlg.cpp \
    PushDialog.cpp \
    PromptReplace.cpp \
    OptionsDlg.cpp \
    NewLayerDlg.cpp \
    MakeOptionsDlg.cpp \
    MakeLayerDlg.cpp \
    LicenceDialog.cpp \
    LayerChooserDlg.cpp \
    IsovistPathDlg.cpp \
    InsertColumnDlg.cpp \
    GridDialog.cpp \
    FindLocDlg.cpp \
    FilePropertiesDlg.cpp \
    FewestLineOptionsDlg.cpp \
    EditConnectionsDlg.cpp \
    DepthmapOptionsDlg.cpp \
    ConvertShapesDlg.cpp \
    ColumnPropertiesDlg.cpp \
    ColourScaleDlg.cpp \
    AxialAnalysisOptionsDlg.cpp \
    AttributeSummary.cpp \
    AttributeChooserDlg.cpp \
    AgentAnalysisDlg.cpp \
    AboutDlg.cpp \
# genlib
    Libs/genlib/dxfp.cpp \
    Libs/genlib/p2dpoly.cpp \
    Libs/genlib/pafmath.cpp \
    Libs/include/generic/xmlparse.cpp \
# salalib
    Libs/salalib/attributes.cpp \
    Libs/salalib/axialmap.cpp \
    Libs/salalib/connector.cpp \
    Libs/salalib/datalayer.cpp \
    Libs/salalib/idepthmap.cpp \
    Libs/salalib/idepthmapx.cpp \
    Libs/salalib/isovist.cpp \
    Libs/salalib/MapInfoData.cpp \
    Libs/salalib/mgraph.cpp \
    Libs/salalib/nagent.cpp \
    Libs/salalib/ngraph.cpp \
    Libs/salalib/ntfp.cpp \
    Libs/salalib/pointdata.cpp \
    Libs/salalib/salaprogram.cpp \
    Libs/salalib/shapemap.cpp \
    Libs/salalib/spacepix.cpp \
    Libs/salalib/sparksieve2.cpp \
    Libs/salalib/tigerp.cpp \
    Libs/salalib/topomet.cpp \
    Libs/salalib/vertex.cpp \
    licenseagreement.cpp

RESOURCES     = resource.qrc

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

