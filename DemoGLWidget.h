/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef DEMOGLWIDGET_H
#define DEMOGLWIDGET_H

#include <QGLWidget>
#include <QTime>
#include <QMouseEvent>
#include <QtOpenGL>

class Entity;
struct FBO;
class Critter;
class Particle;

class DemoGLWidget : public QGLWidget {

    Q_OBJECT
public:
    DemoGLWidget(QWidget *parent = 0);
    ~DemoGLWidget();
protected:
    void paintGL ();
    void initializeGL ();
    void mouseMoveEvent (QMouseEvent* event);

public:
    void loadReady();

private:
    void createEntities(int number);
    void randomTarget();


    QList<Entity*> entities;
    QList<Particle*> particles;
    Critter* critter;
    QImage backgroundImage;

    int frames;
    QTime time;
    QPointF targetLocation;

    FBO* fbo;
    QGLShaderProgram program;
    QGLFramebufferObject *render_fbo;
    QGLFramebufferObject *texture_fbo;

    Entity* entity1;
    Entity* entity2;
    Entity* entity3;
    Entity* entity4;
};
#endif
