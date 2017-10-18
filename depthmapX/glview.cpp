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
#include "salalib/linkutils.h"
#include "salalib/geometrygenerators.h"
#include <QMouseEvent>
#include <QCoreApplication>

static QRgb colorMerge(QRgb color, QRgb mergecolor)
{
   return (color & 0x006f6f6f) | (mergecolor & 0x00a0a0a0);
}

GLView::GLView(QWidget *parent, QGraphDoc* doc, const QRgb &backgroundColour, const QRgb &foregroundColour)
    : QOpenGLWidget(parent),
      m_eyePosX(0),
      m_eyePosY(0),
      m_background(backgroundColour),
      m_foreground(foregroundColour)
{
    m_core = QCoreApplication::arguments().contains(QStringLiteral("--coreprofile"));
    m_pDoc = doc;


    loadDrawingGLObjects();

    loadAxes();

    if(m_pDoc->m_meta_graph->getViewClass() & m_pDoc->m_meta_graph->VIEWAXIAL) {
        m_visibleShapeGraph.loadGLObjects(m_pDoc->m_meta_graph->getDisplayedShapeGraph());
    }
    m_visiblePointMap.setGridColour(colorMerge(foregroundColour, backgroundColour));
    if(m_pDoc->m_meta_graph->getViewClass() & m_pDoc->m_meta_graph->VIEWVGA) {
        m_visiblePointMap.loadGLObjects(m_pDoc->m_meta_graph->getDisplayedPointMap());
    }

    if(m_pDoc->m_meta_graph->getViewClass() & m_pDoc->m_meta_graph->VIEWDATA) {
        m_visibleDataMap.loadGLObjects(m_pDoc->m_meta_graph->getDisplayedDataMap());
    }

    matchViewToCurrentMetaGraph();
}

GLView::~GLView()
{
    makeCurrent();
    m_axes.cleanup();
    m_visibleDrawingLines.cleanup();
    m_visiblePointMap.cleanup();
    m_visibleShapeGraph.cleanup();
    m_visibleDataMap.cleanup();
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
    m_visibleDrawingLines.initializeGL(m_core);
    m_visiblePointMap.initializeGL(m_core);
    m_visibleShapeGraph.initializeGL(m_core);
    m_visibleDataMap.initializeGL(m_core);

    if(m_pDoc->m_meta_graph->getViewClass() & m_pDoc->m_meta_graph->VIEWVGA) {
         m_visiblePointMap.loadGLObjectsRequiringGLContext(m_pDoc->m_meta_graph->getDisplayedPointMap());
    }

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
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);

    if(m_datasetChanged) {

        loadDrawingGLObjects();
        m_visibleDrawingLines.updateGL(m_core);

        if(m_pDoc->m_meta_graph->getViewClass() & m_pDoc->m_meta_graph->VIEWAXIAL) {
            m_visibleShapeGraph.loadGLObjects(m_pDoc->m_meta_graph->getDisplayedShapeGraph());
            m_visibleShapeGraph.updateGL(m_core);
        }

        if(m_pDoc->m_meta_graph->getViewClass() & m_pDoc->m_meta_graph->VIEWDATA) {
            m_visibleDataMap.loadGLObjects(m_pDoc->m_meta_graph->getDisplayedDataMap());
            m_visibleDataMap.updateGL(m_core);
        }

        if(m_pDoc->m_meta_graph->getViewClass() & m_pDoc->m_meta_graph->VIEWVGA) {
            m_visiblePointMap.loadGLObjects(m_pDoc->m_meta_graph->getDisplayedPointMap());
            m_visiblePointMap.updateGL(m_core);
            m_visiblePointMap.loadGLObjectsRequiringGLContext(m_pDoc->m_meta_graph->getDisplayedPointMap());
        }

        m_datasetChanged = false;
    }

    if(m_pDoc->m_meta_graph->getViewClass() & m_pDoc->m_meta_graph->VIEWVGA) {
        m_visiblePointMap.paintGLOverlay(m_mProj, m_mView, m_mModel);
    }
    if(m_pDoc->m_meta_graph->getViewClass() & m_pDoc->m_meta_graph->VIEWAXIAL) {
        m_visibleShapeGraph.paintGLOverlay(m_mProj, m_mView, m_mModel);
    }

    m_visibleDrawingLines.paintGL(m_mProj, m_mView, m_mModel);

    if(m_pDoc->m_meta_graph->getViewClass() & m_pDoc->m_meta_graph->VIEWVGA) {
        m_visiblePointMap.paintGL(m_mProj, m_mView, m_mModel);
    }

    if(m_pDoc->m_meta_graph->getViewClass() & m_pDoc->m_meta_graph->VIEWAXIAL) {
        m_visibleShapeGraph.paintGL(m_mProj, m_mView, m_mModel);
    }

    if(m_pDoc->m_meta_graph->getViewClass() & m_pDoc->m_meta_graph->VIEWDATA) {
        m_visibleDataMap.paintGL(m_mProj, m_mView, m_mModel);
    }

    m_axes.paintGL(m_mProj, m_mView, m_mModel);
}

