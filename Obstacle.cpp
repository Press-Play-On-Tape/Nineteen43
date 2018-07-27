#include "Obstacle.h"
#include "Arduboy2.h"
#include "Sprites.h"
#include "Enums.h"
#include "FixedPoints.h"
#include "FixedPointsCommon.h"

Obstacle::Obstacle(const SQ15x16 x, const SQ15x16 y, const uint8_t *bitmapRef, const uint8_t *maskRef, const SQ7x8 speed, const SQ7x8 value) :
          Base(x, y) {

  _enabled = false;
  _value = value;
  _speed = speed;
  _bitmap = bitmapRef;
  _mask = maskRef;
    
}

Rect Obstacle::getRect() {

  return (Rect){ _x.getInteger(), _y.getInteger(), pgm_read_byte(&_bitmap[0]), pgm_read_byte(&_bitmap[1]) };

}

void Obstacle::move() {

  if (_enabled) {
    _x = _x - (SQ15x16)_speed;
  }

}

SQ7x8 Obstacle::getValue() {

  return _value;

}

void Obstacle::setValue(SQ7x8 value) {

  _value = value;

}

SQ7x8 Obstacle::getSpeed() {

  return _value;

}

void Obstacle::setSpeed(SQ7x8 value) {

  _speed = value;

}

ObstacleType Obstacle::getObstacleType() {

  return _obstacleType;

}

void Obstacle::setObstacleType(ObstacleType value) {

  _obstacleType = value;
  
}

void Obstacle::setBitmap(const uint8_t *value) {

  _bitmap = value;
  
}

void Obstacle::setMask(const uint8_t *value) {

  _mask = value;
  
}

void Obstacle::renderImage() {
    
  if (_enabled && _x.getInteger() + OBSTACLE_WIDTH >= 0 && _x.getInteger() < WIDTH) {
    Sprites::drawExternalMask(_x.getInteger(), _y.getInteger(), _bitmap, _mask, 0, 0);
  }
  else if (_x.getInteger() + OBSTACLE_WIDTH < 0) {
    _enabled = false;
  }

}
