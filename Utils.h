#ifndef UTILS_H
#define UTILS_H

#include <QPointF>
#include <QPainter>
#include <QBrush>

struct FBO {
        unsigned int frame, depth, texid;
        int width, height;
};

class Utils
{
public:
    static void setPainter(QPainter* painter);
    static void clearPainter();

    static void DrawLine(QPointF from, QPointF to);
    static void DrawCircle(QPointF pos, qreal radius);
    static void DrawCircle(QPointF pos, qreal radius, QBrush brush);
    static void DrawCircle(QPointF pos, qreal radius, QBrush brush, QPen pen);

    static QColor randomColor();
    static QBrush createRadialGradientBrush(const QPointF& pos, qreal radius,
                                            QColor inner, QColor outer,
                                            qreal phase);


    static void InitFBO(FBO *fbo);
    static void DeleteFBO(FBO *fbo);

    // Angle in degrees from positive Y-axis in range [-180, 180]
    static qreal VectorToAngle(QVector2D vec);

    // Returns unit vector that points from given angle away from positive Y
    // (given in [-180, 180] degrees)
    static QVector2D AngleToVector(qreal angle);

private:
    static QPainter* painter;
};



#endif // UTILS_H
