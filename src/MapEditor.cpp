#include "Snake-Game/MapEditor.h"
#include <iostream>

MapEditor::MapEditor(MapItem& map) : userMapItem(map){
    initializeWindow();
    initializeColors();
}

MapEditor::~MapEditor() {
    nodelay(stdscr, false);
    getch();
}

void MapEditor::edit() {
    initSetMap();

    MapManager objMap = userMapItem.gameMap;
    int y, x;
    initDrawMap();
    move(WINDOW_OFFSET, WINDOW_OFFSET);
    while (1)
    {
        bool saveFlag = false;
        curs_set(1);
        getyx(stdscr, y, x);

        int input = getch();
        switch (input)
        {
            case KEY_LEFT:
                x--;
                break;
            case KEY_UP:
                y--;
                break;
            case KEY_RIGHT:
                x++;
                break;
            case KEY_DOWN:
                y++;
                break;    

            case '0':
                addch(' ');
                objMap.set(x - WINDOW_OFFSET, y - WINDOW_OFFSET, EMPTY);
                break;
            case '1':
                attron(COLOR_PAIR(1));
                addch(' ');
                attroff(COLOR_PAIR(1));
                objMap.set(x - WINDOW_OFFSET, y - WINDOW_OFFSET, WALL);
                break;
            case '2':
                attron(COLOR_PAIR(2));
                addch(' ');
                attroff(COLOR_PAIR(2));
                objMap.set(x - WINDOW_OFFSET, y - WINDOW_OFFSET, IMWALL);
                break;
            case 's':
                saveFlag = true;
                break;

            default:
                break;
        }

        move(y, x);

        //refresh를 invoke 해줘야 ncurses가 화면에 그려줌
        refresh();
        if(saveFlag) {
            userMapItem.gameMap = objMap;

            autoSetWall();
            FileManager test;
            test.writeMap(userMapItem, "./userchange" + userMapItem.name + ".txt");
            // 종료
            printw("Save!");
            refresh();
            endwin();
            exit(1);
            break;
        }
        // // 딜레이 안주면 게임이 너무 빨리 진행됨.
        // // 프레임 밀리는 현상이 이 함수 관련한거 같음
        // // 아마 다른 방식으로 스레드에 락 주는 방식을 사용해야 할 것 같음
        usleep(300000);
    }
}
void MapEditor::autoSetWall() {
    int dir[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};
    MapManager objMap = userMapItem.gameMap;

    for (int y = 0; y < objMap.height; ++y)
    {
        for (int x = 0; x < objMap.width; ++x)
        {
            if(objMap.get(x, y) == WALL){
                int tmpWallCnt = 0;
                for(int z = 0; z < 4; ++z){
                    int nx = x + dir[z][0];
                    int ny = y + dir[z][1];
                    if(objMap.get(nx, ny) != EMPTY)
                        tmpWallCnt++;
                }
                if(tmpWallCnt == 4)
                    objMap.set(x, y, IMWALL);
            }
        }
    }
    userMapItem.gameMap = objMap;
}

// 사용자에게 입력받은 WH에 맞는 Map 테두리 wall을 만들어 줍니다.
void MapEditor::initSetMap() {
    MapManager objMap = userMapItem.gameMap;
    for (int y = 0; y < objMap.height; ++y)
    {
        for (int x = 0; x < objMap.width; ++x)
        {
            if(y == 0 || y == objMap.height-1)
                objMap.set(x, y, WALL);
            else if(x == 0 || x == objMap.width-1)
                objMap.set(x, y, WALL);
        }
    }
    userMapItem.gameMap = objMap;
}

void MapEditor::initDrawMap() {
    MapManager objMap = userMapItem.gameMap;
    for (int y = 0; y < objMap.height; ++y)
    {
        for (int x = 0; x < objMap.width; ++x)
        {
            move(y + WINDOW_OFFSET, x + WINDOW_OFFSET);
            switch (objMap.get(x, y))
            {
                case EMPTY:
                    addch(' ');
                    break;
                case WALL:
                case IMWALL:
                    attron(COLOR_PAIR(1));
                    addch(' ');
                    attroff(COLOR_PAIR(1));
                    break;
                default:
                    addch('?');
            }
        }
    }
}

