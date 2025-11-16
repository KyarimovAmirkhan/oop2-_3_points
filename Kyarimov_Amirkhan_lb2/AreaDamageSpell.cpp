#include "AreaDamageSpell.h"
#include "GameGrid.h"
#include <iostream>

AreaDamageSpell::AreaDamageSpell(const std::string& name, int cost, int range, int damage, int size)
    : SpellCard(name, cost, range), 
      damageAmount(std::max(1, damage)),
      areaSize(std::max(1, size)) {}

bool AreaDamageSpell::castSpell(GameGrid& grid, int targetX, int targetY)
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
    bool hitAnything = false;
    int enemiesHit = 0;
    std::cout << spellName << " взрывается в точке (" << targetX << ", " << targetY << ")!\n";
    for (int dy = -areaSize/2; dy <= areaSize/2; dy++)
    {
        for (int dx = -areaSize/2; dx <= areaSize/2; dx++)
        {
            int areaX = targetX + dx;
            int areaY = targetY + dy;
            if (grid.isPositionValid(areaX, areaY))
            {
                auto& enemies = grid.getEnemies();
                auto& enemyPositions = grid.getEnemyPositions();
                for (size_t i = 0; i < enemyPositions.size(); i++)
                {
                    if (enemyPositions[i].first == areaX && enemyPositions[i].second == areaY)
                    {
                        if (enemies[i]->isActive())
                        {
                            enemies[i]->receiveDamage(damageAmount);
                            std::cout << "  Враг в (" << areaX << ", " << areaY << ") получает " 
                                      << damageAmount << " урона! ";
                            std::cout << "Здоровье: " << enemies[i]->getHitPoints() << "/" << enemies[i]->getMaxHitPoints() << "\n";
                            hitAnything = true;
                            enemiesHit++;
                            if (!enemies[i]->isActive())
                            {
                                grid.getPlayer()->addPoints(10);
                                std::cout << "  Враг побежден! +10 очков\n";
                            }
                        }
                    }
                }
            }
        }
    }
    if (hitAnything)
    {
        std::cout << "Затронуто врагов: " << enemiesHit << "\n";
    }
    else
    {
        std::cout << "Заклинание рассеивается без эффекта.\n";
    }
    
    return true;
}

std::string AreaDamageSpell::getDescription() const
{
    return spellName + " - Наносит " + std::to_string(damageAmount) + 
           " урона по области " + std::to_string(areaSize) + "x" + std::to_string(areaSize) + 
           ". Радиус: " + std::to_string(castRange) + ", Стоимость: " + std::to_string(manaCost);
}