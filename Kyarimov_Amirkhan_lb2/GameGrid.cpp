#include "GameGrid.h"
#include <random>
#include <iostream>
#include <ctime>

GameGrid::GameGrid(int width, int height) 
    : gridWidth(width), gridHeight(height)
    {
    if (gridWidth < 10)
    {
        std::cout << "Предупреждение: Ширина слишком мала, устанавливаем минимум 10\n";
        gridWidth = 10;
    }
    else if (gridWidth > 25)
    {
        std::cout << "Предупреждение: Ширина слишком велика, устанавливаем максимум 25\n";
        gridWidth = 25;
    }
    if (gridHeight < 10)
    {
        std::cout << "Предупреждение: Высота слишком мала, устанавливаем минимум 10\n";
        gridHeight = 10;
    }
    else if (gridHeight > 25)
    {
        std::cout << "Предупреждение: Высота слишком велика, устанавливаем максимум 25\n";
        gridHeight = 25;
    }
    std::cout << "Создано поле размером: " << gridWidth << "x" << gridHeight << "\n";
    player = std::make_unique<PlayerCharacter>();
    initializeGrid();
}

GameGrid::GameGrid(const GameGrid& other) 
    : gridWidth(other.gridWidth), gridHeight(other.gridHeight),
      cells(other.cells)
      {
    if (other.player)
    {
        int x, y;
        other.player->getPosition(x, y);
        player = std::make_unique<PlayerCharacter>(x, y, 
            other.player->getHitPoints(), 
            other.player->getCurrentDamage(), 10);
    }
    for (const auto& enemy : other.enemies)
    {
        int x, y;
        enemy->getPosition(x, y);
        enemies.push_back(std::make_unique<EnemyCharacter>(x, y, 
            enemy->getHitPoints(), enemy->getAttackPower()));
        enemyPositions.push_back({x, y});
    }
    for (size_t i = 0; i < other.structures.size(); i++)
    {
        structures.push_back(std::make_unique<EnemyStructure>(
            other.structures[i]->getSpawnInterval()));
        structurePositions.push_back(other.structurePositions[i]);
    }
    }

GameGrid::GameGrid(GameGrid&& other) noexcept
    : gridWidth(other.gridWidth), gridHeight(other.gridHeight),
      cells(std::move(other.cells)),
      player(std::move(other.player)),
      enemies(std::move(other.enemies)),
      enemyPositions(std::move(other.enemyPositions)),
      structures(std::move(other.structures)),
      structurePositions(std::move(other.structurePositions))
      {
    
    other.gridWidth = 0;
    other.gridHeight = 0;
    }

GameGrid& GameGrid::operator=(const GameGrid& other)
{
    if (this != &other)
    {
        gridWidth = other.gridWidth;
        gridHeight = other.gridHeight;
        cells = other.cells;
        player.reset();
        if (other.player)
        {
            int x, y;
            other.player->getPosition(x, y);
            player = std::make_unique<PlayerCharacter>(x, y, 
                other.player->getHitPoints(), 
                other.player->getCurrentDamage(), 10);
        }
        enemies.clear();
        enemyPositions.clear();
        for (const auto& enemy : other.enemies)
        {
            int x, y;
            enemy->getPosition(x, y);
            enemies.push_back(std::make_unique<EnemyCharacter>(x, y, 
                enemy->getHitPoints(), enemy->getAttackPower()));
            enemyPositions.push_back({x, y});
        }
        structures.clear();
        structurePositions.clear();
        for (size_t i = 0; i < other.structures.size(); i++)
        {
            structures.push_back(std::make_unique<EnemyStructure>(
                other.structures[i]->getSpawnInterval()));
            structurePositions.push_back(other.structurePositions[i]);
        }
    }
    return *this;
}

GameGrid& GameGrid::operator=(GameGrid&& other) noexcept
{
    if (this != &other) {
        gridWidth = other.gridWidth;
        gridHeight = other.gridHeight;
        cells = std::move(other.cells);
        player = std::move(other.player);
        enemies = std::move(other.enemies);
        enemyPositions = std::move(other.enemyPositions);
        structures = std::move(other.structures);
        structurePositions = std::move(other.structurePositions);
        other.gridWidth = 0;
        other.gridHeight = 0;
    }
    return *this;
}

