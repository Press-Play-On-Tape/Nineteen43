#pragma once
 
#include "Sprites.h"
#include "Enums.h"
#include "Plane.h"

class Enemy : public Plane {

  public: 

    Enemy(const EnemyType type, 
          const uint8_t * const * images);
        
    const Rect getRect();
    void renderImage();
    void move();
    
    EnemyType getEnemyType();
    Direction getDirection();
    Direction getTurretDirection();
//    int8_t *getStartingPos();
    int8_t getOffsetX();
    int8_t getOffsetY();
    uint8_t getDelayStart();
    bool getInvertX();
    bool getInvertY();
    uint8_t getNumberOfBulletsFired();
    uint8_t getWidth();
    uint8_t getHeight();
    
    void setHealth(const SQ7x8 value);
    void decHealth(const SQ7x8 value);    
        
    void setEnemyType(EnemyType value);
    void setDirection(Direction value);
    void setTurretDirection(Direction value);
    void setStartingPos(const int8_t *startingPos);
    void setOffsetX(int8_t value);
    void setOffsetY(int8_t value);
    void setDelayStart(uint8_t value);
    void setInvertX(bool value);
    void setInvertY(bool value);
    void setNumberOfBulletsFired(uint8_t value);
    void decNumberOfBulletsFired();
       
  private:
  
    EnemyType _type;
    Direction _direction;               
    Direction _turretDirection;               
    int8_t _offsetX;
    int8_t _offsetY;
    SQ7x8 _explosionImage;
    uint8_t _bulletsFired;
    
    
    // Used to track progress along sequence ..

    bool _invertX;
    bool _invertY;
    uint8_t _delayStart;
    int16_t _currentPos;
    const int8_t *_startingPos;

    
    // Store the details of a 'repeat' step in a sequence ..
    
    uint8_t _repeatSeqStep;
    int8_t _repeatSeqXInc;
    int8_t _repeatSeqYInc;

};