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

#include <qopengl.h>
#include <QVector>
#include <QVector3D>
#include <QOpenGLShaderProgram>

class GLLineData
{
public:
    GLLineData();
    void paintGL(const QMatrix4x4 &m_proj, const QMatrix4x4 &m_camera, const QMatrix4x4 &m_world);
    void initializeGL(bool m_core);
    void cleanup();

private:
    const int DATA_DIMENSIONS = 3;
    void setupVertexAttribs();
    int count() const { return m_count; }
    int vertexCount() const { return m_count / DATA_DIMENSIONS; }
    const GLfloat *constData() const { return m_data.constData(); }
    void add(const QVector3D &v);

    QVector<GLfloat> m_data;
    int m_count;
    QOpenGLShaderProgram *m_program;
};
