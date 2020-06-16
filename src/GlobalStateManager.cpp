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
    if(!showMapIntro("./tutorial1.txt", "hi")) {
        run();
    }

    // 2-nd map
    if(!showMapIntro("./tutorial2.txt", "hi")) {
        run();
    }
    // 3-rd map
    if(!showMapIntro("./tutorial3.txt", "hi")) {
        run();
    }
    // 4-th map
    if(!showMapIntro("./tutorial4.txt", "hi")) {
        run();
    }

}
bool GlobalStateManager::showMapIntro(std::string mapDir, std::string userName) {
    refresh();
    FileManager fileManager;
    MapItem items = fileManager.readMap(mapDir);

    int maxWidth, maxHeight;

    getmaxyx(stdscr, maxHeight, maxWidth);
    int offsetx = (maxWidth - 50) / 2;
    int offsety = (maxHeight - 8) / 2;

    windowIntro = newwin(8, 50, offsety, offsetx);

    std::string message;
    message = "name : " + items.name;
    mvwprintw(windowIntro, 2, 1, "%s %s", message.c_str());

    message = "author : " + items.name;
    mvwprintw(windowIntro, 3, 1, "%s", message.c_str());

    message = "detail : " + items.name;
    mvwprintw(windowIntro, 4, 1, "%s", message.c_str());

    message = "Press any key to continue or 'q' to quit";
    mvwprintw(windowIntro, 6, 1, "%s", message.c_str());

    wborder(windowIntro, '|', '|', '-', '-', '+', '+', '+', '+');

    wrefresh(windowIntro);
    
    int c = getch();
    
    wclear(windowIntro);
    wrefresh(windowIntro);
    delwin(windowIntro);

    if(c == 'q' || c == 'Q') {
        return 0;
    }

    startGame(mapDir, userName);
    return 1;
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

void GlobalStateManager::startMapEditor() {
    refresh();
    FileManager test;
    MapItem items;
    UserItem users = test.readUser("hi");

    MapEditor editor(items);
    editor.showInputWindow();
    editor.edit();
}