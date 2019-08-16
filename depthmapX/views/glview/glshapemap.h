// depthmapX - spatial network analysis platform
// Copyright (C) 2017, Petros Koutsolampros

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include "salalib/mgraph.h"
#include "depthmapX/views/glview/gllines.h"
#include "depthmapX/views/glview/glpolygons.h"
#include "depthmapX/views/glview/glregularpolygons.h"

class GLShapeMap
{
public:
    void initializeGL(bool m_core)
    {
        m_lines.initializeGL(m_core);
        m_polygons.initializeGL(m_core);
        m_points.initializeGL(m_core);
    }
    void updateGL(bool m_core)
    {
        m_lines.updateGL(m_core);
        m_polygons.updateGL(m_core);
        m_points.updateGL(m_core);
    }
    void cleanup()
    {
        m_lines.cleanup();
        m_polygons.cleanup();
        m_points.cleanup();
    }
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel)
    {
        m_lines.paintGL(m_mProj, m_mView, m_mModel);
        m_polygons.paintGL(m_mProj, m_mView, m_mModel);
        m_points.paintGL(m_mProj, m_mView, m_mModel);
    }
    void loadGLObjects(const std::vector<std::pair<SimpleLine, PafColor>> &colouredLines,
                       const std::vector<std::pair<std::vector<Point2f>, PafColor>> &colouredPolygons,
                       const std::vector<std::pair<Point2f, PafColor>> &colouredPoints,
                       const int pointSides, const float pointRadius);
    void loadGLObjects(ShapeMap &shapeMap);
private:
    GLLines m_lines;
    GLPolygons m_polygons;
    GLRegularPolygons m_points;
};
