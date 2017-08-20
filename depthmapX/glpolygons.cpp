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

#include "glpolygons.h"
#include "glutriangulator.h"

/**
 * @brief GLPolygons::GLPolygons
 * This class is an OpenGL representation of multiple polygons of different colour
 */

void GLPolygons::loadPolygonData(const std::map<std::vector<Point2f>, PafColor>& colouredPolygons)
{
    m_polygons.clear();
    std::map<std::vector<Point2f>, PafColor>::const_iterator iter = colouredPolygons.begin(), end =
    colouredPolygons.end();
    for ( ; iter != end; ++iter )
    {
        const std::vector<Point2f> & points = iter->first;
        const PafColor & colour = iter->second;

        m_polygons.push_back(std::unique_ptr<GLTrianglesUniform>(new GLTrianglesUniform));

        vector<Point2f> triangulated = GLUTriangulator::triangulate(points);
        m_polygons[m_polygons.size() - 1]->loadTriangleData(triangulated, colour);
    }
}

void GLPolygons::initializeGL(bool m_core)
{
    std::vector<std::unique_ptr<GLTrianglesUniform>>::iterator iter = m_polygons.begin(), end =
    m_polygons.end();
    for ( ; iter != end; ++iter )
    {
        (*iter)->initializeGL(m_core);
    }
}

void GLPolygons::updateGL(bool m_core)
{
    std::vector<std::unique_ptr<GLTrianglesUniform>>::iterator iter = m_polygons.begin(), end =
    m_polygons.end();
    for ( ; iter != end; ++iter )
    {
        (*iter)->updateGL(m_core);
    }
}

void GLPolygons::cleanup()
{
    std::vector<std::unique_ptr<GLTrianglesUniform>>::iterator iter = m_polygons.begin(), end =
    m_polygons.end();
    for ( ; iter != end; ++iter )
    {
        (*iter)->cleanup();
    }
}

void GLPolygons::paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel)
{
    std::vector<std::unique_ptr<GLTrianglesUniform>>::iterator iter = m_polygons.begin(), end =
    m_polygons.end();
    for ( ; iter != end; ++iter )
    {
        (*iter)->paintGL(m_mProj, m_mView, m_mModel);
    }
}
