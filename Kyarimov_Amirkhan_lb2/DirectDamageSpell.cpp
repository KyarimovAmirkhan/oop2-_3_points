#include "DirectDamageSpell.h"
#include "GameGrid.h"
#include <iostream>

DirectDamageSpell::DirectDamageSpell(const std::string& name, int cost, int range, int damage)
    : SpellCard(name, cost, range), damageAmount(std::max(1, damage)) {}

bool DirectDamageSpell::castSpell(GameGrid& grid, int targetX, int targetY)
{
    int playerX, playerY;
    grid.getPlayerPosition(playerX, playerY);
    if (!isInRange(playerX, playerY, targetX, targetY))
    {
        std::cout << spellName << ": Цель вне радиуса действия! (нужно: " << castRange << " клеток)\n";
        return false;
    }
    if (!grid.isPositionValid(targetX, targetY))
    {
        std::cout << spellName << ": Цель за пределами поля!\n";
        return false;
    }
    if (grid.hasEnemyAt(targetX, targetY))
    {
        auto& enemies = grid.getEnemies();
        auto& enemyPositions = grid.getEnemyPositions();
        for (size_t i = 0; i < enemyPositions.size(); i++)
        {
            if (enemyPositions[i].first == targetX && enemyPositions[i].second == targetY)
            {
                if (enemies[i]->isActive())
                {
                    enemies[i]->receiveDamage(damageAmount);
                    std::cout << spellName << " попадает во врага и наносит " << damageAmount << " урона!\n";
                    if (!enemies[i]->isActive())
                    {
                        grid.getPlayer()->addPoints(10);
                        std::cout << "Враг побежден! +10 очков\n";
                    }
                    else
                    {
                        std::cout << "Здоровье врага: " << enemies[i]->getHitPoints() << "/" << enemies[i]->getMaxHitPoints() << "\n";
                    }
                    return true;
                }
            }
        }
    }
    std::cout << spellName << ": Нет подходящей цели в указанной позиции!\n";
    return false;
}

std::string DirectDamageSpell::getDescription() const
{
    return spellName + " - Наносит " + std::to_string(damageAmount) + 
           " урона одной цели. Радиус: " + std::to_string(castRange) + 
           ", Стоимость: " + std::to_string(manaCost);
}