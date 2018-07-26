#include "Base.h"
#include "FixedPoints.h"
#include "FixedPointsCommon.h"

Base::Base(SQ15x16 x, SQ15x16 y) {

  _x = x;
  _y = y;

}

SQ15x16 Base::getX() {

  return _x;

}

void Base::setX(SQ15x16 value) {

  _x = value;

}

SQ15x16 Base::getY() {

  return _y;

}

void Base::setY(SQ15x16 value) {

  _y = value;

}

bool Base::getEnabled() {

  return _enabled;

}

void Base::setEnabled(bool value) {

  _enabled = value;

}



