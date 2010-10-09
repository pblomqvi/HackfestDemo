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
    void drawTail(QPainter *painter);
    void move(QPointF newPos);

protected:

    class TailSegment
    {
    public:
        TailSegment(QPointF newPos, qreal newRadius, TailSegment* prev);
        void drawSegment(QPainter* painter);
        void moveSegment();
        void moveSegment(QVector2D targetDirection);

        void updateSinuousOffset();  // Not used currently

        QPointF pos;
        qreal radius;
        TailSegment* prevSegment;
    };

    QList<TailSegment> segments;
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
