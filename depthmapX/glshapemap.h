#pragma once

#include "salalib/mgraph.h"
#include "depthmapX/gllines.h"
#include "depthmapX/glpolygons.h"

class GLShapeMap
{
public:
    void initializeGL(bool m_core)
    {
        m_lines.initializeGL(m_core);
        m_polygons.initializeGL(m_core);
    }
    void updateGL(bool m_core)
    {
        m_lines.updateGL(m_core);
        m_polygons.updateGL(m_core);
    }
    void cleanup()
    {
        m_lines.cleanup();
        m_polygons.cleanup();
    }
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel)
    {
        m_lines.paintGL(m_mProj, m_mView, m_mModel);
        m_polygons.paintGL(m_mProj, m_mView, m_mModel);
    }
    void loadGLObjects(ShapeMap &shapeMap);
private:
    GLLines m_lines;
    GLPolygons m_polygons;
};
