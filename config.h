#ifndef CONFIG_H
#define CONFIG_H

#include <qglobal.h>

const bool DEBUG = false;
const bool USE_SOUND = true;

const int NUM_ENTITIES = 10;
const qreal ENTITY_SPEED = 4.0;
const qreal ENTITY_ANGULAR_TURN_SPEED = 5.0; // in degrees

const qreal CRITTER_SPEED = 2;
const qreal CRITTER_GRADIENT_SPEED = 0.01;
const qreal TAIL_TARGET_DIRECTION_STRENGTH = 0.2;
const qreal TAIL_TARGET_DIRECTION2_STRENGTH = 0.27;
const int   CRITTER_DETECTION_DELAY = 20;

const qreal TARGET_REACHED_DISTANCE = 10;

const qreal STEER_TO_TARGET_STRENGTH = 1.0;
const qreal STEER_TO_CENTER_STRENGTH = 0.7;
const qreal STEER_SEPARATION_STRENGTH = 0.8;
const qreal STEER_SEPARATION_DISTANCE_PENALTY_MULTIPLIES = 2;
const qreal STEER_COHESION_STRENGTH   = 0.2;
const qreal STEER_WANDER_STRENGTH = 2.0;
const qreal STEER_AVOID_WITHIN_DISTANCE_STRENGTH = 1.5;

const qreal STEER_WANDER_MAX_CHANGE = 5; // in degrees
const qreal STEER_WANDER_CHANGE_CHANGE = 0.1; // in degrees
const int   STEER_WANDER_COUNTER_DIVIDER = 10; // Only steer once in n frames

const qreal STEER_AVOID_DISTANCE = 130;

const qreal SINUOUS_TAIL_OFFSET_CHANGE_SPEED = 0.3;

#endif // CONFIG_H
