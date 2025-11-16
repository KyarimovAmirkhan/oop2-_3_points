#include "EnemyStructure.h"

EnemyStructure::EnemyStructure(int interval) 
    : spawnTimer(0), spawnInterval(interval) {}

bool EnemyStructure::shouldSpawnEnemy()
{
    spawnTimer++;
    if (spawnTimer >= spawnInterval)
    {
        spawnTimer = 0;
        return true;
    }
    return false;
}

void EnemyStructure::resetTimer()
{
    spawnTimer = 0;
}

int EnemyStructure::getSpawnInterval() const
{
    return spawnInterval;
}

int EnemyStructure::getSpawnCounter() const
{
    return spawnTimer;
}