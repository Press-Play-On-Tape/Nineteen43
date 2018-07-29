#pragma once
 
#include "EnemyBullet.h"
#include "Bullet.h"
#include "Arduboy2Ext.h"
#include "Enums.h"

class EnemyBullet : public Bullet {

  public:

    EnemyBullet();

    void move();

};
