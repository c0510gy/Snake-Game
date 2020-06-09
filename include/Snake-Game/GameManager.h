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

    // index of score board
    int indexOfSnakeScoreCurrentLength[2];
    int indexOfPlusScore[2];
    int indexOfMinusScore[2];
    int indexOfGateScore[2];

    // index of score board
    int indexOfSnakeGoal[2];
    int indexOfPlusGoal[2];
    int indexOfMinusGoal[2];
    int indexOfGateGoal[2];

    GameRunner mGameRunner;

    WINDOW *windowScoreBoard;
    WINDOW *windowGoalBoard;

    void initializeWindow();
    bool validateWindow();
    void initializeColors();

    void initializeScoreBoard();
    void initializeGoalBoard();

    void updateScoreStatus();
    void updateMissionStatus(const Mission& mission);
};

#endif
