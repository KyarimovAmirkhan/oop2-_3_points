#include "SpellHand.h"
#include "GameGrid.h"
#include "PlayerCharacter.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include <iostream>
#include <random>
#include <ctime>

SpellHand::SpellHand(int maxSize, int enemiesForSpell)
    : maxHandSize(maxSize), enemiesDefeated(0), enemiesForNewSpell(enemiesForSpell)
    {
    if (maxHandSize <= 0)
    {
        std::cout << "Предупреждение: maxHandSize должен быть > 0, установлено 5\n";
        maxHandSize = 5;
    }
    if (enemiesForSpell <= 0)
    {
        std::cout << "Предупреждение: enemiesForSpell должен быть > 0, установлено 3\n";
        enemiesForNewSpell = 3;
    }
    addSpell(generateRandomSpell());
}

bool SpellHand::addSpell(std::unique_ptr<SpellCard> spell)
{
    if (!spell)
    {
        std::cout << "Ошибка: Попытка добавить пустое заклинание!\n";
        return false;
    }
    if (spells.size() >= static_cast<size_t>(maxHandSize))
    {
        std::cout << "Рука заклинаний заполнена! Нельзя добавить новое заклинание.\n";
        return false;
    }
    spells.push_back(std::move(spell));
    std::cout << "Добавлено заклинание: " << spells.back()->getName() << "\n";
    return true;
}

bool SpellHand::castSpell(int index, GameGrid& grid, int targetX, int targetY)
{
    if (index < 0 || index >= static_cast<int>(spells.size()))
    {
        std::cout << "Неверный индекс заклинания! Доступно: 0-" << (spells.size() - 1) << "\n";
        return false;
    }
    if (!spells[index])
    {
        std::cout << "Ошибка: Заклинание с индексом " << index << " не существует!\n";
        return false;
    }
    return spells[index]->castSpell(grid, targetX, targetY);
}

void SpellHand::removeSpell(int index)
{
    if (index >= 0 && index < static_cast<int>(spells.size()))
    {
        std::string spellName = spells[index]->getName();
        spells.erase(spells.begin() + index);
        std::cout << "Заклинание " << spellName << " удалено из руки.\n";
    }
    else
    {
        std::cout << "Неверный индекс для удаления заклинания!\n";
    }
}

void SpellHand::onEnemyDefeated()
{
    enemiesDefeated++;
    std::cout << "Врагов побеждено: " << enemiesDefeated << "/" << enemiesForNewSpell << "\n";
    if (enemiesDefeated >= enemiesForNewSpell)
    {
        enemiesDefeated = 0;
        std::cout << "Вы получили новое заклинание!\n";
        if (!addSpell(generateRandomSpell()))
        {
            std::cout << "Не удалось добавить новое заклинание (рука заполнена)!\n";
        }
    }
}

bool SpellHand::buySpell(std::unique_ptr<SpellCard> spell, PlayerCharacter* player)
{
    (void)player;
    if (!spell)
    {
        std::cout << "Ошибка: Попытка купить пустое заклинание!\n";
        return false;
    }
    return addSpell(std::move(spell));
}

size_t SpellHand::getSpellCount() const
{
    return spells.size();
}

int SpellHand::getMaxHandSize() const
{
    return maxHandSize;
}

const SpellCard* SpellHand::getSpell(int index) const
{
    if (index >= 0 && index < static_cast<int>(spells.size()))
    {
        return spells[index].get();
    }
    return nullptr;
}

void SpellHand::displayHand() const
{
    std::cout << "=== Рука заклинаний (" << spells.size() << "/" << maxHandSize << ") ===\n";
    if (spells.empty())
    {
        std::cout << "Заклинаний нет\n";
    }
    else
    {
        for (size_t i = 0; i < spells.size(); i++)
        {
            if (spells[i])
            {
                std::cout << i << ". " << spells[i]->getDescription() << "\n";
            }
            else
            {
                std::cout << i << ". [ОШИБКА: пустое заклинание]\n";
            }
        }
    }
    std::cout << "Врагов до следующего заклинания: " << (enemiesForNewSpell - enemiesDefeated) << "\n";
}

std::unique_ptr<SpellCard> SpellHand::generateRandomSpell()
{
    std::mt19937 rng(std::time(nullptr));
    std::uniform_int_distribution<int> spellType(0, 1);
    std::uniform_int_distribution<int> damageDist(20, 35);
    std::uniform_int_distribution<int> rangeDist(2, 4);
    std::uniform_int_distribution<int> costDist(1, 3);
    if (spellType(rng) == 0)
    {
        int damage = damageDist(rng);
        int range = rangeDist(rng);
        int cost = costDist(rng);
        return std::make_unique<DirectDamageSpell>("Магическая стрела", cost, range, damage);
    }
    else
    {
        int damage = damageDist(rng) / 2 + 5;
        int range = std::max(1, rangeDist(rng) - 1);
        int cost = costDist(rng) + 1;
        return std::make_unique<AreaDamageSpell>("Огненный шар", cost, range, damage, 2);
    }
}