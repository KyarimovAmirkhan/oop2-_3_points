#ifndef ENEMY_CHARACTER_H
#define ENEMY_CHARACTER_H

#include "GameActor.h"

class EnemyCharacter : public GameActor
{
public:
    EnemyCharacter(int x = 0, int y = 0, int hp = 30, int dmg = 5);
};

#endif