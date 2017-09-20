#ifndef OBSTACLE_H
#define OBSTACLE_H
 
#include "Arduboy2.h"
#include "Sprites.h"
#include "Arduino.h"
#include "Enums.h"
#include "Base.h"

class Obstacle : public Base {
  
  public:
   
    Obstacle(const SQ15x16 x, const SQ15x16 y, const uint8_t *bitmapRef, const uint8_t *maskRef, const SQ7x8 speed, const uint8_t value);

    const uint8_t getValue() const;
    const SQ7x8 getSpeed() const;
    const ObstacleType getObstacleType() const;
    const Rect getRect() const;
           
    void setValue(const uint8_t value);
    void setSpeed(const SQ7x8 value);
    void setObstacleType(const ObstacleType value);
    void setBitmap(const uint8_t *value);
    void setMask(const uint8_t *value);
         
    void move();
    void renderImage();  
    
  private:
             
    SQ7x8 _speed;               

    uint8_t _value;
    ObstacleType _obstacleType;
    const uint8_t *_bitmap;
    const uint8_t *_mask;
};
#endif

