#include "Snake-Game/AiGameManager.h"

AiGameManager::AiGameManager(GameRunner gameRunner, SnakeAI aiGameRunner) : mPlayerGameRunner(gameRunner), mAiGameRunner(aiGameRunner)
{
    initializeWindow();
    initializeColors();
}

AiGameManager::~AiGameManager()
{
    nodelay(stdscr, false);
}

void AiGameManager::initializeWindow()
{
    nodelay(stdscr, true); // 입력 대기 없이(continuous 하게 게임 진행)

    getmaxyx(stdscr, maxHeight, maxWidth);

    const MapManager mMapManager = mPlayerGameRunner.getMap();
    gameMapHeight = mMapManager.height;
    gameMapWidth = mMapManager.width;
}

void AiGameManager::initializeColors()
{
    // 색 사용하기 위해
    start_color();
    use_default_colors();

    if (has_colors() == FALSE)
    {
        // 색 지원 안할때 처리

        printw("Your terminal does not support color\n");
        refresh();
        endwin();
        exit(1);
    }

    // define colors
    init_pair(1, COLOR_WHITE, COLOR_WHITE);   // wall
    init_pair(2, COLOR_GREEN, -1);            // snake
    init_pair(3, COLOR_WHITE, COLOR_MAGENTA); // gate
}

void AiGameManager::play()
{
    int needToGameOverPlayer = 0;
    int needToGameOverAi = 0;

    while (1)
    {
        const MapManager mPlayerMapManager = mPlayerGameRunner.getMap();
        const MapManager mAiMapManager = mAiGameRunner.getMap();

        //1. 입력 받고
        int direction = mPlayerGameRunner.getDirection();
        int input = getch();
        switch (input)
        {
        case KEY_LEFT:
            direction = 0;
            break;

        case KEY_UP:
            direction = 1;
            break;

        case KEY_RIGHT:
            direction = 2;
            break;

        case KEY_DOWN:
            direction = 3;
            break;

        case 'q':
        case 'Q':
        case 'p':
        case 'P':
            showPauseWindow();
            break;

        default:
            break;
        }

        needToGameOverPlayer = !mPlayerGameRunner.nextFrame(direction);
        needToGameOverAi = !mAiGameRunner.nextFrame();

        // 3. 그려주고
        for (int y = 0; y < mPlayerMapManager.height; ++y)
        {
            for (int x = 0; x < mPlayerMapManager.width; ++x)
            {
                // 커서 위치를 x,y로 이동 시킴.
                // ncurses를 활용해 글을 쓰려면 다음 순서를 거쳐야함
                // 1. 커서 이동(move) 2. write(addch)
                move(y + WINDOW_OFFSET, x + WINDOW_OFFSET);
                switch (mPlayerMapManager.get(x, y))
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

        for (int y = 0; y < mAiMapManager.height; ++y)
        {
            for (int x = 0; x < mAiMapManager.width; ++x)
            {
                // 커서 위치를 x,y로 이동 시킴.
                // ncurses를 활용해 글을 쓰려면 다음 순서를 거쳐야함
                // 1. 커서 이동(move) 2. write(addch)
                move(y + WINDOW_OFFSET, x + WINDOW_OFFSET + mPlayerMapManager.width + WINDOW_OFFSET);
                switch (mAiMapManager.get(x, y))
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

        if (needToGameOverPlayer)
        {
            printw("AI WIN");
            refresh();
            endwin();
            exit(1);
            break;
        }

        if (needToGameOverAi)
        {
            printw("YOU WIN");
            refresh();
            endwin();
            exit(1);
            break;
        }

        //refresh를 invoke 해줘야 ncurses가 화면에 그려줌
        refresh();

        // 딜레이 안주면 게임이 너무 빨리 진행됨.
        // 프레임 밀리는 현상이 이 함수 관련한거 같음
        // 아마 다른 방식으로 스레드에 락 주는 방식을 사용해야 할 것 같음
        usleep(300000);
    }
}

void AiGameManager::showPauseWindow()
{

    nodelay(stdscr, false);
    refresh();
    int offsetx = (maxWidth - (maxWidth / 2)) / 2;
    int offsety = (maxHeight - (maxHeight / 2)) / 2;

    windowPause = newwin(maxHeight / 4, maxWidth / 2, offsety, offsetx);

    std::string message = "Game Paused. Press 'any key' to Continue or 'Q' to quit";
    mvwprintw(windowPause, 2, 1, "%s", message.c_str());

    wborder(windowPause, '|', '|', '-', '-', '+', '+', '+', '+');

    wrefresh(windowPause);

    int c = getch();
    if (c == 'q' || c == 'Q')
    {
        refresh();
        endwin();
        exit(1);
    }
    wclear(windowPause);

    wrefresh(windowPause);

    delwin(windowPause);
    nodelay(stdscr, true);
    getch();
}