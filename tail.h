#ifndef TAIL_H
#define TAIL_H

#include <QPointF>
#include <QList>
#include <QPainter>
#include <QVector2D>

class Tail
{
public:
    Tail(const QPointF& pos, qreal angle, int numSegments, qreal startRadius, qreal radiusDiminish);
    void drawTail(QPainter *painter, QBrush* brush = 0);
    void move(QPointF newPos);
    qreal length();

protected:

    class TailSegment
    {
    public:
        TailSegment(QPointF newPos, qreal newRadius, TailSegment* prev);
        void drawSegment(QPainter* painter, QBrush* brush);
        void moveSegment();
        void moveSegment(QVector2D targetDirection);

        void updateSinuousOffset();  // Not used currently

        QPointF pos;
        qreal radius;
        TailSegment* prevSegment;
    };

    QList<TailSegment> segments;
    qreal tailLength;
};

class Tentacle : public Tail
{
public:
    Tentacle(const QPointF& pos, qreal angle, int numSegments, qreal startRadius, qreal radiusDiminish);
    void move(QPointF newPos);
    void setTentacleAngle(qreal targetAngle); // [-180..180]

    qreal tentacleAngle;
    QVector2D tentacleTargetDir;
};

#endif // TAIL_H
