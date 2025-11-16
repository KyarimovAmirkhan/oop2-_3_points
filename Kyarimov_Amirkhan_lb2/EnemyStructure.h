#ifndef ENEMY_STRUCTURE_H
#define ENEMY_STRUCTURE_H

class EnemyStructure
{
private:
    int spawnTimer;
    int spawnInterval;
    
public:
    EnemyStructure(int interval = 10);
    
    bool shouldSpawnEnemy();
    void resetTimer();
    
    int getSpawnInterval() const;
    int getSpawnCounter() const;
};

#endif