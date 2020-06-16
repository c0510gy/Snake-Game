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

void GlobalStateManager::startGame(std::string mapDir, std::string userName) {
    FileManager fileManager;
    MapItem items = fileManager.readMap(mapDir);
    UserItem users = fileManager.readUser(userName);

    GameRunner myGame(items);
    PlayerGameManager gameManager(myGame);

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

void GlobalStateManager::startgameWithAi() {
    refresh();
    std::vector<int> eachHiddenLayer = {20, 12};
    std::vector<long double> modelWeights;

    // parse model weight from file

    std::string gene;
    std::ifstream inFile = std::ifstream("./ai8.txt");
    inFile >> gene;
    inFile.close();

    std::vector<long double> ret;
    std::vector<std::string> splitted;
    std::string tmp = "";
    for(int j = 0; j < gene.size(); ++j){
        if(gene[j] == ','){
            splitted.push_back(tmp);
            tmp = "";
            continue;
        }
        tmp += gene[j];
    }
    for(int j = 0; j < splitted.size(); ++j)
        ret.push_back(stod(splitted[j]));
    modelWeights = ret;


    // get map for both player and ai
    FileManager fileManager;
    MapItem items = fileManager.readMap("./tutorial1.txt");

    SnakeAI aiGameRunner(eachHiddenLayer, modelWeights, items);
    GameRunner playerGameRunner(items);

    AiGameManager gameManager(playerGameRunner, aiGameRunner);

    gameManager.play();

}

void GlobalStateManager::startMapEditor() {
    refresh();

    FileManager test;
    MapItem items = test.readMap("./tutorial1.txt"); // rename
    UserItem users = test.readUser("hi");
    MapEditor editor(items);

    editor.edit(items, "s");
}