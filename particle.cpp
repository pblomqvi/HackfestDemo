#include "particle.h"

#include "Utils.h"

Particle::Particle(const QPointF &position, qreal radius, const qreal velocity)
        : position(position), vel(velocity), radius(radius)
{
}

void Particle::draw(QPainter *painter)
{
    position += QPointF(vel, vel);

    painter->save();
    QPen pen(Qt::white);
    pen.setWidth(2);
    painter->setPen(pen);
    painter->setBrush(QBrush());
    Utils::DrawCircle(position, radius);
    painter->restore();
}