void MapEditor::initializeWindow() {
    nodelay(stdscr,TRUE); // 입력 대기 없이(continuous 하게 게임 진행)
    
    getmaxyx(stdscr, maxHeight, maxWidth);

    const MapManager objMap = userMapItem.gameMap;
    gameMapHeight = objMap.height;
    gameMapWidth = objMap.width;
}

void MapEditor::validateWindow() {
    const MapManager objMap = userMapItem.gameMap;
    int requiredHeight = objMap.height + WINDOW_OFFSET + 10;
    int requiredWidth = objMap.width + WINDOW_OFFSET + 10;
    if (requiredHeight > maxHeight || requiredWidth > maxWidth) {
        move((maxHeight-2)/2,(maxWidth-5)/2);
        printw("Window size should be bigger than %d X %d", requiredHeight, requiredWidth);
        endwin();
        exit(1);
    }
}

void MapEditor::initializeColors() {
    // 색 사용하기 위해
    start_color();
    use_default_colors();

    if (has_colors() == FALSE) {
        // 색 지원 안할때 처리
        printw("Your terminal does not support color\n");
        refresh();
        endwin();
        exit(1);
    }

    // define colors
    init_pair(1, COLOR_WHITE, COLOR_WHITE); // wall
    init_pair(2, COLOR_GREEN, COLOR_GREEN);
    init_pair(3, COLOR_WHITE, COLOR_MAGENTA); // gate
}

// 최초 input 윈도우를 출력하는 함수
void MapEditor::showInputWindow() {
    nodelay(stdscr, false);
    refresh();
    int offsetx = (maxWidth - (maxWidth / 2)) / 2;
    int offsety = (maxHeight -(maxHeight / 2)) / 2;

    windowInfoInput = newwin(maxHeight / 4, maxWidth / 2, offsety, offsetx);
    wborder(windowInfoInput, '|', '|', '-', '-', '+', '+', '+', '+');

    std::string message = "Please enter the basic properties for creating a new map. \n";
    std::string name_key = "Name = ";
    std::string name = "";
    std::string width_key = "Width = ";
    std::string width = "";
    std::string height_key = "Height = ";
    std::string height = "";

    mvwprintw(windowInfoInput, 1, 10, "%s", message.c_str());
    mvwprintw(windowInfoInput, 2, 10, "%s", name_key.c_str());
    mvwprintw(windowInfoInput, 3, 10, "%s", width_key.c_str());
    mvwprintw(windowInfoInput, 4, 10, "%s", height_key.c_str());

    mvwprintw(windowInfoInput, 6, 10, "%s", "You can move between items with the upper and lower keys.");
    mvwprintw(windowInfoInput, 7, 10, "%s", "Press Enter key to save.");
    wrefresh(windowInfoInput);

    curs_set(0);

    int tmp_idx = 2;
    while (1)
    {
        int c = getch();
        switch(tmp_idx){
            case(2):
                name += c;
                name_key += c;
                mvwprintw(windowInfoInput, 2, 10, "%s", name_key.c_str());
                wrefresh(windowInfoInput);
                break;
            case(3):
                width += c;
                width_key += c;
                mvwprintw(windowInfoInput, 3, 10, "%s", width_key.c_str());
                wrefresh(windowInfoInput);
                break;
            case(4):
                height += c;
                height_key += c;
                mvwprintw(windowInfoInput, 4, 10, "%s", height_key.c_str());
                wrefresh(windowInfoInput);
                break;
        } 
        
        if(c == '\n' && tmp_idx < 4) {
            ++tmp_idx;
        }
        else if(c == '\n' && tmp_idx == 4){
            break; 
        }
    }

    MapManager tmpinit(atoi(width.c_str()), atoi(height.c_str()));
    userMapItem.name = name;
    userMapItem.gameMap = tmpinit;

    refresh();
    endwin();

    wclear(windowInfoInput);
    wrefresh(windowInfoInput);
    delwin(windowInfoInput);
    nodelay(stdscr, true);
    getch();
}
