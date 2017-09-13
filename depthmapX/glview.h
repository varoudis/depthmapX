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

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include "GraphDoc.h"
#include "depthmapX/gllines.h"
#include "depthmapX/gllinesuniform.h"
#include "depthmapX/glrastertexture.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLView : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLView(QWidget *parent = 0,
           QGraphDoc* doc = NULL,
           const QRgb &backgroundColour = qRgb(0,0,0),
           const QRgb &foregroundColour = qRgb(255,255,255));
    ~GLView();

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;
    void notifyDatasetChanged() { m_datasetChanged = true; update(); }
    void matchViewToCurrentMetaGraph();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void wheelEvent(QWheelEvent *event) override;

private:
    bool m_perspectiveView = false;
    bool m_coreProfile;
    QMatrix4x4 m_mProj;
    QMatrix4x4 m_mView;
    QMatrix4x4 m_mModel;

    QGraphDoc* m_pDoc;
    const QRgb &m_foreground;
    const QRgb &m_background;

    GLLines m_axes;
    GLLinesUniform m_grid;
    GLLines m_visibleAxial;
    GLLinesUniform m_visibleDrawingLines;
    GLRasterTexture m_visiblePointMap;
    GLLines m_visibleDataMap;

    QPoint m_mouseLastPos;
    float m_eyePosX;
    float m_eyePosY;
    float m_minZoomFactor = 1;
    float m_zoomFactor = 20;
    float m_maxZoomFactor = 200;
    GLfloat m_screenRatio;
    int m_screenWidth;
    int m_screenHeight;

    bool m_datasetChanged = false;

    void panBy(int dx, int dy);
    void recalcView();
    void zoomBy(float dzf, int mouseX, int mouseY);
    void matchViewToRegion(QtRegion region);

    void loadAxes();
    void loadDrawingGLObjects();
    void loadDataMapGLObjects();
    void loadAxialGLObjects();
    void loadVGAGLObjects();
    void loadVGAGLObjectsRequiringGLContext();
};

