#include "tail.h"
#include "Utils.h"
#include "config.h"

#include <QVector2D>

Tail::Tail(const QPointF& pos, qreal angle, int numSegments, qreal startRadius, qreal radiusDiminish)
{
    if(numSegments < 1) return;

    // Calculate direction of the startup direction of the tail
    QVector2D startOffsetDirection = Utils::AngleToVector(angle);
    QVector2D nextSegmentPos = QVector2D(pos);

    // Add first segment
    segments.append(TailSegment(nextSegmentPos.toPointF(), startRadius, 0));
    numSegments--;
    qreal radius = startRadius - radiusDiminish;
    nextSegmentPos += (startOffsetDirection * (startRadius + radius));
    TailSegment* prevSegment = &segments[0];
    tailLength = 2*startRadius;


    // Add rest of the segments
    for(int i = 1; numSegments > 0; i++)
    {
        segments.append(TailSegment(nextSegmentPos.toPointF(), radius, prevSegment));
        numSegments--;
        tailLength += 2*radius;
        qreal newRadius = radius - radiusDiminish;
        nextSegmentPos += (startOffsetDirection * (radius + newRadius));
        radius = newRadius;
        prevSegment = &segments[i];
        if(radius < 1) break; // Stop when radius gets too small
    }
}

void Tail::drawTail(QPainter *painter, QBrush* brush)
{
    for(int i = 0; i < segments.size(); i++)
    {
        segments[i].drawSegment(painter, brush);
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

qreal Tail::length()
{
    return tailLength;
}

Tail::TailSegment::TailSegment(QPointF newPos, qreal newRadius, TailSegment* prev)
    :pos(newPos), radius(newRadius), prevSegment(prev)
{
}

void Tail::TailSegment::drawSegment(QPainter* painter, QBrush* brush)
{
    if(brush)
    {
        Utils::DrawCircle(pos, radius, *brush);
    }
    else
    {
        Utils::DrawCircle(pos, radius);
    }

}

void Tail::TailSegment::moveSegment()
{
    // Segment location is based on previous segments location
    QVector2D offsetVector = QVector2D(pos - prevSegment->pos);
    offsetVector.normalize();
    pos = prevSegment->pos + (offsetVector * (radius+prevSegment->radius)).toPointF();
}

void Tail::TailSegment::moveSegment(QVector2D targetDirection)
{
    // Segment location is based on previous segments location
    QVector2D offsetVector = QVector2D(pos - prevSegment->pos);
    offsetVector.normalize();
    offsetVector += targetDirection * TAIL_TARGET_DIRECTION_STRENGTH;
    offsetVector.normalize();

    pos = prevSegment->pos + (offsetVector * (radius+prevSegment->radius)).toPointF();
}

void Tail::TailSegment::updateSinuousOffset()
{
    qreal sinuousOffset = 0.0;  // HUOM OBS MUY IMPORTANTE!!
    bool sinuousOffsetIncreasing = true;

    // Calculate new offset
    if(sinuousOffsetIncreasing)
    {
        sinuousOffset += SINUOUS_TAIL_OFFSET_CHANGE_SPEED;
        if(sinuousOffset >= 1.0 - SINUOUS_TAIL_OFFSET_CHANGE_SPEED)
        {
            sinuousOffset = 1.0;
            sinuousOffsetIncreasing = false;
        }
    }
    else
    {
        sinuousOffset -= SINUOUS_TAIL_OFFSET_CHANGE_SPEED;
        if(sinuousOffset <= -1.0 + SINUOUS_TAIL_OFFSET_CHANGE_SPEED)
        {
            sinuousOffset = -1.0;
            sinuousOffsetIncreasing = true;
        }
    }
}

Tentacle::Tentacle(const QPointF& pos, qreal angle, int numSegments, qreal startRadius, qreal radiusDiminish)
    :Tail(pos, angle, numSegments, startRadius, radiusDiminish)
{
    setTentacleAngle(angle);
}

void Tentacle::move(QPointF newPos)
{
    // Tentacle move takes tentacleAngle into account
    // Move the first segment
    segments[0].pos = newPos;

    // Move the rest
    for(int i = 1; i < segments.length(); i++)
    {
        segments[i].moveSegment(tentacleTargetDir);
    }
}
void Tentacle::setTentacleAngle(qreal targetAngle)
{
    tentacleAngle = targetAngle;
    tentacleTargetDir = Utils::AngleToVector(tentacleAngle);
}