void GameGrid::initializeGrid()
{
    cells.resize(gridHeight, std::vector<GridCell>(gridWidth));
    std::mt19937 rng(std::time(nullptr));
    std::uniform_real_distribution<float> dist(0.0f, 1.0f);
    for (int y = 0; y < gridHeight; y++)
    {
        for (int x = 0; x < gridWidth; x++)
        {
            float randomValue = dist(rng);
            if (randomValue < 0.1f)
            {
                cells[y][x].setTerrain(TerrainType::OBSTACLE);
            }
            else if (randomValue < 0.2f)
            {
                cells[y][x].setTerrain(TerrainType::SLOWING);
            }
        }
    }
    cells[0][0].setPlayerPresence(true);
    player->setPosition(0, 0);
    generateRandomEnemies(3);
    generateRandomStructures(2);
}

void GameGrid::generateRandomEnemies(int count)
{
    std::mt19937 rng(std::time(nullptr));
    std::uniform_int_distribution<int> xDist(0, gridWidth - 1);
    std::uniform_int_distribution<int> yDist(0, gridHeight - 1);
    int enemiesCreated = 0;
    int attempts = 0;
    const int maxAttempts = 100;
    while (enemiesCreated < count && attempts < maxAttempts)
    {
        int x = xDist(rng);
        int y = yDist(rng);
        if (isCellPassable(x, y) && !hasPlayerAt(x, y) && !hasEnemyAt(x, y))
        {
            enemies.push_back(std::make_unique<EnemyCharacter>(x, y));
            enemyPositions.push_back({x, y});
            cells[y][x].setEnemyPresence(true);
            enemiesCreated++;
        }
        attempts++;
    }
    if (enemiesCreated < count)
    {
        std::cout << "Предупреждение: Создано только " << enemiesCreated << " из " << count << " врагов\n";
    }
}

void GameGrid::generateRandomStructures(int count)
{
    std::mt19937 rng(std::time(nullptr));
    std::uniform_int_distribution<int> xDist(0, gridWidth - 1);
    std::uniform_int_distribution<int> yDist(0, gridHeight - 1);
    std::uniform_int_distribution<int> intervalDist(8, 12);
    int structuresCreated = 0;
    int attempts = 0;
    const int maxAttempts = 100;
    while (structuresCreated < count && attempts < maxAttempts)
    {
        int x = xDist(rng);
        int y = yDist(rng);
        if (isCellPassable(x, y) && !hasPlayerAt(x, y) && !hasEnemyAt(x, y) && !hasStructureAt(x, y))
        {
            structures.push_back(std::make_unique<EnemyStructure>(intervalDist(rng)));
            structurePositions.push_back({x, y});
            cells[y][x].setStructurePresence(true);
            cells[y][x].setTerrain(TerrainType::OBSTACLE);
            structuresCreated++;
        }
        attempts++;
    }
    if (structuresCreated < count)
    {
        std::cout << "Предупреждение: Создано только " << structuresCreated << " из " << count << " зданий\n";
    }
}

bool GameGrid::movePlayer(int newX, int newY)
{
    if (!isPositionValid(newX, newY))
    {
        std::cout << "Позиция за пределами поля!\n";
        return false;
    }
    if (!isCellPassable(newX, newY))
    {
        std::cout << "Клетка непроходима!\n";
        return false;
    }
    if (hasEnemyAt(newX, newY))
    {
        return attackEnemyAt(newX, newY);
    }
    int oldX, oldY;
    player->getPosition(oldX, oldY);
    cells[oldY][oldX].setPlayerPresence(false);
    player->setPosition(newX, newY);
    cells[newY][newX].setPlayerPresence(true);
    return true;
}

bool GameGrid::attackEnemyAt(int x, int y)
{
    int playerX, playerY;
    player->getPosition(playerX, playerY);
    int distance = std::abs(playerX - x) + std::abs(playerY - y);
    int attackRange = player->getAttackRange();
    if (distance > attackRange)
    {
        std::cout << "Цель слишком далеко! Дистанция: " << distance << ", Радиус атаки: " << attackRange << "\n";
        return false;
    }
    if (!hasEnemyAt(x, y))
    {
        std::cout << "На этой клетке нет врага!\n";
        return false;
    }
    for (size_t i = 0; i < enemyPositions.size(); i++)
    {
        if (enemyPositions[i].first == x && enemyPositions[i].second == y && enemies[i]->isActive())
        {
            int damage = player->getCurrentDamage();
            enemies[i]->receiveDamage(damage);
            std::cout << "Игрок атакует врага и наносит " << damage << " урона! ";
            std::cout << "Здоровье врага: " << enemies[i]->getHitPoints() << "/" << enemies[i]->getMaxHitPoints() << "\n";
            if (!enemies[i]->isActive())
            {
                player->addPoints(10);
                std::cout << "Враг побежден! +10 очков\n";
            }
            return true;
        }
    }
    std::cout << "Враг на этой клетке уже побежден!\n";
    return false;
}

