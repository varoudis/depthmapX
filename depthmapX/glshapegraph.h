#pragma once

#include "salalib/mgraph.h"
#include "depthmapX/glshapemap.h"
#include "depthmapX/gllinesuniform.h"

class GLShapeGraph
{
public:
    void initializeGL(bool m_core)
    {
        m_shapeMap.initializeGL(m_core);
        m_linkLines.initializeGL(m_core);
        m_linkFills.initializeGL(m_core);
        m_unlinkFills.initializeGL(m_core);
        m_unlinkLines.initializeGL(m_core);
    }
    void updateGL(bool m_core)
    {
        m_shapeMap.updateGL(m_core);
        m_linkLines.updateGL(m_core);
        m_linkFills.updateGL(m_core);
        m_unlinkFills.updateGL(m_core);
        m_unlinkLines.updateGL(m_core);
    }
    void cleanup()
    {
        m_shapeMap.cleanup();
        m_linkLines.cleanup();
        m_linkFills.cleanup();
        m_unlinkFills.cleanup();
        m_unlinkLines.cleanup();
    }
    void paintGLOverlay(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel)
    {
        if(m_showLinks)
        {
            glLineWidth(4);
            m_linkFills.paintGL(m_mProj, m_mView, m_mModel);
            m_linkLines.paintGL(m_mProj, m_mView, m_mModel);
            m_unlinkFills.paintGL(m_mProj, m_mView, m_mModel);
            m_unlinkLines.paintGL(m_mProj, m_mView, m_mModel);
            glLineWidth(1);
        }
    }
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel)
    {
        m_shapeMap.paintGL(m_mProj, m_mView, m_mModel);
    }
    void showLinks(bool showLinks) {
        m_showLinks = showLinks;
    }
    void loadGLObjects(ShapeGraph &shapeGraph);
private:
    GLShapeMap m_shapeMap;
    GLLinesUniform m_linkLines;
    GLTrianglesUniform m_linkFills;
    GLLinesUniform m_unlinkLines;
    GLTrianglesUniform m_unlinkFills;

    bool m_showLinks = false;
};
