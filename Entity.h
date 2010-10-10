#ifndef ENTITY_H
#define ENTITY_H

// Repserenset single entity which can work as particle emitter

#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QVector2D>

#include "tail.h"

class QPainter;

class Entity
{
public:
    Entity(const QPointF &position, qreal radius, const qreal velocity, int syncedInstrument);
    ~Entity();

    void drawEntity(QPainter *painter);

    void updateBrush();
    void updateCache();

    void clearSteering();
    bool steerToTarget(QPointF target, qreal strength); // Returns true if target reached
    void steerWithFlock(QMutableListIterator<Entity*> localFlock,
                        qreal separationStrength,
                        qreal cohesionStrength);
    void steerToAvoindWithinDistance(QPointF target, qreal strength);

    void move();

private:

    QBrush brush;
    QPointF position;
    QVector2D steeringVector;
    QVector2D prevSteeringVector;
    QVector2D directionVector; // last move direction
    qreal vel;
    qreal radius;
    QColor innerColor;
    QColor outerColor;
    QImage *cache;
    int syncedInstrumentIndex;

    Tail tail;

};

#endif // ENTITY_H
