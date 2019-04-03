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

#include "genlib/p2dpoly.h"
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QRgb>
#include <QVector3D>
#include <QVector>
#include <qopengl.h>

/**
 * @brief General triangles representation. Each triangle may have its own colour
 */

class GLTriangles {
  public:
    GLTriangles() : m_count(0), m_program(0) {}
    void loadTriangleData(const std::vector<std::pair<std::vector<Point2f>, QRgb>> &triangleData);
    void paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel);
    void initializeGL(bool m_core);
    void updateGL(bool m_core);
    void cleanup();
    void updateColour(const QRgb &polyColour);
    int vertexCount() const { return m_count / DATA_DIMENSIONS; }
    GLTriangles(const GLTriangles &) = delete;
    GLTriangles &operator=(const GLTriangles &) = delete;

  protected:
    void init(int numTriangles) {
        m_built = false;
        m_count = 0;
        m_data.resize(numTriangles * 3 * DATA_DIMENSIONS);
    }
    void add(const QVector3D &v, const QVector3D &c);

  private:
    const int DATA_DIMENSIONS = 6;
    void setupVertexAttribs();
    const GLfloat *constData() const { return m_data.constData(); }

    QVector<GLfloat> m_data;
    int m_count;
    bool m_built = false;
    QVector4D m_colour = QVector4D(1.0f, 1.0f, 1.0f, 1.0f);

    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vbo;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
};
