#ifndef PLAYER_CHARACTER_H
#define PLAYER_CHARACTER_H

#include "GameActor.h"
#include "BattleType.h"
#include "SpellHand.h"
#include <memory>

class GameGrid;
class SpellCard;

class PlayerCharacter : public GameActor
{
private:
    BattleMode currentMode;
    int closeCombatDamage;
    int distanceCombatDamage;
    int closeCombatRange;
    int distanceCombatRange;
    bool canMoveNextRound;
    SpellHand spellHand;
    
public:
    PlayerCharacter(int x = 0, int y = 0, int hp = 100, 
                   int closeDmg = 15, int distDmg = 10,
                   int closeRange = 1, int distRange = 3);
    
    void toggleBattleMode();
    void receiveDamage(int damage) override;
    
    bool castSpell(int spellIndex, GameGrid& grid, int targetX, int targetY);
    void displaySpells() const;
    void onEnemyDefeated();
    bool buySpell(std::unique_ptr<SpellCard> spell);
    
    BattleMode getCurrentMode() const;
    int getCurrentDamage() const;
    bool canMoveNextTurn() const;
    SpellHand& getSpellHand();
    
    void setMovementAbility(bool able);
    void restoreMovement();
};

#endif