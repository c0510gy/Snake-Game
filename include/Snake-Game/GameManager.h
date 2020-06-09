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

#define SCORE_BOARD_WIDTH 15
#define SCORE_BOARD_HEIGHT 10

#define WINDOW_OFFSET 3

class GameManager {
public:
    GameManager(GameRunner gameRunner);
    ~GameManager();
    void play();
private:
    int maxHeight, maxWidth;
    int gameMapHeight, gameMapWidth;
    GameRunner mGameRunner;

    WINDOW *windowScoreBoard;

    void initializeWindow();
    bool validateWindow();
    void initializeColors();

    void initializeScoreBoard();
};

#endif
