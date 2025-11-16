#ifndef SPELL_HAND_H
#define SPELL_HAND_H

#include "SpellCard.h"
#include <vector>
#include <memory>

class GameGrid;
class PlayerCharacter;

class SpellHand
{
private:
    std::vector<std::unique_ptr<SpellCard>> spells;
    int maxHandSize;
    int enemiesDefeated;
    int enemiesForNewSpell;
    
public:
    SpellHand(int maxSize = 5, int enemiesForSpell = 3);
    
    bool addSpell(std::unique_ptr<SpellCard> spell);
    bool castSpell(int index, GameGrid& grid, int targetX, int targetY);
    void removeSpell(int index);
    
    void onEnemyDefeated();
    bool buySpell(std::unique_ptr<SpellCard> spell, PlayerCharacter* player);
    
    size_t getSpellCount() const;
    int getMaxHandSize() const;
    const SpellCard* getSpell(int index) const;
    void displayHand() const;
    
    std::unique_ptr<SpellCard> generateRandomSpell();
};

#endif