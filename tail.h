#ifndef TAIL_H
#define TAIL_H

#include <QPointF>
#include <QList>
#include <QPainter>

class Tail
{
public:
    Tail(const QPointF& pos, int numSegments, qreal startRadius, qreal radiusDiminish);

    void drawTail(QPainter *painter);
    void move(QPointF newPos);

private:

    class TailSegment
    {
    public:
        TailSegment(QPointF newPos, qreal newRadius, TailSegment* prev);
        void drawSegment(QPainter* painter);
        void moveSegment();

        QPointF pos;
        qreal radius;
        TailSegment* prevSegment;
    };


    QList<TailSegment> segments;
};

#endif // TAIL_H
