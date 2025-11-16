#include "GameController.h"
#include "DirectDamageSpell.h"
#include "AreaDamageSpell.h"
#include <iostream>

GameController::GameController(int width, int height) 
    : gameField(std::make_unique<GameGrid>(width, height)), turnCounter(0) {}

void GameController::displayGameState() const
{
    std::cout << "Ход: " << turnCounter 
              << " | Здоровье: " << gameField->getPlayer()->getHitPoints() 
              << " | Очки: " << gameField->getPlayer()->getPoints() 
              << " | Режим: " << (gameField->getPlayer()->getCurrentMode() == BattleMode::CLOSE_COMBAT ? "Ближний" : "Дальний")
              << " | Урон: " << gameField->getPlayer()->getCurrentDamage()
              << " | Радиус: " << gameField->getPlayer()->getAttackRange()
              << " | Может двигаться: " << (gameField->getPlayer()->canMoveNextTurn() ? "Да" : "Нет") << "\n\n";
    int playerX, playerY;
    gameField->getPlayerPosition(playerX, playerY);
    auto& enemies = gameField->getEnemies();
    auto& enemyPositions = gameField->getEnemyPositions();
    for (int y = 0; y < gameField->getHeight(); y++)
    {
        for (int x = 0; x < gameField->getWidth(); x++)
        {
            if (gameField->hasPlayerAt(x, y))
            {
                std::cout << "P ";
            }
            else if (gameField->hasEnemyAt(x, y))
            {
                for (size_t i = 0; i < enemyPositions.size(); i++)
                {
                    if (enemyPositions[i].first == x && enemyPositions[i].second == y && enemies[i]->isActive())
                    {
                        int health = enemies[i]->getHitPoints();
                        if (health >= 20) std::cout << "E ";
                        else if (health >= 10) std::cout << "e ";
                        else std::cout << "e-";
                        break;
                    }
                }
            }
            else if (gameField->hasStructureAt(x, y))
            {
                std::cout << "B ";
            }
            else
            {
                switch (gameField->getCellTerrain(x, y))
                {
                    case TerrainType::OBSTACLE: std::cout << "# "; break;
                    case TerrainType::SLOWING: std::cout << "~ "; break;
                    case TerrainType::CLEAR: std::cout << ". "; break;
                }
            }
        }
        std::cout << "\n";
    }
    std::cout << "\nЗдоровье врагов: ";
    bool first = true;
    for (size_t i = 0; i < enemyPositions.size(); i++)
    {
        if (enemies[i]->isActive())
        {
            if (!first) std::cout << ", ";
            int x = enemyPositions[i].first;
            int y = enemyPositions[i].second;
            std::cout << "(" << x << "," << y << "):" << enemies[i]->getHitPoints() << "HP";
            first = false;
        }
    }
    if (first)
    {
        std::cout << "нет активных врагов";
    }
    std::cout << "\n\n";
}

