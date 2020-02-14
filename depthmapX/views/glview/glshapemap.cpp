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

#include "glshapemap.h"

void GLShapeMap::loadGLObjects(const std::vector<std::pair<SimpleLine, PafColor>> &colouredLines,
                               const std::vector<std::pair<std::vector<Point2f>, PafColor>>& colouredPolygons,
                               const std::vector<std::pair<Point2f, PafColor>> &colouredPoints,
                               const int pointSides, const float pointRadius) {
    m_lines.loadLineData(colouredLines);
    m_polygons.loadPolygonData(colouredPolygons);
    m_points.loadPolygonData(colouredPoints, pointSides, pointRadius);
}

void GLShapeMap::loadGLObjects(ShapeMap &shapeMap) {
    loadGLObjects(shapeMap.getAllLinesWithColour(),
                  shapeMap.getAllPolygonsWithColour(),
                  shapeMap.getAllPointsWithColour(),
                  8, shapeMap.getSpacing()*0.1);
}
