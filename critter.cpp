#include "critter.h"
#include "config.h"
#include "Utils.h"
#include "synth.h"

#include <QPainter>
#include <QPen>
#include <math.h>

Critter::Critter(const QPointF &position, qreal radius, qreal velocity)
    : position(position), vel(velocity), radius(radius), tails()
{
    // Create tentacles with differing angles
    tails.append(Tentacle(position,    0, 5, 8.0 ,1.0));
    tails.append(Tentacle(position,   72, 5, 8.0 ,1.0));
    tails.append(Tentacle(position,  144, 5, 8.0 ,1.0));
    tails.append(Tentacle(position,  -72, 5, 8.0 ,1.0));
    tails.append(Tentacle(position, -144, 5, 8.0 ,1.0));

    wanderCounter = 0;
    wanderLeft = true;

    steeringVector = QVector2D();
    prevSteeringVector = QVector2D();
    directionVector = QVector2D(0.0, 1.0); // Default direction is towards World Y

    innerColor = Utils::randomColor();
    outerColor = Utils::randomColor();
    cache = 0;
    updateBrush();

    radiantPos = 0.0;
    gradientBrush = Utils::createRadialGradientBrush(position, tails[0].length(),
                                                     innerColor, outerColor,
                                                     radiantPos);

}

Critter::~Critter()
{
    if (cache)
        delete cache;
}

void Critter::updateCache()
{
    if (cache)
        delete cache;
    cache = new QImage(qRound(radius * 2 + 2), qRound(radius * 2 + 2), QImage::Format_ARGB32);
    cache->fill(0x00000000);
    QPainter p(cache);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    QPen pen(Qt::white);
    pen.setWidth(2);
    p.setPen(pen);
    p.setBrush(brush);
    p.drawEllipse(0, 0, int(2*radius), int(2*radius));
}

void Critter::updateBrush()
{
    QRadialGradient gradient(QPointF(radius, radius), radius,
                             QPointF(radius*0.5, radius*0.5));

    gradient.setColorAt(0, QColor(255, 255, 255, 255));
    gradient.setColorAt(0.25, innerColor);
    gradient.setColorAt(1, outerColor);
    brush = QBrush(gradient);
    updateCache();
}

void Critter::drawCritter(QPainter *painter)
{
    painter->save();

    float opacity = synth_get_current_envelope_for_instrument(0);

    if (opacity < 0.1f) opacity = 0.1f;
    painter->setOpacity(opacity);

    // Draw tail first
    for(int i = 0; i < tails.size(); i++)
    {
        tails[i].drawTail(painter, &gradientBrush);
    }

    painter->translate(position.x() - radius, position.y() - radius);

    painter->drawImage(0, 0, *cache);
    painter->restore();
}

void Critter::clearSteering()
{
    prevSteeringVector = steeringVector;
    steeringVector = QVector2D();
}

void Critter::steerForWander(qreal strength)
{
    // Change wander direction once every STEER_WANDER_COUNTER_DIVIDER frames
    wanderCounter = (wanderCounter + 1) % STEER_WANDER_COUNTER_DIVIDER;
    if(wanderCounter == 0) wanderLeft = (qrand()/(RAND_MAX+1.0) > 0.5) ? true : false;

    // Steer towards selected wander direction
    qreal direction = Utils::VectorToAngle(directionVector);
    if(wanderLeft) direction -= STEER_WANDER_MAX_CHANGE;
    else direction += STEER_WANDER_MAX_CHANGE;
    steeringVector += Utils::AngleToVector(direction) * strength;
    steeringVector.normalize();
}

bool Critter::steerToTarget(QPointF target, qreal strength)
{
    QVector2D toTarget = QVector2D(target - position);
    bool targetReached = toTarget.length() < TARGET_REACHED_DISTANCE;
    toTarget.normalize();
    steeringVector += toTarget * strength;
    steeringVector.normalize();

    return targetReached;
}

void Critter::move()
{
    QPointF newPosition = position + steeringVector.toPointF() * vel;
    directionVector = QVector2D(newPosition - position);
    position = newPosition;

    // Move tails as well
    for(int i = 0; i < tails.size(); i++)
    {
        tails[i].move(position);
    }

    gradientBrush = Utils::createRadialGradientBrush(position, tails[0].length(),
                                                     innerColor, outerColor,
                                                     radiantPos);
}

QPointF Critter::pos()
{
    return position;
}

void Critter::setExpandingColor(QColor newColor)
{
    innerColor = outerColor;
    outerColor = newColor;
    radiantPos = 0.01;
}

void Critter::updateColor()
{
    /*
    radiantPos += CRITTER_GRADIENT_SPEED;
    if(radiantPos > 0.99) radiantPos = 0.99;
    */

    radiantPos = synth_get_current_envelope_for_instrument(0);
}
