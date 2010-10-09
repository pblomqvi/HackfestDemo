#ifndef CONFIG_H
#define CONFIG_H

#include <qglobal.h>

const bool DEBUG = true;

const int NUM_ENTITIES = 10;
const qreal ENTITY_SPEED = 2.0;
const qreal ENTITY_ANGULAR_TURN_SPEED = 5.0; // in degrees

const qreal TARGET_REACHED_DISTANCE = 10;

const int LOCAL_FLOCK_SIZE = 4;

const qreal STEER_TO_TARGET_STRENGTH  = 1.0;
const qreal STEER_SEPARATION_STRENGTH = 0.5;
const qreal STEER_SEPARATION_DISTANCE_PENALTA_MULTIPLIES = 2;
const qreal STEER_COHESION_STRENGTH   = 0.2;


#endif // CONFIG_H
