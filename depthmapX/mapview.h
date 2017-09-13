#pragma once

#include <QOpenGLWidget>
#include "GraphDoc.h"
#include "settings.h"

class MapView : public QOpenGLWidget
{
protected:
    QGraphDoc &m_pDoc;
    Settings &m_settings;

public:
    MapView(QGraphDoc &pDoc,
            Settings &settings,
            QWidget* parent = Q_NULLPTR);
    virtual void OnModeJoin() = 0;
    virtual void OnModeUnjoin() = 0;
    virtual void OnViewPan() = 0;
    virtual void OnViewZoomIn() = 0;
    virtual void OnViewZoomOut() = 0;
    virtual void OnEditFill() = 0;
    virtual void OnEditSemiFill() = 0;
    virtual void OnEditAugmentFill() = 0;
    virtual void OnEditPencil() = 0;
    virtual void OnModeIsovist() = 0;
    virtual void OnModeTargetedIsovist() = 0;
    virtual void OnEditLineTool() = 0;
    virtual void OnEditPolygonTool() = 0;
    virtual void OnModeSeedAxial() = 0;
    virtual void OnEditSelect() = 0;
    QGraphDoc *getGraphDoc() { return &m_pDoc; }
};
