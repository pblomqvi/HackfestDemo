#ifndef CRITTER_H
#define CRITTER_H

// Repserenset single entity which can work as particle emitter

#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QVector2D>
#include <QRadialGradient>

#include "tail.h"

class QPainter;



class Critter
{
public:
    Critter(const QPointF &position, qreal radius, const qreal velocity);
    ~Critter();

    void drawCritter(QPainter *painter);

    void updateBrush();
    void updateCache();

    void clearSteering();
    void steerForWander(qreal strength);
    bool steerToTarget(QPointF target, qreal strength);
    void move();

    void setTentacleTarget(QPointF target);

    QPointF pos();

    void setExpandingColor(QColor newColor);
    void updateColor();

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

    qreal radiantPos;
    QBrush gradientBrush;

    QList<Tentacle> tails;
    QPointF tentacleTarget;
    int wanderCounter;
    bool wanderLeft;
};


#endif // CRITTER_H
