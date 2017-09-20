#ifndef ENEMY_BULLET_H
#define ENEMY_BULLET_H
 
#include "EnemyBullet.h"
#include "Bullet.h"
#include "Arduboy2.h"
#include "Enums.h"

class EnemyBullet : public Bullet {

  public:

    EnemyBullet();

    void move();

};

#endif
