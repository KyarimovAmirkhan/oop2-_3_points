#include "PlayerCharacter.h"
#include "GameGrid.h"
#include "SpellCard.h"
#include <iostream>

PlayerCharacter::PlayerCharacter(int x, int y, int hp, int closeDmg, int distDmg, int closeRange, int distRange) 
    : GameActor(x, y, hp), currentMode(BattleMode::CLOSE_COMBAT), 
      closeCombatDamage(std::max(1, closeDmg)),
      distanceCombatDamage(std::max(1, distDmg)),
      closeCombatRange(std::max(1, closeRange)),
      distanceCombatRange(std::max(1, distRange)),
      canMoveNextRound(true), spellHand(5, 3)
      {
    if (hp <= 0)
    {
        std::cout << "Предупреждение: Здоровье игрока должно быть > 0, установлено 100\n";
        stats.hitPoints = 100;
        stats.maximumHitPoints = 100;
    }
    setAttackRange(closeCombatRange);
}

void PlayerCharacter::toggleBattleMode()
{
    currentMode = (currentMode == BattleMode::CLOSE_COMBAT) ? 
                  BattleMode::DISTANCE_FIGHT : BattleMode::CLOSE_COMBAT;
    if (currentMode == BattleMode::CLOSE_COMBAT)
    {
        setAttackRange(closeCombatRange);
        std::cout << "Переключен на ближний бой. Урон: " << closeCombatDamage << ", Радиус: " << closeCombatRange << "\n";
    }
    else
    {
        setAttackRange(distanceCombatRange);
        std::cout << "Переключен на дальний бой. Урон: " << distanceCombatDamage << ", Радиус: " << distanceCombatRange << "\n";
    }
    canMoveNextRound = false;
}

void PlayerCharacter::receiveDamage(int damage)
{
    if (damage < 0)
    {
        std::cout << "Предупреждение: Попытка нанести отрицательный урон!\n";
        return;
    }
    GameActor::receiveDamage(damage);
    if (!isActive())
    {
        std::cout << "Игрок побежден!\n";
    }
}

bool PlayerCharacter::castSpell(int spellIndex, GameGrid& grid, int targetX, int targetY)
{
    if (spellIndex < 0)
    {
        std::cout << "Неверный индекс заклинания!\n";
        return false;
    }
    if (!canMoveNextRound)
    {
        std::cout << "Игрок не может действовать в этот ход!\n";
        return false;
    }
    bool result = spellHand.castSpell(spellIndex, grid, targetX, targetY);
    if (result)
    {
        canMoveNextRound = false; // Ход тратится только при успешном применении
    }
    return result;
}

void PlayerCharacter::displaySpells() const
{
    spellHand.displayHand();
}

void PlayerCharacter::onEnemyDefeated()
{
    spellHand.onEnemyDefeated();
}

bool PlayerCharacter::buySpell(std::unique_ptr<SpellCard> spell)
{
    if (!spell)
    {
        std::cout << "Ошибка: Попытка купить пустое заклинание!\n";
        return false;
    }
    return spellHand.buySpell(std::move(spell), this);
}

BattleMode PlayerCharacter::getCurrentMode() const
{
    return currentMode;
}

int PlayerCharacter::getCurrentDamage() const
{
    return (currentMode == BattleMode::CLOSE_COMBAT) ? closeCombatDamage : distanceCombatDamage;
}

bool PlayerCharacter::canMoveNextTurn() const
{
    return canMoveNextRound;
}

SpellHand& PlayerCharacter::getSpellHand()
{
    return spellHand;
}

void PlayerCharacter::setMovementAbility(bool able)
{
    canMoveNextRound = able;
}

void PlayerCharacter::restoreMovement()
{
    canMoveNextRound = true;
}