void GLView::loadAxes() {
    std::vector<std::pair<SimpleLine, PafColor>> axesData;
    axesData.push_back(std::pair<SimpleLine, PafColor> (SimpleLine(0,0,1,0), PafColor(1,0,0)));
    axesData.push_back(std::pair<SimpleLine, PafColor> (SimpleLine(0,0,0,1), PafColor(0,1,0)));
    m_axes.loadLineData(axesData);
}

void GLView::loadDrawingGLObjects() {
    m_pDoc->m_meta_graph->setLock(this);
    m_visibleDrawingLines.loadLineData(m_pDoc->m_meta_graph->getVisibleDrawingLines(), m_foreground);
    m_pDoc->m_meta_graph->releaseLock(this);
}

void GLView::resizeGL(int w, int h)
{
    m_screenWidth = w;
    m_screenHeight = h;
    m_screenRatio = GLfloat(w) / h;
    recalcView();
}

void GLView::mouseReleaseEvent(QMouseEvent *event)
{
    Point2f worldPoint = getWorldPoint(event->pos());
    if (!m_pDoc->m_communicator) {
        QtRegion r( worldPoint, worldPoint );
        bool selected = false;
        switch(m_mouseMode)
        {
        case MOUSE_MODE_NONE:
        {
            // nothing, deselect
            m_pDoc->m_meta_graph->clearSel();
            break;
        }
        case MOUSE_MODE_SELECT:
        {
            // typical selection
            m_pDoc->m_meta_graph->setCurSel( r, false );
            break;
        }
        case MOUSE_MODE_JOIN:
        {
            selected = m_pDoc->m_meta_graph->setCurSel( r, false );
            if(selected) {
                m_mouseMode = MOUSE_MODE_JOIN | MOUSE_MODE_SECOND_POINT;
            }
            break;
        }
        case MOUSE_MODE_JOIN | MOUSE_MODE_SECOND_POINT:
        {
            int selectedCount = m_pDoc->m_meta_graph->getSelCount();
            if (selectedCount > 0) {
                if (m_pDoc->m_meta_graph->getState() & MetaGraph::POINTMAPS) {
                    m_pDoc->m_meta_graph->getDisplayedPointMap().mergePoints( worldPoint );
                } else if (m_pDoc->m_meta_graph->getState() & MetaGraph::SHAPEGRAPHS && selectedCount == 1) {
                    m_pDoc->m_meta_graph->setCurSel( r, true ); // add the new one to the selection set
                    const pvecint& selectedSet = m_pDoc->m_meta_graph->getSelSet();
                    if (selectedSet.size() == 2) {
                        // axial is only joined one-by-one
                        m_pDoc->modifiedFlag = true;
                        m_pDoc->m_meta_graph->getDisplayedShapeGraph().linkShapes(selectedSet[0], selectedSet[1], true);
                        m_pDoc->m_meta_graph->clearSel();
                    }
                }
                m_pDoc->m_meta_graph->clearSel();
                m_mouseMode = MOUSE_MODE_JOIN;
            }
            break;
        }
        case MOUSE_MODE_UNJOIN:
        {
            selected = m_pDoc->m_meta_graph->setCurSel( r, false );
            if(selected) {
                if (m_pDoc->m_meta_graph->getState() & MetaGraph::POINTMAPS) {
                    if (m_pDoc->m_meta_graph->getDisplayedPointMap().unmergePoints()) {
                        m_pDoc->modifiedFlag = true;
                        m_pDoc->SetRedrawFlag(QGraphDoc::VIEW_ALL,QGraphDoc::REDRAW_GRAPH, QGraphDoc::NEW_DATA);
                    }
                } else {
                    m_mouseMode = MOUSE_MODE_UNJOIN | MOUSE_MODE_SECOND_POINT;
                }
            }
            break;
        }
        case MOUSE_MODE_UNJOIN | MOUSE_MODE_SECOND_POINT:
        {
            int selectedCount = m_pDoc->m_meta_graph->getSelCount();
            if (selectedCount > 0) {
                if (m_pDoc->m_meta_graph->getState() & MetaGraph::SHAPEGRAPHS && selectedCount == 1) {
                    m_pDoc->m_meta_graph->setCurSel( r, true ); // add the new one to the selection set
                    const pvecint& selectedSet = m_pDoc->m_meta_graph->getSelSet();
                    if (selectedSet.size() == 2) {
                        // axial is only joined one-by-one
                        m_pDoc->modifiedFlag = true;
                        m_pDoc->m_meta_graph->getDisplayedShapeGraph().unlinkShapes(selectedSet[0], selectedSet[1], true);
                        m_pDoc->m_meta_graph->clearSel();
                    }
                }
                m_pDoc->m_meta_graph->clearSel();
                m_mouseMode = MOUSE_MODE_UNJOIN;
            }
            break;
        }
        }

        m_pDoc->SetRedrawFlag(QGraphDoc::VIEW_ALL,QGraphDoc::REDRAW_POINTS, QGraphDoc::NEW_SELECTION);
    }
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
    float pzf = m_zoomFactor;
    m_zoomFactor = m_zoomFactor * dzf;
    if(m_zoomFactor < m_minZoomFactor) m_zoomFactor = m_minZoomFactor;
    else if(m_zoomFactor > m_maxZoomFactor) m_zoomFactor = m_maxZoomFactor;
    m_eyePosX += (m_zoomFactor - pzf) * m_screenRatio * GLfloat(mouseX - m_screenWidth*0.5f) / GLfloat(m_screenWidth);
    m_eyePosY -= (m_zoomFactor - pzf) * GLfloat(mouseY - m_screenHeight*0.5f) / GLfloat(m_screenHeight);
    recalcView();
}
void GLView::panBy(int dx, int dy)
{
    m_eyePosX += m_zoomFactor * GLfloat(dx) / m_screenHeight;
    m_eyePosY -= m_zoomFactor * GLfloat(dy) / m_screenHeight;

    recalcView();
}
void GLView::recalcView()
{
    m_mProj.setToIdentity();

    if(m_perspectiveView)
    {
        m_mProj.perspective(45.0f, m_screenRatio, 0.01f, 100.0f);
        m_mProj.scale(1.0f, 1.0f, m_zoomFactor);
    }
    else
    {
        m_mProj.ortho(-m_zoomFactor * 0.5f * m_screenRatio, m_zoomFactor * 0.5f * m_screenRatio, -m_zoomFactor * 0.5f, m_zoomFactor * 0.5f, 0, 10);
    }
    m_mProj.translate(m_eyePosX, m_eyePosY, 0.0f);
    update();
}

