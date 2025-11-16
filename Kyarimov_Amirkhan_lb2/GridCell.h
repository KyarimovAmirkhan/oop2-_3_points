#ifndef GRID_CELL_H
#define GRID_CELL_H

enum class TerrainType
{
    CLEAR,
    OBSTACLE,
    SLOWING
};

class GridCell
{
private:
    TerrainType terrain;
    bool hasPlayer;
    bool hasEnemy;
    bool hasStructure;
    
public:
    GridCell();
    
    TerrainType getTerrain() const;
    bool isPassable() const;
    bool containsPlayer() const;
    bool containsEnemy() const;
    bool containsStructure() const;
    
    void setTerrain(TerrainType type);
    void setPlayerPresence(bool present);
    void setEnemyPresence(bool present);
    void setStructurePresence(bool present);
};

#endif