#ifndef UTILS_H
#define UTILS_H

#include <QPointF>
#include <QPainter>

class Utils
{
public:
    static void setPainter(QPainter* painter);
    static void clearPainter();

    static void DrawLine(QPointF from, QPointF to);
    static void DrawCircle(QPointF pos, qreal radius);

    // Angle in degrees from positive Y-axis in range [-180, 180]
    static qreal VectorToAngle(QVector2D vec);

    // Returns unit vector that points from given angle away from positive Y
    // (given in [-180, 180] degrees)
    static QVector2D AngleToVector(qreal angle);

private:
    static QPainter* painter;
};



#endif // UTILS_H
