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

    initscr(); // ncurses 시작
    noecho(); // 커서 blink 없이
    curs_set(0);
    nodelay(stdscr,TRUE); // 입력 대기 없이(continuous 하게 게임 진행)
    keypad(stdscr, true); // 화살표 입력 받기 위해 

    // 색 사용하기 위해
    start_color();
    use_default_colors();

    if (has_colors() == FALSE) {
        // 색 지원 안할때 처리
        endwin();
        printf("Your terminal does not support color\n");
        exit(1);
    }

    // define colors
    init_pair(1, COLOR_WHITE, COLOR_WHITE); // wall
    init_pair(2, COLOR_GREEN, -1); // snake
    init_pair(3, COLOR_WHITE, COLOR_MAGENTA); // gate


    // 화면 크기가 충분하지 않을때 처리
    int maxHeight, maxWidth;
    getmaxyx(stdscr, maxHeight, maxWidth);
    if (nowMap.height > maxHeight || nowMap.width > maxWidth)
    {
        cout << nowMap.height << " " << nowMap.width << endl;
        printf("exit");
        exit(1);
    }

    int direction = 0;

    while (1)
    {
        //1. 입력 받고
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

        printw("%d",direction);
        // 2. 맵 받아오고
        const MapManager nowMap = myGame.getMap();

        // 3. 그려주고
        for (int y = 0; y < nowMap.height; ++y)
        {
            for (int x = 0; x < nowMap.width; ++x)
            {
                // 커서 위치를 x,y로 이동 시킴.
                // ncurses를 활용해 글을 쓰려면 다음 순서를 거쳐야함
                // 1. 커서 이동(move) 2. write(addch)
                move(x, y); 
                switch (nowMap.get(x, y))
                {
                case EMPTY:
                    addch(' ');
                    break;
                case WALL:
                case IMWALL:
                    // attron : 색 활성화
                    attron(COLOR_PAIR(1));
                    // addch : 현재 위치에 (위에서 활성화한 색을 가진) 문자 출력
                    addch(' ');
                    // attroff : 색 비활성화
                    attroff(COLOR_PAIR(1));
                    break;
                case SNAKE:
                    attron(COLOR_PAIR(2));
                    addch('@');
                    attroff(COLOR_PAIR(2));
                    break;
                case GROWTH:
                    addch('+');
                    break;
                case POISON:
                    addch('-');
                    break;
                case GATE:
                    attron(COLOR_PAIR(3));
                    addch('O');
                    attroff(COLOR_PAIR(3));
                    break;
                default:
                    addch('?');
                }
            }
        }
        //refresh를 invoke 해줘야 ncurses가 화면에 그려줌
        refresh();
        if(!myGame.nextFrame(direction)) {
            // 종료됨
            printw("breaked");
            refresh();
            break;
        }
        // 딜레이 안주면 게임이 너무 빨리 진행됨.
        // 프레임 밀리는 현상이 이 함수 관련한거 같음
        // 아마 다른 방식으로 스레드에 락 주는 방식을 사용해야 할 것 같음
        usleep(500000);
    }

    // 아래는 프로그램 종료 로직
    // 이런식으로 종료 안하면 콘솔이 망가짐.
    nodelay(stdscr, false);
	getch();
    endwin();

    return 0;
}