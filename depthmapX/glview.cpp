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


#include "depthmapX/glview.h"
#include <QMouseEvent>
#include <QCoreApplication>
#include <math.h>

GLView::GLView(QWidget *parent, QGraphDoc* doc, const QRgb &backgroundColour, const QRgb &foregroundColour)
    : QOpenGLWidget(parent),
      m_xPos(0),
      m_yPos(0),
      m_zPos(0),
      m_background(backgroundColour),
      m_foreground(foregroundColour)
{
    std::cout << "starting" << std::endl;
    m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
    pDoc = doc;


    pDoc->m_meta_graph->setLock(this);
    m_lineData.loadLineData(pDoc->m_meta_graph->getVisibleLines(), m_foreground);
    pDoc->m_meta_graph->releaseLock(this);

}

GLView::~GLView()
{
    cleanup();
}

QSize GLView::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLView::sizeHint() const
{
    return QSize(400, 400);
}

void GLView::setXPosition(float xPos)
{
    if (xPos != m_xPos) {
        m_xPos = xPos;
        emit xPositionChanged(xPos);
        update();
    }
}

void GLView::setYPosition(float yPos)
{
    if (yPos != m_yPos) {
        m_yPos = yPos;
        emit yPositionChanged(yPos);
        update();
    }
}

void GLView::setZPosition(float zPos)
{
    if (zPos != m_zPos) {
        m_zPos = zPos;
        emit zPositionChanged(zPos);
        update();
    }
}

void GLView::cleanup()
{
    makeCurrent();
    m_lineData.cleanup();
    doneCurrent();
}

void GLView::initializeGL()
{
    connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, &GLView::cleanup);

    initializeOpenGLFunctions();
    glClearColor(qRed(m_background)/255.0f, qGreen(m_background)/255.0f, qBlue(m_background)/255.0f, 1);

    m_lineData.initializeGL(m_core);

    m_mView.setToIdentity();
    m_mView.translate(0, 0, -1);
}

void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    m_mModel.setToIdentity();
    m_mModel.translate(m_xPos, m_yPos, m_zPos);

    m_lineData.paintGL(m_mProj, m_mView, m_mModel);
}

void GLView::resizeGL(int w, int h)
{
    m_mProj.setToIdentity();
    GLfloat ratio = GLfloat(w) / h;
    if(perspectiveView)
    {
        m_mProj.perspective(45.0f, ratio, 0.01f, 100.0f);
    }
    else
    {
        m_mProj.ortho(-0.5f * ratio, 0.5f * ratio, -0.5f, 0.5f, 0, 10);
    }
}

void GLView::mousePressEvent(QMouseEvent *event)
{
    m_lastPos = event->pos();
}

void GLView::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_lastPos.x();
    int dy = event->y() - m_lastPos.y();

    if (event->buttons() & Qt::RightButton) {
        setXPosition(m_xPos + 0.002f * dx);
        setYPosition(m_yPos - 0.002f * dy);
    }
    m_lastPos = event->pos();
}
