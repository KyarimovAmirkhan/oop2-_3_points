#ifndef SPELL_CARD_H
#define SPELL_CARD_H

#include <string>

class GameGrid;

class SpellCard
{
protected:
    std::string spellName;
    int manaCost;
    int castRange;
    
public:
    SpellCard(const std::string& name, int cost, int range);
    virtual ~SpellCard() = default;
    
    virtual bool castSpell(GameGrid& grid, int targetX, int targetY) = 0;
    virtual std::string getDescription() const = 0;
    
    std::string getName() const;
    int getManaCost() const;
    int getCastRange() const;
    bool isInRange(int casterX, int casterY, int targetX, int targetY) const;
};

#endif