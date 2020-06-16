/* 
 * title: GlobalStateManager.h
 * author: 구형모 - chrisais9@naver.com
 */

#ifndef SNAKE_GAME_GLOBAL_STATE_MANAGER
#define SNAKE_GAME_GLOBAL_STATE_MANAGER

#include "Snake-Game.h"
#include "MenuManager.h"
#include "PlayerGameManager.h"
#include "AiGameManager.h"
#include "MapEditor.h"
#include <iostream>

class GlobalStateManager {
private:
    WINDOW* windowIntro; 

public:
    GlobalStateManager();

    void run();
    
    void startGame(std::string mapDir, std::string userName);
    void startGame();
    bool showMapIntro(std::string mapDir, std::string userName);

    void startSetting();

    void startMapEditor();

    void startgameWithAi();
    
};

#endif