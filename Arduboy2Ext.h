#pragma once

#include <Arduboy2.h>
#include "Enums.h"

#ifdef ORIENTATION_HORIZONTAL
class Arduboy2Ext : public Arduboy2 {

  public:

    Arduboy2Ext();

    uint8_t justPressedButtons() const;
    uint8_t pressedButtons() const;
    uint16_t getFrameCount() const;
    uint8_t getFrameCount(uint8_t mod) const;

    void clearButtonState();
    
    void drawHorizontalDottedLine(uint8_t x1, uint8_t x2, uint8_t y, uint8_t spacing);
    void drawVerticalDottedLine(uint8_t y1, uint8_t y2, uint8_t x, uint8_t spacing);
    
};
#endif
#ifdef ORIENTATION_VERTICAL
class Arduboy2Ext : public Arduboy2Base {

  public:

    Arduboy2Ext();

    uint8_t justPressedButtons() const;
    uint8_t pressedButtons() const;
    uint16_t getFrameCount() const;
    uint8_t getFrameCount(uint8_t mod) const;

    void clearButtonState();
    
    void drawHorizontalDottedLine(uint8_t x1, uint8_t x2, uint8_t y, uint8_t spacing);
    void drawVerticalDottedLine(uint8_t y1, uint8_t y2, uint8_t x, uint8_t spacing);
    
};
#endif