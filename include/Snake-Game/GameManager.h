/* 
 * title: GameManager.h
 * author: 구형모 - chrisais9@naver.com
 */

#ifndef SNAKE_GAME_GAME_MANAGER
#define SNAKE_GAME_GAME_MANAGER

#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>
#include <unistd.h>
#include "Score.h"
#include "Point.h"
#include "Snake-Game.h"
#include "MapItem.h"

#define WINDOW_OFFSET 3

#define SCORE_BOARD_WIDTH 15
#define SCORE_BOARD_HEIGHT 10

#define GOAL_BOARD_WIDTH 15
#define GOAL_BOARD_HEIGHT 10


class GameManager {
public:
    GameManager(GameRunner gameRunner, MapItem mapItem);
    ~GameManager();
    void play();
private:
    int maxHeight, maxWidth;
    int gameMapHeight, gameMapWidth;

    int initDirection; // 처음 어느 방향부터 시작해야하나

    GameRunner mGameRunner;

    WINDOW *windowScoreBoard;
    WINDOW *windowGoalBoard;

    void initializeWindow();
    void validateWindow();
    void initializeColors();

    void initializeScoreBoard();
    void initializeGoalBoard();

    void updateScoreStatus(const Score& score);
    void updateMissionStatus(const Mission& mission);
};

#endif
