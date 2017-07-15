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
#include "depthmapX/gllinesuniform.h"

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

public slots:
    void setXPosition(float xPos);
    void setYPosition(float yPos);
    void setZPosition(float zPos);
    void cleanup();

signals:
    void xPositionChanged(float xPos);
    void yPositionChanged(float yPos);
    void zPositionChanged(float zPos);

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int width, int height) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    bool perspectiveView = false;
    bool m_core;
    float m_xPos;
    float m_yPos;
    float m_zPos;
    QPoint m_lastPos;
    GLLinesUniform m_lineData;
    QMatrix4x4 m_mProj;
    QMatrix4x4 m_mView;
    QMatrix4x4 m_mModel;
    QGraphDoc* pDoc;
    const QRgb &m_foreground;
    const QRgb &m_background;
};

