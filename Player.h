#pragma once
 
#include "Arduboy2Ext.h"
#include "Sprites.h"
#include "Arduino.h"
#include "Enums.h"
#include "Plane.h"

class Player : public Plane {
  
  public:
   
    Player(const uint8_t * const * images);

    void renderImage();
    void initGame();
    void initMission();

    SQ7x8 getFuel();
    uint8_t getBullets();
    uint16_t getScore();
    uint16_t getGrandScore();
    bool inRoll();
    bool getPowerUp();

    void setFuel(SQ7x8 value);
    void decFuel(SQ7x8 value);
    void setBullets(uint8_t value);
    void addBullets(uint8_t value);
    void decBullets();
    void setScore(uint16_t value);
    void setGrandScore(uint16_t value);
    void setPowerUp(boolean value);
    void decPowerUp();
    void startRoll();
  
  private:
    uint16_t _score;
    uint16_t _grandScore;
    uint8_t _bullets;
    SQ7x8 _fuel;
    uint8_t _rollState;
    uint8_t _powerUp;
    
};

