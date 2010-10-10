#ifndef PARTICLE_H
#define PARTICLE_H

#include <QBrush>
#include <QColor>
#include <QPointF>
#include <QRect>
#include <QRectF>
#include <QVector2D>
#include <QRadialGradient>

class QPainter;

class Particle
{
public:
    Particle(const QPointF &position, qreal radius);
    ~Particle();

    void draw(QPainter *painter, QPointF velocity);

private:
    QPointF position;
    qreal radius;
};

#endif // PARTICLE_H
