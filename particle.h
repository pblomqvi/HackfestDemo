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
    Particle(const QPointF &position, qreal radius, const qreal velocity);
    ~Particle();

    void draw(QPainter *painter);

private:
    QPointF position;
    qreal vel;
    qreal radius;
};

#endif // PARTICLE_H
