#include "Snake-Game/GameManager.h"

GameManager::GameManager(GameRunner gameRunner): mGameRunner(gameRunner) {
    initializeWindow();
    initializeColors();
    initializeScoreBoard();
    initializeGoalBoard();
}

GameManager::~GameManager() {
    nodelay(stdscr, false);
    getch();
    delwin(windowScoreBoard);
    delwin(windowGoalBoard);
    endwin();
}

void GameManager::play() {

    int direction = 1;
    while (1)
    {
        const MapManager mMapManager = mGameRunner.getMap();
        //1. 입력 받고
        int input = getch();
        switch (input)
        {
            case KEY_LEFT:
                direction = 0;
                break;

            case KEY_DOWN:
                direction = 3;
                break;

            case KEY_RIGHT:
                direction = 2;
                break;

            case KEY_UP:
                direction = 1;
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
                move(y + WINDOW_OFFSET, x + WINDOW_OFFSET);
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
        const StatusManager& mStatusManager = mGameRunner.getStatus();
        updateScoreStatus(mStatusManager.getScore());
        updateMissionStatus(mStatusManager.getMission());
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

void GameManager::validateWindow() {
    
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
    int offsetx = gameMapWidth + WINDOW_OFFSET + SCORE_BOARD_WIDTH + 5;
    int offsety = WINDOW_OFFSET;

    windowScoreBoard = newwin(SCORE_BOARD_HEIGHT, SCORE_BOARD_WIDTH, offsety, offsetx);

    // box(windowScoreBoard, 0, 0);
    wborder(windowScoreBoard, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(windowScoreBoard, 0, 5, "Score");

    mvwprintw(windowScoreBoard, 2, 1, "B: 3 / 10");
    mvwprintw(windowScoreBoard, 3, 1, "+: 0");
    mvwprintw(windowScoreBoard, 4, 1, "-: 0");
    mvwprintw(windowScoreBoard, 5, 1, "G: 0");

    wrefresh(windowScoreBoard);
}


void GameManager::updateScoreStatus(const Score& score) {
    mvwprintw(windowScoreBoard, 2, 1, "B: %d / %d", score.scoreBody, score.MAX_SCORE_BODY);
    mvwprintw(windowScoreBoard, 3, 1, "+: %d", score.scoreGrowth);
    mvwprintw(windowScoreBoard, 4, 1, "-: %d", score.scorePoison);
    mvwprintw(windowScoreBoard, 5, 1, "G: 0", score.scoreGate);

    wrefresh(windowScoreBoard);
}

void GameManager::initializeGoalBoard() {

    refresh();
    int offsetx = gameMapWidth + WINDOW_OFFSET + GOAL_BOARD_WIDTH + 5;
    int offsety = WINDOW_OFFSET + GOAL_BOARD_HEIGHT + 1;

    windowGoalBoard = newwin(GOAL_BOARD_HEIGHT, GOAL_BOARD_WIDTH, offsety, offsetx);

    wborder(windowGoalBoard, '|', '|', '-', '-', '+', '+', '+', '+');
    mvwprintw(windowGoalBoard, 0, 5 , "Goal");
    mvwprintw(windowGoalBoard, 2, 1, "B: %d (%c)",10,' ');
    mvwprintw(windowGoalBoard, 3, 1, "+: %d (%c)",7,' ');
    mvwprintw(windowGoalBoard, 4, 1, "-: %d (%c)",5,'O');
    mvwprintw(windowGoalBoard, 5, 1, "G: %d (%c)",3 ,' ');

    wrefresh(windowGoalBoard);
}

void GameManager::updateMissionStatus(const Mission& mission) {
    mvwprintw(windowGoalBoard, 2, 1, "B: %d (%c)", mission.goalBody, (mission.achBody ? 'O' : ' '));
    mvwprintw(windowGoalBoard, 3, 1, "+: %d (%c)", mission.goalGrowth, (mission.achGrowth ? 'O' : ' '));
    mvwprintw(windowGoalBoard, 4, 1, "-: %d (%c)", mission.goalPoison, (mission.achPoison ? 'O' : ' '));
    mvwprintw(windowGoalBoard, 5, 1, "G: %d (%c)", mission.goalGate, (mission.achGate ? 'O' : ' '));

    wrefresh(windowGoalBoard);

}

