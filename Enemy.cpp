#include "Enemy.h"
#include "Sprites.h"
#include "Enums.h"
#include "FixedPoints.h"
#include "FixedPointsCommon.h"
#include "Sequences.h"

#define SCREEN_OFFSET_X 64.0
#define SCREEN_OFFSET_Y 32.0

Enemy::Enemy(const EnemyType type, 
             const uint8_t * const * images) : 

             Plane(images) {

  _type = type;
  _enabled = false;
  _bulletsFired = ENEMY_NO_BULLETS_FIRED_MAX;
  _repeatSeqStep = 0;
  _health = 0;
  _explosionImage = 0;
  _turretDirection = Direction::West;

}

const Rect Enemy::getRect() {

  #ifndef PLANES_HAVE_BORDERS
  return (Rect){ _x.getInteger(), _y.getInteger(), pgm_read_byte(pgm_read_word_near(&_bitmaps[static_cast<uint8_t>(Direction::West)])), pgm_read_byte(pgm_read_word_near(&_bitmaps[static_cast<uint8_t>(Direction::West) + 1])) };
  #else
  return (Rect){ _x.getInteger() + 1, _y.getInteger() + static_cast<uint8_t>(1), pgm_read_byte(pgm_read_word_near(&_bitmaps[static_cast<uint8_t>(Direction::West)])) - static_cast<uint8_t>(2), pgm_read_byte(pgm_read_word_near(&_bitmaps[static_cast<uint8_t>(Direction::West) + static_cast<uint8_t>(1)])) + static_cast<uint8_t>(2) };
  #endif

}

uint8_t Enemy::getWidth() {

  return pgm_read_byte(pgm_read_word_near(&_bitmaps[static_cast<uint8_t>(Direction::West)])); // The boat uses only image 7.  Images 1-6 and 8 are used to store the turret.

}

uint8_t Enemy::getHeight() {

  return pgm_read_byte(pgm_read_word_near(&_bitmaps[static_cast<uint8_t>(Direction::West) + 1]));

}

EnemyType Enemy::getEnemyType() {

  return _type;

}

void Enemy::setEnemyType(EnemyType value) {

  _type = value;

}

Direction Enemy::getDirection() {

  return _direction;

}

void Enemy::setDirection(Direction value) {

  _direction = value;

}

Direction Enemy::getTurretDirection() {

  return _turretDirection;

}

void Enemy::setTurretDirection(Direction value) {

  _turretDirection = value;

}

void Enemy::setStartingPos(const int8_t *value) {

  _startingPos = value;
  _currentPos = 0;
  
}

int8_t Enemy::getOffsetX() {

  return _offsetX;

}

void Enemy::setOffsetX(int8_t value) {

  _offsetX = value;

}

int8_t Enemy::getOffsetY() {

  return _offsetY;

}

void Enemy::setOffsetY(int8_t value) {

  _offsetY = value;

}

uint8_t Enemy::getDelayStart() {

  return _delayStart;

}

void Enemy::setDelayStart(uint8_t value) {

  _delayStart = value;

}

bool Enemy::getInvertX() {

  return _invertX;

}

void Enemy::setInvertX(bool value) {

  _invertX = value;

}

bool Enemy::getInvertY() {

  return _invertY;

}

void Enemy::setInvertY(bool value) {

  _invertY = value;

}

void Enemy::setHealth(SQ7x8 value) {

  Plane::setHealth(value);
  _explosionImage = 0;

}

void Enemy::decHealth(SQ7x8 value) {

  Plane::decHealth(value);
  _explosionImage = 3;

}

uint8_t Enemy::getNumberOfBulletsFired() {

  return _bulletsFired;

}

void Enemy::setNumberOfBulletsFired(const uint8_t value) {

  _bulletsFired = value;

}

void Enemy::decNumberOfBulletsFired() {

  --_bulletsFired;

}