bool GameController::handlePlayerAction(char input)
{
    auto* player = gameField->getPlayer();
    if (!player->canMoveNextTurn())
    {
        std::cout << "Игрок не может двигаться в этот ход!\n";
        if (input == 'f' || input == 'F')
        {
            player->toggleBattleMode();
            std::cout << "Переключен на "
                    << (player->getCurrentMode() == BattleMode::CLOSE_COMBAT ? "ближний" : "дальний")
                    << " бой\n";
            player->restoreMovement();
            return true;
        }
        else if (input == 'c' || input == 'C')
        {
            player->displaySpells();
            if (player->getSpellHand().getSpellCount() > 0)
            {
                std::cout << "Введите индекс заклинания и координаты цели (индекс x y): ";
                int spellIndex, targetX, targetY;
                if (!(std::cin >> spellIndex >> targetX >> targetY))
                {
                    std::cout << "Ошибка ввода! Введите числа.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    player->restoreMovement();
                    return true;
                }
                
                if (player->castSpell(spellIndex, *gameField, targetX, targetY))
                {
                    player->restoreMovement();
                    return true;
                }
                else
                {
                    std::cout << "Заклинание не сработало!\n";
                    player->restoreMovement();
                    return true;
                }
            }
            else
            {
                std::cout << "Нет доступных заклинаний!\n";
                player->restoreMovement();
            }
            return true;
        }
        else if (input == 'b' || input == 'B')
        {
            displaySpellShop();
            player->restoreMovement();
            return true;
        }
        else if (input == 'w' || input == 'W' || input == 's' || input == 'S' ||
                input == 'a' || input == 'A' || input == 'd' || input == 'D')
        {
            std::cout << "Движение заблокировано эффектом замедления!\n";
            player->restoreMovement();
            return true;
        }
        else if (input == 'h' || input == 'H')
        {
            displayEnemyHealth();
            player->restoreMovement();
            return true;
        }
        else
        {
            std::cout << "Неверная команда!\n";
            return true;
        }
    }
    int currentX, currentY;
    gameField->getPlayerPosition(currentX, currentY);
    int newX = currentX, newY = currentY;
    switch (input)
    {
        case 'w':
        case 'W':
            newY--;
            break;
        case 's':
        case 'S':
            newY++;
            break;
        case 'a':
        case 'A':
            newX--;
            break;
        case 'd':
        case 'D':
            newX++;
            break;
        case 'f':
        case 'F':
            player->toggleBattleMode();
            std::cout << "Переключен на "
                    << (player->getCurrentMode() == BattleMode::CLOSE_COMBAT ? "ближний" : "дальний")
                    << " бой\n";
            return true;
        case 'c':
        case 'C':
            player->displaySpells();
            if (player->getSpellHand().getSpellCount() > 0) {
                std::cout << "Введите индекс заклинания и координаты цели (индекс x y): ";
                int spellIndex, targetX, targetY;
                if (!(std::cin >> spellIndex >> targetX >> targetY))
                {
                    std::cout << "Ошибка ввода! Введите числа.\n";
                    std::cin.clear();
                    std::cin.ignore(10000, '\n');
                    return true;
                }
                if (player->castSpell(spellIndex, *gameField, targetX, targetY))
                {
                    return true;
                }
                else
                {
                    std::cout << "Заклинание не сработало! Ход не потрачен.\n";
                    return false; // Ход не засчитывается
                }
            }
            else
            {
                std::cout << "Нет доступных заклинаний!\n";
            }
            return true;
        case 'b':
        case 'B':
            displaySpellShop();
            return true;
        case 'h':
        case 'H':
            displayEnemyHealth();
            return true;
        default:
            std::cout << "Неверная команда!\n";
            return true;
    }
    
    if (gameField->movePlayer(newX, newY))
    {
        if (gameField->getCellTerrain(newX, newY) == TerrainType::SLOWING)
        {
            player->setMovementAbility(false);
            std::cout << "Игрок наступил на замедляющую клетку и не может двигаться в следующий ход!\n";
        }
        return true;
    }
    else
    {
        std::cout << "Нельзя переместиться туда!\n";
        return false;
    }
}

void GameController::displayEnemyHealth()
{
    auto& enemies = gameField->getEnemies();
    auto& enemyPositions = gameField->getEnemyPositions();
    std::cout << "\n=== ПОДРОБНОЕ ЗДОРОВЬЕ ВРАГОВ ===\n";
    bool foundEnemies = false;
    for (size_t i = 0; i < enemyPositions.size(); i++)
    {
        if (enemies[i]->isActive())
        {
            int x = enemyPositions[i].first;
            int y = enemyPositions[i].second;
            std::cout << "Враг на (" << x << "," << y << "): " 
                      << enemies[i]->getHitPoints() << "/" 
                      << enemies[i]->getMaxHitPoints() << " HP\n";
            foundEnemies = true;
        }
    }
    if (!foundEnemies)
    {
        std::cout << "Активных врагов нет\n";
    }
    std::cout << "================================\n\n";
}

