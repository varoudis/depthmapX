#pragma once

#include "salalib/mgraph.h"
#include "depthmapX/gllinesuniform.h"
#include "depthmapX/glrastertexture.h"
#include "depthmapX/gltrianglesuniform.h"

class GLPointMap
{
public:
    void initializeGL(bool m_core)
    {
        m_grid.initializeGL(m_core);
        m_pointMap.initializeGL(m_core);
        m_linkLines.initializeGL(m_core);
        m_linkFills.initializeGL(m_core);
    }
    void updateGL(bool m_core)
    {
        m_pointMap.updateGL(m_core);
        m_grid.updateGL(m_core);
        m_linkLines.updateGL(m_core);
        m_linkFills.updateGL(m_core);
    }
    void cleanup()
    {
        m_grid.cleanup();
        m_pointMap.cleanup();
        m_linkLines.cleanup();
        m_linkFills.cleanup();
    }
    void paintGLOverlay(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel)
    {
        glLineWidth(4);
        m_linkFills.paintGL(m_mProj, m_mView, m_mModel);
        m_linkLines.paintGL(m_mProj, m_mView, m_mModel);
        glLineWidth(1);
    }
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel)
    {
        m_grid.paintGL(m_mProj, m_mView, m_mModel);
        m_pointMap.paintGL(m_mProj, m_mView, m_mModel);
    }
    void setGridColour(QRgb gridColour) {
        m_gridColour = gridColour;
    }
    void showLinks(bool showLinks) {
        m_showLinks = showLinks;
    }
    void loadGLObjects(PointMap& pointMap);
    void loadGLObjectsRequiringGLContext(const PointMap& currentPointMap);
private:
    GLLinesUniform m_grid;
    GLRasterTexture m_pointMap;
    GLLinesUniform m_linkLines;
    GLTrianglesUniform m_linkFills;

    QRgb m_gridColour = (qRgb(255, 255, 255) & 0x006f6f6f) | (qRgb(0, 0, 0) & 0x00a0a0a0);

    bool m_showGrid = true;
    bool m_showLinks = false;
};
