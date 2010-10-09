#include "Entity.h"
#include "config.h"
#include "Utils.h"
#include "synth.h"

#include <QPainter>
#include <QPen>
#include <math.h>

Entity::Entity(const QPointF &position, qreal radius, qreal velocity, TurningMode tMode)
    : position(position), vel(velocity), radius(radius), turningMode(tMode),
    tail(position, 7, radius / 1.2, 1.0)
{
    steeringVector = QVector2D();
    prevSteeringVector = QVector2D();
    directionVector = QVector2D(0.0, 1.0); // Default direction is towards World Y

    innerColor = randomColor();
    outerColor = randomColor();
    cache = 0;
    updateBrush();
}

Entity::~Entity()
{
    if (cache)
        delete cache;
}

QColor Entity::randomColor()
{
    int red = int(185 + 70.0*qrand()/(RAND_MAX+1.0));
    int green = int(185 + 70.0*qrand()/(RAND_MAX+1.0));
    int blue = int(205 + 50.0*qrand()/(RAND_MAX+1.0));
    int alpha = int(91 + 100.0*qrand()/(RAND_MAX+1.0));

    return QColor(red, green, blue, alpha);
}

void Entity::updateCache()
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

void Entity::updateBrush()
{
    QRadialGradient gradient(QPointF(radius, radius), radius,
                             QPointF(radius*0.5, radius*0.5));

    gradient.setColorAt(0, QColor(255, 255, 255, 255));
    gradient.setColorAt(0.25, innerColor);
    gradient.setColorAt(1, outerColor);
    brush = QBrush(gradient);
    updateCache();
}

void Entity::drawEntity(QPainter *painter)
{
    // Draw tail first
    tail.drawTail(painter);

    painter->save();
    painter->translate(position.x() - radius, position.y() - radius);
    //painter->setOpacity(0.8);
    painter->setOpacity(synth_get_current_envelope_for_instrument(0));
    painter->drawImage(0, 0, *cache);
    painter->restore();
}

void Entity::clearSteering()
{
    prevSteeringVector = steeringVector;
    steeringVector = QVector2D();
}

bool Entity::steerToTarget(QPointF target, qreal strength)
{
    QVector2D toTarget = QVector2D(target - position);
    bool targetReached = toTarget.length() < TARGET_REACHED_DISTANCE;
    toTarget.normalize();
    steeringVector += toTarget * strength;
    steeringVector.normalize();

    return targetReached;
}

void Entity::steerWithFlock(QMutableListIterator<Entity*> localFlock,
                            qreal separationStrength,
                            qreal cohesionStrength)
{
    // Separation is sum of vectors from local flockmates to this, effect dimisishes with distance
    // Cohesion is vector towards center of flock

    QVector2D separationVector = QVector2D(); // For separation
    QPointF cohesionCenter = position; // For cohesion, start with self
    int flockCounter = 1; // 1 == self
    while(localFlock.hasNext())
    {
        // Check next not-self flockmate
        Entity* flockMate = localFlock.next();

        // Calculate change to separation vector
        if(flockMate == this) continue; // Ignore self
        QVector2D toSelf = QVector2D(position - flockMate->position);
        qreal distance = toSelf.length();
        toSelf /= (distance * distance * STEER_SEPARATION_DISTANCE_PENALTA_MULTIPLIES);
        separationVector += toSelf;

        // Calculate cohesion center
        cohesionCenter += this->position;
        flockCounter++;
    }
    // Prepare separation
    separationVector.normalize();

    // Prepare cohesion
    cohesionCenter /= flockCounter;
    QVector2D cohesionVector = QVector2D(cohesionCenter - position);
    cohesionVector.normalize();

    // Combine flock steering
    steeringVector += (separationVector * separationStrength)
                      + (cohesionVector * cohesionStrength);
    steeringVector.normalize();
}

void Entity::move()
{
    if(turningMode == Smooth)
    {
        // Directions angle from World-Y axis
        double dirAngle   = Utils::VectorToAngle(directionVector);
        double steerAngle = Utils::VectorToAngle(steeringVector);

        // Steering angle relative to direction
        if(steerAngle - dirAngle > 0)
        {
            // We turn right, calculate new direction (relative to World Y)
            directionVector = Utils::AngleToVector(dirAngle + ENTITY_ANGULAR_TURN_SPEED);
        }
        else
        {
            // We turn left, calculate new direction (relative to World Y)
            directionVector = Utils::AngleToVector(dirAngle - ENTITY_ANGULAR_TURN_SPEED);
        }

        // Go to new position
        position += directionVector.toPointF() * vel;
    }
    else // Instant (Default)
    {
        steeringVector += prevSteeringVector;
        steeringVector.normalize();
        position += steeringVector.toPointF() * vel;
    }

    // Move tail as well
    tail.move(position);

    //if(DEBUG) Utils::DrawLine(position, position + 10*steeringVector.toPointF());

}
