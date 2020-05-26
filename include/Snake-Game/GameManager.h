/* 
 * title: GameManager.h
 * 
 */

#ifndef SNAKE_GAME_GAME_MANAGER
#define SNAKE_GAME_GAME_MANAGER

#include <iostream>
#include <fstream>
#include <string>
#include <ncurses.h>
#include <unistd.h>
#include "Snake-Game.h"

class GameManager{

    int maxHeight, maxWidth;
    GameRunner mGameRunner;
    // (myMap, startPoint);
    const MapManager mMapManager;

    GameManager(GameRunner gameRunner): mGameRunner(gameRunner), mMapManager(gameRunner.getMap()) {
        initializeWindow();
    }

    void play() {
        int direction = 0;
        while (1)
        {
            const MapManager nowMap = mGameRunner.getMap();
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
            if(!mGameRunner.nextFrame(direction)) {
                printw("breaked");
                refresh();
                break;
            }
            usleep(500000);
        }
    }
    void initializeWindow() {
        initscr();
        noecho();
        curs_set(0);
        nodelay(stdscr,TRUE);
        keypad(stdscr, true);
    }

    bool validateWindow() {
        getmaxyx(stdscr, maxHeight, maxWidth);
        if (mMapManager.height > maxHeight || mMapManager.width > maxWidth)
            return false;
        return true;
    }
};

#endif