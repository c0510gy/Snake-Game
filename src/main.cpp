#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>
#include <unistd.h>
#include "Snake-Game/Snake-Game.h"
#include "GameManager.h"

using namespace std;

int main(int argc, char *argv[])
{
    FileManager test;
    MapItem items = test.readMap("./testMap2.txt");
    UserItem users = test.readUser("hi");

    GameRunner myGame(items.gameMap, items.startPoint);
    GameManager gameManager = GameManager(myGame);

    gameManager.play();

    return 0;
}