#ifndef GAME_GRID_H
#define GAME_GRID_H

#include <vector>
#include <memory>
#include "GridCell.h"
#include "PlayerCharacter.h"
#include "EnemyCharacter.h"
#include "EnemyStructure.h"

class GameGrid
{
private:
    int gridWidth, gridHeight;
    std::vector<std::vector<GridCell>> cells;
    
    std::unique_ptr<PlayerCharacter> player;
    std::vector<std::unique_ptr<EnemyCharacter>> enemies;
    std::vector<std::pair<int, int>> enemyPositions;
    std::vector<std::unique_ptr<EnemyStructure>> structures;
    std::vector<std::pair<int, int>> structurePositions;
    
    void initializeGrid();
    void generateRandomEnemies(int count);
    void generateRandomStructures(int count);
    
public:
    GameGrid(int width, int height);
    GameGrid(const GameGrid& other);
    GameGrid(GameGrid&& other) noexcept;
    
    GameGrid& operator=(const GameGrid& other);
    GameGrid& operator=(GameGrid&& other) noexcept;
    
    bool movePlayer(int newX, int newY);
    bool attackEnemyAt(int x, int y);
    void getPlayerPosition(int& x, int& y) const;
    
    void moveEnemies();
    void generateEnemiesFromStructures();
    void removeDefeatedEnemies();
    
    bool isPositionValid(int x, int y) const;
    bool isCellPassable(int x, int y) const;
    bool hasPlayerAt(int x, int y) const;
    bool hasEnemyAt(int x, int y) const;
    bool hasStructureAt(int x, int y) const;
    TerrainType getCellTerrain(int x, int y) const;
    
    int getWidth() const;
    int getHeight() const;
    size_t getEnemyCount() const;
    PlayerCharacter* getPlayer() const;
    
    const std::vector<std::unique_ptr<EnemyCharacter>>& getEnemies() const;
    const std::vector<std::pair<int, int>>& getEnemyPositions() const;
};

#endif