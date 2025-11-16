#ifndef CHARACTER_ATTRIBUTES_H
#define CHARACTER_ATTRIBUTES_H

struct CharacterAttributes
{
    int hitPoints;
    int maximumHitPoints;
    int attackPower;
    int points;
    CharacterAttributes(int hp = 100, int dmg = 10, int pts = 0) 
        : hitPoints(hp), maximumHitPoints(hp), attackPower(dmg), points(pts) {}
};

#endif