void GameGrid::moveEnemies()
{
    std::mt19937 rng(std::time(nullptr));
    std::uniform_int_distribution<int> dirDist(0, 3);
    for (size_t i = 0; i < enemyPositions.size(); i++)
    {
        if (!enemies[i]->isActive()) continue;
        int x = enemyPositions[i].first;
        int y = enemyPositions[i].second;
        int direction = dirDist(rng);
        int newX = x, newY = y;
        switch (direction)
        {
            case 0: newY--; break;
            case 1: newX++; break;
            case 2: newY++; break;
            case 3: newX--; break;
        }
        if (isPositionValid(newX, newY))
        {
            if (hasPlayerAt(newX, newY))
            {
                int damage = enemies[i]->getAttackPower();
                player->receiveDamage(damage);
                std::cout << "Враг атакует игрока и наносит " << damage << " урона! ";
                std::cout << "Здоровье игрока: " << player->getHitPoints() << "/" << player->getMaxHitPoints() << "\n";
            }
            else if (isCellPassable(newX, newY) && !hasEnemyAt(newX, newY) && !hasStructureAt(newX, newY))
            {
                cells[y][x].setEnemyPresence(false);
                cells[newY][newX].setEnemyPresence(true);
                enemyPositions[i] = {newX, newY};
                enemies[i]->setPosition(newX, newY);
            }
        }
    }
}

void GameGrid::generateEnemiesFromStructures()
{
    for (size_t i = 0; i < structurePositions.size(); i++)
    {
        if (structures[i]->shouldSpawnEnemy())
        {
            int bx = structurePositions[i].first;
            int by = structurePositions[i].second;
            std::vector<std::pair<int, int>> possiblePositions;
            for (int dx = -1; dx <= 1; dx++)
            {
                for (int dy = -1; dy <= 1; dy++)
                {
                    if (dx == 0 && dy == 0) continue;
                    int nx = bx + dx;
                    int ny = by + dy;
                    if (isPositionValid(nx, ny) && isCellPassable(nx, ny) && 
                        !hasEnemyAt(nx, ny) && !hasPlayerAt(nx, ny))
                    {
                        possiblePositions.emplace_back(nx, ny);
                    }
                }
            }
            if (!possiblePositions.empty())
            {
                std::mt19937 rng(std::time(nullptr));
                std::uniform_int_distribution<int> posDist(0, possiblePositions.size() - 1);
                auto pos = possiblePositions[posDist(rng)];
                enemies.push_back(std::make_unique<EnemyCharacter>(pos.first, pos.second));
                enemyPositions.push_back(pos);
                cells[pos.second][pos.first].setEnemyPresence(true);
                std::cout << "Появился новый враг из здания!\n";
            }
        }
    }
}

void GameGrid::removeDefeatedEnemies()
{
    for (size_t i = 0; i < enemies.size(); )
    {
        if (!enemies[i]->isActive())
        {
            auto pos = enemyPositions[i];
            cells[pos.second][pos.first].setEnemyPresence(false);
            enemyPositions.erase(enemyPositions.begin() + i);
            enemies.erase(enemies.begin() + i);
        }
        else
        {
            i++;
        }
    }
}

void GameGrid::getPlayerPosition(int& x, int& y) const
{
    player->getPosition(x, y);
}

bool GameGrid::isPositionValid(int x, int y) const
{
    return x >= 0 && x < gridWidth && y >= 0 && y < gridHeight;
}

bool GameGrid::isCellPassable(int x, int y) const
{
    return isPositionValid(x, y) && cells[y][x].isPassable();
}

bool GameGrid::hasPlayerAt(int x, int y) const
{
    return isPositionValid(x, y) && cells[y][x].containsPlayer();
}

bool GameGrid::hasEnemyAt(int x, int y) const
{
    return isPositionValid(x, y) && cells[y][x].containsEnemy();
}

bool GameGrid::hasStructureAt(int x, int y) const
{
    return isPositionValid(x, y) && cells[y][x].containsStructure();
}

TerrainType GameGrid::getCellTerrain(int x, int y) const
{
    return isPositionValid(x, y) ? cells[y][x].getTerrain() : TerrainType::OBSTACLE;
}

int GameGrid::getWidth() const
{
    return gridWidth;
}

int GameGrid::getHeight() const
{
    return gridHeight;
}

size_t GameGrid::getEnemyCount() const
{
    return enemies.size();
}

PlayerCharacter* GameGrid::getPlayer() const
{
    return player.get();
}

const std::vector<std::unique_ptr<EnemyCharacter>>& GameGrid::getEnemies() const
{
    return enemies;
}

const std::vector<std::pair<int, int>>& GameGrid::getEnemyPositions() const
{
    return enemyPositions;
}