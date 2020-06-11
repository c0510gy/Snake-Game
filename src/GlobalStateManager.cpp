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

void GlobalStateManager::startGame(std::string mapDir, std::string userName) {
    FileManager fileManager;
    MapItem items = fileManager.readMap(mapDir);
    UserItem users = fileManager.readUser(userName);

    GameRunner myGame(items);
    GameManager gameManager = GameManager(myGame);

    gameManager.play();
}

void GlobalStateManager::startGame() {
    refresh();
    
    // 1-st map
    startGame("./tutorial1.txt", "hi");
    // // 2-nd map
    // startGame("./tutorial2.txt", "hi");
    // // 3-rd map
    // startGame("./tutorial3.txt", "hi");
    // // 4-th map
    // startGame("./tutorial4.txt", "hi");

}

void GlobalStateManager::startCustomGame() {
    refresh();

    FileManager test;
    MapItem items = test.readMap("./testMap2.txt"); // rename
    UserItem users = test.readUser("hi");

    GameRunner myGame(items);
    GameManager gameManager = GameManager(myGame);

    gameManager.play();
}