#include "DemoGLWidget.h"
#include <QPainter>
#include <QPaintEngine>
#include <math.h>

#include "config.h"
#include "Utils.h"
#include "Entity.h"
#include "critter.h"

DemoGLWidget::DemoGLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    setAttribute(Qt::WA_PaintOnScreen);
    setAttribute(Qt::WA_NoSystemBackground);
    setAutoBufferSwap(false);
#ifndef Q_WS_QWS
    setMinimumSize(300, 250);
#endif

    frames = 0;
    randomTarget();
}

DemoGLWidget::~DemoGLWidget()
{
}

void DemoGLWidget::initializeGL ()
{
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

    createEntities(NUM_ENTITIES);
}

void DemoGLWidget::paintGL()
{
    QPainter painter;
    painter.begin(this);
    Utils::setPainter(&painter);

    painter.beginNativePainting();

    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);

    painter.endNativePainting();

    // Draw Entities and Critter
    foreach (Entity *entity, entities)
    {
        entity->drawEntity(&painter);
    }
    critter->drawCritter(&painter);

    // Display FPS
    QString framesPerSecond;
    framesPerSecond.setNum(frames /(time.elapsed() / 1000.0), 'f', 2);
    painter.setPen(Qt::white);
    painter.drawText(20, 40, framesPerSecond + " fps");

    // Update critter location
    critter->clearSteering();
    critter->steerForWander(STEER_WANDER_STRENGTH);
    bool reached = critter->steerToTarget(targetLocation, STEER_TO_TARGET_STRENGTH);
    if(reached) randomTarget();
    critter->move();
    critter->updateColor();

    // Move entities
    for(int i = 0; i < entities.size(); i++)
    {
        Entity *entity = entities[i];

        // Use all entities as local flock for now
        QMutableListIterator<Entity*> localFlock(entities);

        entity->clearSteering();
        entity->steerToTarget(critter->pos(), STEER_TO_TARGET_STRENGTH);
        entity->steerWithFlock(localFlock, STEER_SEPARATION_STRENGTH, STEER_COHESION_STRENGTH);
        entity->steerToAvoindWithinDistance(critter->pos(), STEER_AVOID_WITHIN_DISTANCE_STRENGTH);
        entity->move();
    }

    swapBuffers();
    painter.end();

    //Do frame rate calculations
    if (!(frames % 100)) {
        time.start();
        frames = 0;
    }
    frames ++;

}

void DemoGLWidget::createEntities(int number)
{
    for (int i = 0; i < number; ++i) {
        QPointF position(width()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))),
                        height()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))));
        qreal radius = qMin(width(), height())*(0.0175 + 0.0875*qrand()/(RAND_MAX+1.0));
        qreal velocity = ENTITY_SPEED;

        entities.append(new Entity(position, radius, velocity));
    }

    critter = new Critter(QPointF(500,200), 10, CRITTER_SPEED);
}

void DemoGLWidget::randomTarget()
{
    targetLocation = QPointF(width()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))),
                    height()*(0.1 + (0.8*qrand()/(RAND_MAX+1.0))));
    if(DEBUG) printf("RandomSteerToTarget: (%.2f, %.2f)\n", targetLocation.x(), targetLocation.y());
}

void DemoGLWidget::mouseMoveEvent (QMouseEvent* event)
{
    targetLocation = QPointF(event->x(), event->y());
    event->accept();

    critter->setExpandingColor(Utils::randomColor());
}

