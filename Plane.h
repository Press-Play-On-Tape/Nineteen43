#ifndef PLANE_H
#define PLANE_H
 
#include "Sprites.h"
#include "Base.h"
#include "Enums.h"
#include "FixedPoints.h"
#include "FixedPointsCommon.h"

class Plane : public Base {

  public:

    Plane(const uint16_t images);
    
    const Rect getRect() const;
    const SQ7x8 getHealth() const;

    
    void setHealth(const SQ7x8 value);
    void addHealth(const SQ7x8 value);
    void decHealth(const SQ7x8 value);

  protected:

    SQ7x8 _health;
    uint16_t _bitmaps;

};
#endif
