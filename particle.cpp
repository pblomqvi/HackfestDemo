#include "particle.h"

#include "Utils.h"

Particle::Particle(const QPointF &position, qreal radius)
        : position(position), radius(radius)
{
}

void Particle::draw(QPainter *painter, QPointF velocity)
{
    position += velocity;

    painter->save();
    QPen pen(Qt::NoPen);
    QBrush brush(Qt::cyan);
    Utils::DrawCircle(position, 2*radius, brush, pen);
    painter->restore();
}
