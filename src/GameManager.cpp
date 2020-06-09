#include "Snake-Game/GameManager.h"

GameManager::GameManager(GameRunner gameRunner): mGameRunner(gameRunner) {
    initializeWindow();
    initializeColors();
    initializeScoreBoard();
}

GameManager::~GameManager() {
    nodelay(stdscr, false);
    getch();
    delwin(windowScoreBoard);
    endwin();
}

void GameManager::play() {

    int direction = 0;
    while (1)
    {
        const MapManager mMapManager = mGameRunner.getMap();
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
        
        // 3. 그려주고
        for (int y = 0; y < mMapManager.height; ++y)
        {
            for (int x = 0; x < mMapManager.width; ++x)
            {
                // 커서 위치를 x,y로 이동 시킴.
                // ncurses를 활용해 글을 쓰려면 다음 순서를 거쳐야함
                // 1. 커서 이동(move) 2. write(addch)
                move(x + WINDOW_OFFSET, y + WINDOW_OFFSET); 
                switch (mMapManager.get(x, y))
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
        if(!mGameRunner.nextFrame(direction)) {
            // 종료됨
            printw("Game Over");
            refresh();
            break;
        }
        // 딜레이 안주면 게임이 너무 빨리 진행됨.
        // 프레임 밀리는 현상이 이 함수 관련한거 같음
        // 아마 다른 방식으로 스레드에 락 주는 방식을 사용해야 할 것 같음
        usleep(500000);
    }
}

void GameManager::initializeWindow() {
    setlocale(LC_ALL, "");
    initscr(); // ncurses 시작
    noecho(); // 커서 blink 없이
    curs_set(0);
    nodelay(stdscr,TRUE); // 입력 대기 없이(continuous 하게 게임 진행)
    keypad(stdscr, true); // 화살표 입력 받기 위해 
    
    getmaxyx(stdscr, maxHeight, maxWidth);

    const MapManager mMapManager = mGameRunner.getMap();
    gameMapHeight = mMapManager.height;
    gameMapWidth = mMapManager.width;
}

bool GameManager::validateWindow() {
    
    const MapManager mMapManager = mGameRunner.getMap();
    if (mMapManager.height > maxHeight || mMapManager.width > maxWidth)
    {
        move((maxHeight-2)/2,(maxWidth-5)/2);
        printw("Window size should be bigger than %d X %d", maxHeight, maxWidth);
        endwin();
        exit(1);
    }
}

void GameManager::initializeColors() {
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

void GameManager::initializeScoreBoard() {

    refresh();
    int offsetx = gameMapWidth + SCORE_BOARD_WIDTH + 5;
    int offsety = 3;

    windowScoreBoard = newwin(SCORE_BOARD_HEIGHT, SCORE_BOARD_WIDTH, offsety, offsetx);

    // box(windowScoreBoard, 0, 0);
    wborder(windowScoreBoard, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(windowScoreBoard, 0, 2, "Scoreboard");
    wrefresh(windowScoreBoard);
}