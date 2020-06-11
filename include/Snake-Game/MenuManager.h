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

#define MENU_WINDOW_WIDTH 15
#define MENU_WINDOW_HEIGHT 20

#define MODE_START_GAME 0
#define MODE_SETTING 1
#define LOAD_MAP 2
#define EDIT_MAP 3


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
    
};

#endif
