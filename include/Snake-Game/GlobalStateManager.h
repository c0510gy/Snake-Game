/* 
 * title: GlobalStateManager.h
 * author: 구형모 - chrisais9@naver.com
 */

#ifndef SNAKE_GAME_GLOBAL_STATE_MANAGER
#define SNAKE_GAME_GLOBAL_STATE_MANAGER

#include "Snake-Game.h"
#include "MenuManager.h"
#include "GameManager.h"

class GlobalStateManager {

public:
    GlobalStateManager();

    void run();

    void startGame();
    void startSetting();
    void startSaveGame();
    void startMapEditor();
};

#endif