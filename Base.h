#pragma once

#include "FixedPoints.h"
#include "FixedPointsCommon.h"

class Base {

  public:

    Base(SQ15x16 x, SQ15x16 y);

    SQ15x16 getX();
    SQ15x16 getY();
    bool getEnabled();

    void setX(SQ15x16 value);
    void setY(SQ15x16 value);
    void setEnabled(bool value);

  protected:

    SQ15x16 _x;               
    SQ15x16 _y;               
    bool _enabled;

};