void GameController::displaySpellShop()
{
    auto* player = gameField->getPlayer();
    std::cout << "\n=== МАГАЗИН ЗАКЛИНАНИЙ (Ваши очки: " << player->getPoints() << ") ===\n";
    std::cout << "1. Магическая стрела - 50 очков\n";
    std::cout << "   Прямое заклинание урона, радиус 3, урон 25\n";
    std::cout << "2. Огненный шар - 75 очков\n";
    std::cout << "   Заклинание урона по площади, радиус 2, урон 20 в области 2x2\n"; // Урон увеличен
    std::cout << "3. Ледяная стрела - 60 очков\n";
    std::cout << "   Прямое заклинание, радиус 3, урон 20\n";
    std::cout << "4. Удар молнии - 80 очков\n";
    std::cout << "   Заклинание урона по площади, радиус 4, урон 18 в области 3x3\n";
    std::cout << "5. Вернуться в игру\n";
    std::cout << "Выберите заклинание для покупки (1-5): ";
    int choice;
    if (!(std::cin >> choice))
    {
        std::cout << "Ошибка ввода! Введите число.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        return;
    }
    if (choice >= 1 && choice <= 4)
    {
        buySpellFromShop(choice);
    }
    else if (choice == 5)
    {
        std::cout << "Возвращаемся в игру...\n";
    }
    else
    {
        std::cout << "Неверный выбор!\n";
    }
}

void GameController::buySpellFromShop(int spellType)
{
    auto* player = gameField->getPlayer();
    std::unique_ptr<SpellCard> spell;
    int cost = 0;
    switch (spellType)
    {
        case 1:
            spell = std::make_unique<DirectDamageSpell>("Магическая стрела", 2, 3, 25);
            cost = 50;
            break;
        case 2:
            spell = std::make_unique<AreaDamageSpell>("Огненный шар", 3, 2, 20, 2); // Урон увеличен до 20
            cost = 75;
            break;
        case 3:
            spell = std::make_unique<DirectDamageSpell>("Ледяная стрела", 2, 3, 20);
            cost = 60;
            break;
        case 4:
            spell = std::make_unique<AreaDamageSpell>("Удар молнии", 4, 4, 18, 3);
            cost = 80;
            break;
    }
    if (player->getPoints() >= cost)
    {
        if (player->buySpell(std::move(spell)))
        {
            player->addPoints(-cost);
            std::cout << "Заклинание успешно куплено! Осталось очков: " << player->getPoints() << "\n";
        }
    }
    else
    {
        std::cout << "Недостаточно очков! Нужно " << cost << ", но у вас " << player->getPoints() << "\n";
    }
}

void GameController::handleEnemyActions()
{
    gameField->moveEnemies();
    gameField->generateEnemiesFromStructures();
    size_t enemiesBefore = gameField->getEnemyCount();
    gameField->removeDefeatedEnemies();
    size_t enemiesAfter = gameField->getEnemyCount();
    if (enemiesAfter < enemiesBefore)
    {
        gameField->getPlayer()->onEnemyDefeated();
    }
}

void GameController::startGame()
{
    std::cout << "Игра началась! Управление:\n";
    std::cout << "WASD - Движение, F - Смена режима боя, C - Использовать заклинания\n";
    std::cout << "B - Купить заклинания, H - Показать здоровье врагов, Q - Выйти\n\n";
    while (gameField->getPlayer()->isActive())
    {
        displayGameState();
        char userInput;
        std::cout << "Введите команду: ";
        std::cin >> userInput;
        if (userInput == 'q' || userInput == 'Q')
        {
            std::cout << "Игра завершена игроком.\n";
            break;
        }
        if (handlePlayerAction(userInput))
        {
            turnCounter++;
            handleEnemyActions();
        }
        if (!gameField->getPlayer()->isActive())
        {
            std::cout << "Конец игры! Игрок побежден.\n";
            break;
        }
        if (gameField->getEnemyCount() == 0)
        {
            std::cout << "Победа! Все враги уничтожены!\n";
            break;
        }
    }
    std::cout << "Финальный счет: " << gameField->getPlayer()->getPoints() << "\n";
}