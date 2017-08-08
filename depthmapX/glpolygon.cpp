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

#include "glpolygon.h"
#include "glu.h"
#include "glutriangulator.h"
#include <qmath.h>

static const char *vertexShaderSourceCore =
    "#version 150\n"
    "in vec4 vertex;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "void main() {\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSourceCore =
    "#version 150\n"
    "uniform vec4 colourVector;\n"
    "out highp vec4 fragColor;\n"
    "void main() {\n"
    "   fragColor = colourVector;\n"
    "}\n";

static const char *vertexShaderSource =
    "attribute vec4 vertex;\n"
    "uniform mat4 projMatrix;\n"
    "uniform mat4 mvMatrix;\n"
    "void main() {\n"
    "   gl_Position = projMatrix * mvMatrix * vertex;\n"
    "}\n";

static const char *fragmentShaderSource =
    "uniform vec4 colourVector;\n"
    "void main() {\n"
    "   gl_FragColor = colourVector;\n"
    "}\n";

/**
 * @brief GLPolygon::GLPolygon
 * This class is an OpenGL representation of a single polygon of uniform colour
 */

GLPolygon::GLPolygon()
    : m_count(0),
      m_program(0)
{

}

void GLPolygon::loadPolygonData(const std::vector<Point2f>& points, const PafColor &polyColour)
{
    built = false;

    m_count = 0;

    vector<Point2f> triangulated = GLUTriangulator::triangulate(points);

    m_data.resize(triangulated.size() * DATA_DIMENSIONS);

    std::vector<Point2f>::const_iterator iter = triangulated.begin(), end =
    triangulated.end();
    for ( ; iter != end; ++iter )
    {
        const Point2f & point = *iter;
        add(QVector3D(point.x, point.y, 0.0f));
    }
    m_colour.setX(polyColour.redf());
    m_colour.setY(polyColour.greenf());
    m_colour.setZ(polyColour.bluef());
}

void GLPolygon::setupVertexAttribs()
{
    m_vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, DATA_DIMENSIONS * sizeof(GLfloat), 0);
    m_vbo.release();
}

void GLPolygon::initializeGL(bool m_core)
{
    if(m_data.size() == 0) return;
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_core ? vertexShaderSourceCore : vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, m_core ? fragmentShaderSourceCore : fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");
    m_colourVectorLoc = m_program->uniformLocation("colourVector");

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(constData(), m_count * sizeof(GLfloat));

    setupVertexAttribs();
    m_program->setUniformValue(m_colourVectorLoc, m_colour);
    m_program->release();
    built = true;
}

void GLPolygon::updateGL(bool m_core) {
    if(m_program == 0) {
        // has not been initialised yet, do that instead
        initializeGL(m_core);
    } else {
        m_vbo.bind();
        m_vbo.allocate(constData(), m_count * sizeof(GLfloat));
        m_vbo.release();
        built = true;
    }
}

void GLPolygon::updateColour(const PafColor &polyColour)
{
    m_colour.setX(polyColour.redf());
    m_colour.setY(polyColour.greenf());
    m_colour.setZ(polyColour.bluef());
    m_program->bind();
    m_program->setUniformValue(m_colourVectorLoc, m_colour);
    m_program->release();
}

void GLPolygon::cleanup()
{
    if(!built) return;
    m_vbo.destroy();
    delete m_program;
    m_program = 0;
}

void GLPolygon::paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel)
{
    if(!built) return;
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_mProj);
    m_program->setUniformValue(m_mvMatrixLoc, m_mView * m_mModel);

    glDrawArrays(GL_TRIANGLES, 0, vertexCount());

    m_program->release();
}

void GLPolygon::add(const QVector3D &v)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    m_count += DATA_DIMENSIONS;
}
