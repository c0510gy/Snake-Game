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

#define WINDOW_OFFSET 3

#define SCORE_BOARD_WIDTH 15
#define SCORE_BOARD_HEIGHT 10

#define GOAL_BOARD_WIDTH 15
#define GOAL_BOARD_HEIGHT 10


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
    WINDOW *windowGoalBoard;

    void initializeWindow();
    bool validateWindow();
    void initializeColors();

    void initializeScoreBoard();
    void initializeGoalBoard();
};

#endif
