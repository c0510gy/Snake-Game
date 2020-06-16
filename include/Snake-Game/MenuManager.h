/* 
 * title: MenuManager.h
 * author: 구형모 - chrisais9@naver.com
 */

#ifndef SNAKE_GAME_MENU_MANAGER
#define SNAKE_GAME_MENU_MANAGER

#include <ncurses.h>
#include <cstdlib>
#include <vector>
#include <string>
#include "Snake-Game/GlobalStateManager.h"

#define MENU_WINDOW_WIDTH 15
#define MENU_WINDOW_HEIGHT 10

#define MODE_START_GAME 0
#define MODE_AI 1
#define MODE_CUSTOM_GAME 2
#define MODE_EDIT_MAP 3
#define MODE_EXIT 4



class MenuManager {
public:
    MenuManager();
    
    void showMenu();
    void destroyMenu();
private:
    int maxHeight, maxWidth;
    std::vector<std::string> modesName;

    WINDOW* menuWindow;
    void initializeMenu();
    void printMenu(int needTobeHighligted);
    void startMode(int mode);
    
};

#endif
