#include "tail.h"
#include "Utils.h"

#include <QVector2D>

Tail::Tail(const QPointF& pos, int numSegments, qreal startRadius, qreal radiusDiminish)
{
    if(numSegments < 1) return;

    // Add first segment
    segments.append(TailSegment(pos, startRadius, 0));
    numSegments--;
    qreal radius = startRadius - radiusDiminish;
    TailSegment* prevSegment = &segments[0];

    // Add rest of the segments
    for(int i = 1; numSegments > 0; i++)
    {
        segments.append(TailSegment(pos, radius, prevSegment));
        numSegments--;
        radius -= radiusDiminish;
        prevSegment = &segments[i];

        if(radius <= 0) break; // Stop when radius gets too small
    }
}

void Tail::drawTail(QPainter *painter)
{
    for(int i = 0; i < segments.size(); i++)
    {
        segments[i].drawSegment(painter);
    }
}

void Tail::move(QPointF newPos)
{
    // Move the first segment
    segments[0].pos = newPos;

    // Move the rest
    for(int i = 1; i < segments.length(); i++)
    {
        segments[i].moveSegment();
    }
}

Tail::TailSegment::TailSegment(QPointF newPos, qreal newRadius, TailSegment* prev)
    :pos(newPos), radius(newRadius), prevSegment(prev) {}

void Tail::TailSegment::drawSegment(QPainter* painter)
{
    Utils::DrawCircle(pos, radius);
}

void Tail::TailSegment::moveSegment()
{
    // Segment location is based on previous segments location
    QVector2D offsetVector = QVector2D(pos - prevSegment->pos);
    offsetVector.normalize();
    pos = prevSegment->pos + (offsetVector * (radius+prevSegment->radius)).toPointF();
}