Point2f GLView::getWorldPoint(const QPoint &screenPoint) {
    return Point2f(+ m_zoomFactor * float(screenPoint.x() - m_screenWidth*0.5)  / m_screenHeight - m_eyePosX,
                   - m_zoomFactor * float(screenPoint.y() - m_screenHeight*0.5) / m_screenHeight - m_eyePosY);

}

void GLView::matchViewToCurrentMetaGraph() {
    const QtRegion &region = m_pDoc->m_meta_graph->getBoundingBox();
    matchViewToRegion(region);
    recalcView();
}

void GLView::matchViewToRegion(QtRegion region) {
    if((region.top_right.x == 0 && region.bottom_left.x == 0)
            || (region.top_right.y == 0 && region.bottom_left.y == 0))
        // region is unset, don't try to change the view to it
        return;
    m_eyePosX = - (region.top_right.x + region.bottom_left.x)*0.5f;
    m_eyePosY = - (region.top_right.y + region.bottom_left.y)*0.5f;
    if(region.width() > region.height())
    {
        m_zoomFactor = region.top_right.x - region.bottom_left.x;
    }
    else
    {
        m_zoomFactor = region.top_right.y - region.bottom_left.y;
    }
    m_minZoomFactor = m_zoomFactor * 0.001;
    m_maxZoomFactor = m_zoomFactor * 10;
}

void GLView::OnModeJoin()
{
    if (m_pDoc->m_meta_graph->getState() & (MetaGraph::POINTMAPS | MetaGraph::SHAPEGRAPHS)) {
        m_mouseMode = MOUSE_MODE_JOIN;
        m_visiblePointMap.showLinks(true);
        m_visibleShapeGraph.showLinks(true);
        m_pDoc->m_meta_graph->clearSel();
        notifyDatasetChanged();
    }
}

void GLView::OnModeUnjoin()
{
    if (m_pDoc->m_meta_graph->getState() & (MetaGraph::POINTMAPS | MetaGraph::SHAPEGRAPHS)) {
        m_mouseMode = MOUSE_MODE_UNJOIN;
        m_visiblePointMap.showLinks(true);
        m_visibleShapeGraph.showLinks(true);
        m_pDoc->m_meta_graph->clearSel();
        notifyDatasetChanged();
    }
}
