#ifndef DIRECT_DAMAGE_SPELL_H
#define DIRECT_DAMAGE_SPELL_H

#include "SpellCard.h"

class DirectDamageSpell : public SpellCard
{
private:
    int damageAmount;
    
public:
    DirectDamageSpell(const std::string& name, int cost, int range, int damage);
    
    bool castSpell(GameGrid& grid, int targetX, int targetY) override;
    std::string getDescription() const override;
};

#endif