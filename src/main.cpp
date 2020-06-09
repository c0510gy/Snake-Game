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
    ifstream inFile;
    inFile.open("./map.txt");
    Point startPoint;
    int width, height;
    inFile >> width >> height;
    MapManager myMap(width, height);
    inFile >> startPoint.x >> startPoint.y;
    for (int y = 0; y < myMap.height; ++y)
    {
        string str;
        inFile >> str;
        for (int x = 0; x < myMap.width; ++x)
        {
            int v = str[x] - '0';
            switch (v)
            {
            case 0:
                myMap.set(x, y, EMPTY);
                break;
            case 1:
                myMap.set(x, y, WALL);
                break;
            case 2:
                myMap.set(x, y, IMWALL);
                break;
            }
        }
    }
    cout << width << " " << height << endl;


    GameRunner myGame(myMap, startPoint);
    GameManager gameManger = GameManager(myGame);
    gameManger.play();

    return 0;
}