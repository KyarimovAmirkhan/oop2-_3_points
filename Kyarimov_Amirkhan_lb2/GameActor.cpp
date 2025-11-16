#include "GameActor.h"

GameActor::GameActor(int x, int y, int hp, int dmg, int range) 
    : stats(hp, dmg), posX(x), posY(y), attackRange(range) {}

void GameActor::receiveDamage(int damage)
{
    stats.hitPoints = (stats.hitPoints - damage > 0) ? stats.hitPoints - damage : 0;
}

bool GameActor::isActive() const
{
    return stats.hitPoints > 0;
}

int GameActor::getHitPoints() const
{
    return stats.hitPoints;

}

int GameActor::getMaxHitPoints() const
{
    return stats.maximumHitPoints;
}

int GameActor::getAttackPower() const
{
    return stats.attackPower;
}

int GameActor::getPoints() const
{
    return stats.points;
}

int GameActor::getAttackRange() const
{
    return attackRange;
}

void GameActor::getPosition(int& x, int& y) const
{
    x = posX;
    y = posY;
}

void GameActor::setPosition(int x, int y)
{
    posX = x;
    posY = y;
}

void GameActor::addPoints(int value)
{
    stats.points += value;
}

void GameActor::setAttackRange(int range)
{
    attackRange = range;
}