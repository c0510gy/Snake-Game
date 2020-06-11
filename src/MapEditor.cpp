#include "Snake-Game/MapEditor.h"
#include <iostream>

MapEditor::MapEditor(const MapItem& map) {
    initializeWindow(map);
    initializeColors();
}

MapEditor::~MapEditor() {
    nodelay(stdscr, false);
    getch();
}

void MapEditor::edit(const MapItem& map, std::string filePath) {
    drawMap(map);
    int y, x;
    while (1)
    {
        int direction = 0;
        curs_set(1);

        move(y + WINDOW_OFFSET, x + WINDOW_OFFSET);
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
                break;
            case '1':
                break;
            case '2':
                break;
            case 's':
                direction = 4;
                break;

            default:
                break;
        }

        move(y, x);

        //refresh를 invoke 해줘야 ncurses가 화면에 그려줌
        refresh();
        if(direction == 4) {
            // 종료됨
            printw("Game Over");
            refresh();
            endwin();
            exit(1);
            break;
        }
        // 딜레이 안주면 게임이 너무 빨리 진행됨.
        // 프레임 밀리는 현상이 이 함수 관련한거 같음
        // 아마 다른 방식으로 스레드에 락 주는 방식을 사용해야 할 것 같음
        usleep(300000);
    }
}

void MapEditor::drawMap(const MapItem& map) {
    const MapManager objectMap = map.gameMap;

    for (int y = 0; y < objectMap.height; ++y)
    {
        for (int x = 0; x < objectMap.width; ++x)
        {
            // 커서 위치를 x,y로 이동 시킴.
            // ncurses를 활용해 글을 쓰려면 다음 순서를 거쳐야함
            // 1. 커서 이동(move) 2. write(addch)
            move(y + WINDOW_OFFSET, x + WINDOW_OFFSET);
            switch (objectMap.get(x, y))
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
}

void MapEditor::initializeWindow(const MapItem& map) {
    nodelay(stdscr,TRUE); // 입력 대기 없이(continuous 하게 게임 진행)
    
    getmaxyx(stdscr, maxHeight, maxWidth);

    const MapManager objectMap = map.gameMap;
    gameMapHeight = objectMap.height;
    gameMapWidth = objectMap.width;
}

void MapEditor::validateWindow(const MapItem& map) {
    
    const MapManager objectMap = map.gameMap;
    int requiredHeight = objectMap.height + WINDOW_OFFSET + SCORE_BOARD_HEIGHT + GOAL_BOARD_HEIGHT + 10;
    int requiredWidth = objectMap.width + WINDOW_OFFSET + SCORE_BOARD_WIDTH + SCORE_BOARD_HEIGHT + 10;
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
    init_pair(2, COLOR_GREEN, -1); // snake
    init_pair(3, COLOR_WHITE, COLOR_MAGENTA); // gate
}
