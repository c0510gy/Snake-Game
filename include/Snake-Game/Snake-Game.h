/* 
 * title: Snake-Game.h
 * 
 */

#ifndef SNAKE_GAME
#define SNAKE_GAME

#include <vector>
#include <deque>
#include <queue>
#include <set>

struct Point{
    int x, y;
    Point(int x=0, int y=0){
        this->x = x;
        this->y = y;
    }
    Point& operator+=(const Point& p){
        x += p.x;
        y += p.y;
        return *this;
    }
    bool operator<(const Point& p) const{
        if(x == p.x)
            return y < p.y;
        return x < p.x;
    }
    bool operator>(const Point& p) const{
        if(x == p.x)
            return y > p.y;
        return x > p.x;
    }
    bool operator==(const Point& p) const{
        return x == p.x && y == p.y;
    }
};

enum Item{
    EMPTY,
    WALL,
    IMWAll,
    GATE,
    GROWTH,
    POISON,
    SANKE
};

Point DIR[4] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

class Snake{
private:
    int length = 3;
    int direction = 0;
    std::deque<Point> snakeBody;
public:
    Snake(Point headPoint={0, 0}, int length=3, int direction=0);

    bool move();
    void growth();
    void poison();
    void portal(int x, int y);

    int getDirection();
    void setDirection(int direction);
};

class GameRunner{
private:
    std::vector<std::vector<Item>> gameMap;
    Snake snake;

    int frams = 0;

    const int ITEM_MAX_TIME = 10;
    const int MAX_NUMBER_OF_GROWTH = 3;
    const int MAX_NUMBER_OF_POISON = 3;

    std::vector<Point> itemsCandidates;
    std::vector<Point> portalCandidates;

    std::queue<std::pair<Point, int>> growthTimeQ, poisonTimeQ;
    std::multiset<Point> itemsUsed;
    int numberOfGrowth = 0;
    int numberOfPoison = 0;

    int portalTime = -1;
    std::vector<Point> portals;

    void updateGrowth();
    void updatePoison();
    void updatePortal();
public:
    GameRunner(const std::vector<std::vector<Item>>& gameMap);

    bool nextFrame();
};

#endif