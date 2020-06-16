/* 
 * title: AiGameManager.h
 * author: 구형모 - chrisais9@naver.com
 */

#ifndef SNAKE_GAME_AI_GAME_MANAGER
#define SNAKE_GAME_AI_GAME_MANAGER

#include <ncurses.h>
#include <unistd.h>
#include <string>
#include "Snake-Game.h"
#include "GA-MLP/Snake-AI.h"

#define WINDOW_OFFSET 3

class AiGameManager {
public:
    AiGameManager(GameRunner gameRunner, SnakeAI aiGameRunner);
    ~AiGameManager();
    void play();
private:
    int maxHeight, maxWidth;
    int gameMapHeight, gameMapWidth;

    GameRunner mPlayerGameRunner;
    SnakeAI mAiGameRunner;

    WINDOW* windowPause;

    void initializeWindow();

    void initializeColors();

    void showMapIntro();
    void showPauseWindow();
};

#endif
