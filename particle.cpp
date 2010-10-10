#include "particle.h"

#include "Utils.h"

Particle::Particle(const QPointF &position, qreal radius)
        : position(position), radius(radius)
{
}

void Particle::draw(QPainter *painter, QPointF velocity)
{
    position += velocity * (rand()%2);

    painter->save();
    QPen pen(Qt::NoPen);
    QBrush brush(QColor(200,230,255));
    Utils::DrawCircle(position, 2*radius, brush, pen);
    painter->restore();
}
