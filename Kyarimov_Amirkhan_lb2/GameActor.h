#ifndef GAME_ACTOR_H
#define GAME_ACTOR_H

#include "CharacterAttributes.h"

class GameActor
{
protected:
    CharacterAttributes stats;
    int posX, posY;
    int attackRange;
    
public:
    GameActor(int x = 0, int y = 0, int hp = 100, int dmg = 10, int range = 1);
    virtual ~GameActor() = default;
    
    virtual void receiveDamage(int damage);
    bool isActive() const;
    
    int getHitPoints() const;
    int getMaxHitPoints() const;
    int getAttackPower() const;
    int getPoints() const;
    int getAttackRange() const;
    void getPosition(int& x, int& y) const;
    
    void setPosition(int x, int y);
    void addPoints(int value);
    void setAttackRange(int range);
};

#endif