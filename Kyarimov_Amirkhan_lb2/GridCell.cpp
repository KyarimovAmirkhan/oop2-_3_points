#include "GridCell.h"

GridCell::GridCell() 
    : terrain(TerrainType::CLEAR), hasPlayer(false), 
      hasEnemy(false), hasStructure(false) {}

TerrainType GridCell::getTerrain() const
{
    return terrain;
}

bool GridCell::isPassable() const
{
    return terrain != TerrainType::OBSTACLE && !hasStructure;
}

bool GridCell::containsPlayer() const
{
    return hasPlayer;
}

bool GridCell::containsEnemy() const
{
    return hasEnemy;
}

bool GridCell::containsStructure() const
{
    return hasStructure;
}

void GridCell::setTerrain(TerrainType type)
{
    terrain = type;
}

void GridCell::setPlayerPresence(bool present)
{
    hasPlayer = present;
}

void GridCell::setEnemyPresence(bool present)
{
    hasEnemy = present;
}

void GridCell::setStructurePresence(bool present)
{
    hasStructure = present;
}