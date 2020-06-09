/* 
 * title: GameManager.h
 * 
 */

#ifndef SNAKE_GAME_GAME_MANAGER
#define SNAKE_GAME_GAME_MANAGER

#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>
#include <unistd.h>
#include "Snake-Game.h"

class GameManager {
public:
    GameManager(GameRunner gameRunner);
    ~GameManager();
    void play();
private:
    int maxHeight, maxWidth;
    GameRunner mGameRunner;

    void initializeWindow();
    bool validateWindow();
    
    void initializeColors();
};

#endif
