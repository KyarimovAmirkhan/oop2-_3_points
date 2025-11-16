#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "GameGrid.h"
#include <memory>

class GameController
{
private:
    std::unique_ptr<GameGrid> gameField;
    int turnCounter;
    
    void displayGameState() const;
    bool handlePlayerAction(char input);
    void handleEnemyActions();
    void displaySpellShop();
    void buySpellFromShop(int spellType);
    void displayEnemyHealth();
    
public:
    GameController(int width, int height);
    void startGame();
};

#endif