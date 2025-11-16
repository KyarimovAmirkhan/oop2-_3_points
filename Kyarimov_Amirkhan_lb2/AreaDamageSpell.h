#ifndef AREA_DAMAGE_SPELL_H
#define AREA_DAMAGE_SPELL_H

#include "SpellCard.h"

class AreaDamageSpell : public SpellCard
{
private:
    int damageAmount;
    int areaSize;
    
public:
    AreaDamageSpell(const std::string& name, int cost, int range, int damage, int size = 2);
    
    bool castSpell(GameGrid& grid, int targetX, int targetY) override;
    std::string getDescription() const override;
};

#endif