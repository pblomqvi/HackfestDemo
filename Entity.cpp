#include "Entity.h"
#include "config.h"
#include "Utils.h"
#include "synth.h"

#include <QPainter>
#include <QPen>
#include <math.h>
#include <QQuaternion>

Entity::Entity(const QPointF &position, qreal radius, qreal velocity, int syncedInstrument)
    : position(position), vel(velocity), radius(radius),
    tail(position, 0, 7, radius / 1.2, 1.0), syncedInstrumentIndex(syncedInstrument)
{
    steeringVector = QVector2D();
    prevSteeringVector = QVector2D();
    directionVector = QVector2D(0.0, 1.0); // Default direction is towards World Y

    innerColor = QColor(40,40,90,20);
    outerColor = QColor(0,200,200,20);
    cache = 0;
    updateBrush();
}

Entity::~Entity()
{
    if (cache)
        delete cache;
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
    painter->save();

    // Draw tail first
    tail.drawTail(painter, &brush);

    //painter->setOpacity(0.8);
    painter->setOpacity(synth_get_current_envelope_for_instrument(syncedInstrumentIndex));

    painter->translate(position.x() - radius, position.y() - radius);
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
        toSelf /= (distance * distance * STEER_SEPARATION_DISTANCE_PENALTY_MULTIPLIES);
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

qreal Entity::steerToAvoindWithinDistance(QPointF target, qreal strength)
{
    QVector2D fromTarget = QVector2D(position - target);
    if(fromTarget.length() < STEER_AVOID_DISTANCE)
    {
        // Too close to target, avoid
        fromTarget.normalize();
        steeringVector += fromTarget * strength;
        steeringVector.normalize();
    }
    return fromTarget.length();
}

void Entity::move()
{
    // Directions angle from World-Y axis
    //double dirAngle   = Utils::VectorToAngle(directionVector);
    //double steerAngle = Utils::VectorToAngle(steeringVector);

    // Steering angle relative to direction
    /*if(steerAngle - dirAngle > 0)
    {
        // We turn right, calculate new direction (relative to World Y)
        directionVector = Utils::AngleToVector(dirAngle + ENTITY_ANGULAR_TURN_SPEED);
    }
    else
    {
        // We turn left, calculate new direction (relative to World Y)
        directionVector = Utils::AngleToVector(dirAngle - ENTITY_ANGULAR_TURN_SPEED);
    }
    if(fabs(steerAngle - dirAngle) <= ENTITY_ANGULAR_TURN_SPEED)
    {
        directionVector = steeringVector;
    }
    else
    {
        QQuaternion q = QQuaternion::fromAxisAndAngle(0, 1, 0, dirAngle);
        QVector3D rotetedSteering = q.rotatedVector(QVector3D(steeringVector));
        if(rotetedSteering.x() > 0) // In direction vectors coordinate system
        {
            // Have to turn right (in world coordinate system)
            directionVector = Utils::AngleToVector(dirAngle + ENTITY_ANGULAR_TURN_SPEED);
        }
        else
        {
            // Have to turn left (in world coordinate system)
            directionVector = Utils::AngleToVector(dirAngle - ENTITY_ANGULAR_TURN_SPEED);
        }
    }*/
    //if(DEBUG) Utils::DrawLine(position, position + 50*directionVector.toPointF());
    // Go to new position

    QVector2D newDirVec = directionVector + steeringVector * 0.1;
    newDirVec.normalize();
    float env = synth_get_current_envelope_for_instrument(syncedInstrumentIndex);
    position += newDirVec.toPointF() * (vel + env*radius*0.5f);
    directionVector = newDirVec;
    // Move tail as well
    tail.move(position);

    //if(DEBUG) Utils::DrawLine(position, position + 30*steeringVector.toPointF());

}

QPointF Entity::pos()
{
    return position;
}
