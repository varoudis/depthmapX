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

#include "gltriangles.h"
#include <qmath.h>

// clang-format off
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
        "in vec4 colour;\n"
        "out highp vec4 fragColor;\n"
        "void main() {\n"
        "   fragColor = colour;\n"
        "}\n";

static const char *vertexShaderSource =
        "attribute vec4 vertex;\n"
        "attribute vec4 colour;\n"
        "varying vec4 fragColour;\n"
        "uniform mat4 projMatrix;\n"
        "uniform mat4 mvMatrix;\n"
        "void main() {\n"
        "   gl_Position = projMatrix * mvMatrix * vertex;\n"
        "   fragColour = colour;\n"
        "}\n";

static const char *fragmentShaderSource =
        "varying highp vec4 fragColour;\n"
        "void main() {\n"
        "   gl_FragColor = fragColour;\n"
        "}\n";
// clang-format on

void GLTriangles::loadTriangleData(const std::vector<std::pair<std::vector<Point2f>, QRgb>> &triangleData) {

    init(triangleData.size());

    for (auto &triangle : triangleData) {
        float r = qRed(triangle.second) / 255.0;
        float g = qGreen(triangle.second) / 255.0;
        float b = qBlue(triangle.second) / 255.0;
        for (auto &point : triangle.first) {
            add(QVector3D(point.x, point.y, 0.0f), QVector3D(r, g, b));
        }
    }
}

void GLTriangles::setupVertexAttribs() {
    m_vbo.bind();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glEnableVertexAttribArray(0);
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, DATA_DIMENSIONS * sizeof(GLfloat), 0);
    f->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, DATA_DIMENSIONS * sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
    m_vbo.release();
}

void GLTriangles::initializeGL(bool m_core) {
    if (m_data.size() == 0)
        return;
    m_program = new QOpenGLShaderProgram;
    m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, m_core ? vertexShaderSourceCore : vertexShaderSource);
    m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
                                       m_core ? fragmentShaderSourceCore : fragmentShaderSource);
    m_program->bindAttributeLocation("vertex", 0);
    m_program->bindAttributeLocation("colour", 1);
    m_program->link();

    m_program->bind();
    m_projMatrixLoc = m_program->uniformLocation("projMatrix");
    m_mvMatrixLoc = m_program->uniformLocation("mvMatrix");

    m_vao.create();
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

    m_vbo.create();
    m_vbo.bind();
    m_vbo.allocate(constData(), m_count * sizeof(GLfloat));

    setupVertexAttribs();
    m_program->release();
    m_built = true;
}

void GLTriangles::updateGL(bool m_core) {
    if (m_program == 0) {
        // has not been initialised yet, do that instead
        initializeGL(m_core);
    } else {
        m_vbo.bind();
        m_vbo.allocate(constData(), m_count * sizeof(GLfloat));
        m_vbo.release();
        m_built = true;
    }
}

void GLTriangles::cleanup() {
    if (!m_built)
        return;
    m_vbo.destroy();
    delete m_program;
    m_program = 0;
}

void GLTriangles::paintGL(const QMatrix4x4 &m_mProj, const QMatrix4x4 &m_mView, const QMatrix4x4 &m_mModel) {
    if (!m_built)
        return;
    QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
    m_program->bind();
    m_program->setUniformValue(m_projMatrixLoc, m_mProj);
    m_program->setUniformValue(m_mvMatrixLoc, m_mView * m_mModel);

    QOpenGLFunctions *glFuncs = QOpenGLContext::currentContext()->functions();
    glFuncs->glDrawArrays(GL_TRIANGLES, 0, vertexCount());

    m_program->release();
}

void GLTriangles::add(const QVector3D &v, const QVector3D &c) {
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = c.x();
    *p++ = c.y();
    *p++ = c.z();
    m_count += DATA_DIMENSIONS;
}