void Enemy::renderImage() {

  int16_t x = _x.getInteger();
  int16_t y = _y.getInteger();
  
  if (_enabled && _delayStart == 0 && x + this->getWidth() >= 0 && x < WIDTH) {

    Sprites::drawExternalMask(x, y, pgm_read_word_near(&_bitmaps[static_cast<uint8_t>(_direction)]), pgm_read_word_near(&_bitmaps[IMAGES_MASK_OFFSET + static_cast<uint8_t>(_direction)]), 0, 0);
    
    if (_type == EnemyType::Boat && _turretDirection != Direction::West) {

      Sprites::drawExternalMask(x + ENEMY_BOAT_TURRENT_X, y + ENEMY_BOAT_TURRENT_Y, pgm_read_word_near(&_bitmaps[static_cast<uint8_t>(_turretDirection)]), pgm_read_word_near(&_bitmaps[IMAGES_MASK_OFFSET + static_cast<uint8_t>(_turretDirection)]), 0, 0);
      
    }

    if (_health <= 0) {
 
      _health = _health - 0.25;
      
      const auto healthInt = _health.getInteger();
      const auto absHealthInt = abs(healthInt);
    
      switch (healthInt) {
  
        case -1 ... 0:
          Sprites::drawExternalMask(x, y, pgm_read_word_near(&_bitmaps[IMAGES_EXPLOSION_OFFSET + static_cast<uint8_t>(absHealthInt)]), pgm_read_word_near(&_bitmaps[IMAGES_EXPLOSION_MASK_OFFSET + static_cast<uint8_t>(absHealthInt)]), 0, 0);
          break;
          
        case -3 ... -2:
#ifndef PLANES_HAVE_BORDERS       
          Sprites::drawOverwrite(x, y, pgm_read_word_near(&_bitmaps[IMAGES_EXPLOSION_OFFSET + static_cast<uint8_t>(absHealthInt)]), 0);
#else
          Sprites::drawExternalMask(x, y, pgm_read_word_near(&_bitmaps[IMAGES_EXPLOSION_OFFSET + static_cast<uint8_t>(absHealthInt)]), pgm_read_word_near(&_bitmaps[IMAGES_EXPLOSION_MASK_OFFSET + static_cast<uint8_t>(absHealthInt)]), 0, 0);
#endif
          break;

        case -4:
#ifndef PLANES_HAVE_BORDERS       
          Sprites::drawOverwrite(x, y, pgm_read_word_near(&_bitmaps[IMAGES_EXPLOSION_OFFSET + 3]), 0);
#else
          Sprites::drawExternalMask(x, y, pgm_read_word_near(&_bitmaps[IMAGES_EXPLOSION_OFFSET + 3]), pgm_read_word_near(&_bitmaps[IMAGES_EXPLOSION_MASK_OFFSET + 3]), 0, 0);
#endif
          _enabled = false;
          break;
        
      }
      
	  }
	
    if (_explosionImage > 0) {
	
      switch (_explosionImage.getInteger()) {
	
        case 1:
        case 3:
          Sprites::drawExternalMask(x, y, pgm_read_word_near(&_bitmaps[IMAGES_EXPLOSION_OFFSET]), pgm_read_word_near(&_bitmaps[IMAGES_EXPLOSION_MASK_OFFSET]), 0, 0);
          break;

        case 2:
          Sprites::drawExternalMask(x, y, pgm_read_word_near(&_bitmaps[IMAGES_EXPLOSION_OFFSET + 1]), pgm_read_word_near(&_bitmaps[IMAGES_EXPLOSION_MASK_OFFSET + 1]), 0, 0);
          break;

      }
          
      --_explosionImage = _explosionImage - 0.25;
      
    }
        
  }

}

void Enemy::move() {

  if (_enabled) {

    if (_delayStart > 0) {
      
      _delayStart--;
      
    }
    
    if (_delayStart == 0) {

      if (_repeatSeqStep == 0) {

        const uint8_t temp = (uint8_t)pgm_read_byte(&_startingPos[_currentPos + 2]);
        int8_t moveX = (int8_t)pgm_read_byte(&_startingPos[_currentPos + 0]);
        int8_t moveY = (int8_t)pgm_read_byte(&_startingPos[_currentPos + 1]);
        moveX = (_invertX ? -moveX : moveX);
        moveY = (_invertY ? -moveY : moveY);
    
        if (temp != END_SEQUENCE_UINT) {
        
          if (temp < (uint8_t)Direction::Count) {

            _x = moveX + _offsetX + SCREEN_OFFSET_X;
            _y = moveY + _offsetY + SCREEN_OFFSET_Y;
            
            if (_type == EnemyType::Boat) {
              _direction = Direction::West;
            }
            else {
              _direction = (Direction)temp;
            }
 

			      // If the sequence has been inverted then the directions need to be transposed ..
			
            if (_invertX && !_invertY) { _direction = inverseX[(uint8_t)_direction]; }
            if (!_invertX && _invertY) { _direction = inverseY[(uint8_t)_direction]; }
            if (_invertX && _invertY)  { _direction = inverseXandY[(uint8_t)_direction]; }
      
          }
          else {

            _repeatSeqXInc = moveX;
            _repeatSeqYInc = moveY;
            _repeatSeqStep = (int8_t)temp;

            _x = _x + _repeatSeqXInc;
            _y = _y + _repeatSeqYInc;
            --_repeatSeqStep;
        
          }

          _currentPos = _currentPos + 3;
          
        }
        else {

          _enabled = false;
        
        }
      
      }
      else {

        _x = _x + _repeatSeqXInc;
        _y = _y + _repeatSeqYInc;
        --_repeatSeqStep;
      
      }

    }
    
  }
  
}
