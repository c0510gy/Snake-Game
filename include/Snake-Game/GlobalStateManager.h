/* 
 * title: GlobalStateManager.h
 * author: 구형모 - chrisais9@naver.com
 */

#ifndef SNAKE_GAME_GLOBAL_STATE_MANAGER
#define SNAKE_GAME_GLOBAL_STATE_MANAGER

#include "Snake-Game.h"
#include "MenuManager.h"
#include "GameManager.h"
#include "MapEditor.h"

class GlobalStateManager {
private:
    WINDOW* windowIntro; 
    void startGame(std::string mapDir, std::string userName);

public:
    GlobalStateManager();

    void run();
    
    void startGame();
    bool showMapIntro(std::string mapDir, std::string userName);

    void startCustomGame();

    void startSetting();

    void startMapEditor();
    

};

#endif