#include "glshapemap.h"

void GLShapeMap::loadGLObjects(ShapeMap &shapeMap) {
    m_lines.loadLineData(shapeMap.getAllLinesWithColour());
    m_polygons.loadPolygonData(shapeMap.getAllPolygonsWithColour());
}
