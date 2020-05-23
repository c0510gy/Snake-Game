#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>
#include <unistd.h>
#include "Snake-Game/Snake-Game.h"
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

    // need to be refactored as class from below
    GameRunner myGame(myMap, startPoint);
    const MapManager nowMap = myGame.getMap();

    initscr();
    noecho();
    curs_set(0);
    nodelay(stdscr,true);
    keypad(stdscr, true);

    int maxHeight, maxWidth;
    getmaxyx(stdscr, maxHeight, maxWidth);
    if (nowMap.height > maxHeight || nowMap.width > maxWidth)
    {
        cout << nowMap.height << " " << nowMap.width << endl;
        printf("exit");
        return 0;
    }

    while (1)
    {
        const MapManager nowMap = myGame.getMap();
        for (int y = 0; y < nowMap.height; ++y)
        {
            for (int x = 0; x < nowMap.width; ++x)
            {
                move(x, y);
                switch (nowMap.get(x, y))
                {
                case EMPTY:
                    addch(' ');
                    break;
                case WALL:
                case IMWALL:
                    addch('#');
                    break;
                case SNAKE:
                    addch('@');
                    break;
                case GROWTH:
                    addch('+');
                    break;
                case POISON:
                    addch('-');
                    break;
                case GATE:
                    addch('O');
                    break;
                default:
                    addch('?');
                }
            }
        }
        int direction = 0;
        int input = getch();
        switch (input)
        {
            case KEY_LEFT:
                direction = 1;
                break;

            case KEY_DOWN:
                direction = 2;
                break;

            case KEY_RIGHT:
                direction = 3;
                break;

            case KEY_UP:
                direction = 0;
                break;

            default:
                break;
        }

        refresh();
        printw("%d",direction);
        if(!myGame.nextFrame(direction)) {
            printw("breaked");
            refresh();
            break;
        }
    }

    nodelay(stdscr, false);
	getch();
    endwin();

    return 0;
}