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
      m_eyePosX(0),
      m_eyePosY(0),
      m_background(backgroundColour),
      m_foreground(foregroundColour)
{
    m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
    pDoc = doc;


    pDoc->m_meta_graph->setLock(this);
    m_lineData.loadLineData(pDoc->m_meta_graph->getVisibleLines(), m_foreground);
    pDoc->m_meta_graph->releaseLock(this);

    std::vector<std::pair<SimpleLine, QRgb>> axesData;
    axesData.push_back(std::pair<SimpleLine, QRgb> (SimpleLine(0,0,1,0), qRgb(255,0,0)));
    axesData.push_back(std::pair<SimpleLine, QRgb> (SimpleLine(0,0,0,1), qRgb(0,255,0)));
    m_axes.loadLineData(axesData);

    PointMap& currentPointMap = pDoc->m_meta_graph->getDisplayedPointMap();
    QtRegion region = currentPointMap.getRegion();
    m_pointData.loadRegionData(region.bottom_left.x, region.bottom_left.y, region.top_right.x, region.top_right.y);

    m_eyePosX = - (region.top_right.x + region.bottom_left.x)*0.5f;
    m_eyePosY = - (region.top_right.y + region.bottom_left.y)*0.5f;
    if(region.width() > region.height())
    {
        zoomFactor = region.top_right.x - region.bottom_left.x;
    }
    else
    {
        zoomFactor = region.top_right.y - region.bottom_left.y;
    }
    minZoomFactor = zoomFactor * 0.001;
    maxZoomFactor = zoomFactor * 10;
}

GLView::~GLView()
{
    makeCurrent();
    m_axes.cleanup();
    m_lineData.cleanup();
    m_pointData.cleanup();
    doneCurrent();
}

QSize GLView::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLView::sizeHint() const
{
    return QSize(400, 400);
}

void GLView::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(qRed(m_background)/255.0f, qGreen(m_background)/255.0f, qBlue(m_background)/255.0f, 1);

    m_axes.initializeGL(m_core);
    m_lineData.initializeGL(m_core);
    m_pointData.initializeGL(m_core);

    PointMap& currentPointMap = pDoc->m_meta_graph->getDisplayedPointMap();
    QImage data(currentPointMap.getCols(),currentPointMap.getRows(), QImage::Format_RGBA8888);
    data.fill(Qt::transparent);

    AttributeTable& table = currentPointMap.getAttributeTable();
    for (int i = 0; i < table.getRowCount(); i++) {
       PixelRef pix = table.getRowKey(i);
       data.setPixelColor(pix.x, pix.y, table.getDisplayColorByKey( pix ));
    }

    m_pointData.loadPixelData(data);

    m_mModel.setToIdentity();

    m_mView.setToIdentity();
    m_mView.translate(0, 0, -1);
}

void GLView::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    m_axes.paintGL(m_mProj, m_mView, m_mModel);
    m_pointData.paintGL(m_mProj, m_mView, m_mModel);
    m_lineData.paintGL(m_mProj, m_mView, m_mModel);
}

void GLView::resizeGL(int w, int h)
{
    screenWidth = w;
    screenHeight = h;
    screenRatio = GLfloat(w) / h;
    recalcView();
}

void GLView::mousePressEvent(QMouseEvent *event)
{
    m_mouseLastPos = event->pos();
}

void GLView::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - m_mouseLastPos.x();
    int dy = event->y() - m_mouseLastPos.y();

    if (event->buttons() & Qt::RightButton) {
        panBy(dx, dy);
    }
    m_mouseLastPos = event->pos();
}

void GLView::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees = event->angleDelta() / 8;

    int x = event->x();
    int y = event->y();

    zoomBy(1 - 0.25f * numDegrees.y() / 15.0f, x, y);

    event->accept();
}

void GLView::zoomBy(float dzf, int mouseX, int mouseY)
{
    float pzf = zoomFactor;
    zoomFactor = zoomFactor * dzf;
    if(zoomFactor < minZoomFactor) zoomFactor = minZoomFactor;
    else if(zoomFactor > maxZoomFactor) zoomFactor = maxZoomFactor;
    m_eyePosX += (zoomFactor - pzf) * screenRatio * GLfloat(mouseX - screenWidth*0.5f) / GLfloat(screenWidth);
    m_eyePosY -= (zoomFactor - pzf) * GLfloat(mouseY - screenHeight*0.5f) / GLfloat(screenHeight);
    recalcView();
}
void GLView::panBy(int dx, int dy)
{
    m_eyePosX += zoomFactor * screenRatio * GLfloat(dx) / screenWidth;
    m_eyePosY -= zoomFactor * GLfloat(dy) / screenWidth;

    recalcView();
}
void GLView::recalcView()
{
    m_mProj.setToIdentity();

    if(perspectiveView)
    {
        m_mProj.perspective(45.0f, screenRatio, 0.01f, 100.0f);
        m_mProj.scale(1.0f, 1.0f, zoomFactor);
    }
    else
    {
        m_mProj.ortho(-zoomFactor * 0.5f * screenRatio, zoomFactor * 0.5f * screenRatio, -zoomFactor * 0.5f, zoomFactor * 0.5f, 0, 10);
    }
    m_mProj.translate(m_eyePosX, m_eyePosY, 0.0f);
    update();
}
