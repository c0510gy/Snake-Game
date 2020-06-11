#include "Snake-Game/GlobalStateManager.h"

GlobalStateManager::GlobalStateManager() {
    initscr(); // ncurses 시작
    cbreak();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
}

void GlobalStateManager::run() {
    MenuManager menuManager = MenuManager();
    menuManager.showMenu();
    menuManager.destroyMenu();
}

void GlobalStateManager::startGame() {
    refresh();
    FileManager test;
    MapItem items = test.readMap("./testMap2.txt");
    UserItem users = test.readUser("hi");

    GameRunner myGame(items);
    GameManager gameManager = GameManager(myGame);

    gameManager.play();
}