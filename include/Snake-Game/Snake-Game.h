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
#include <cstdlib>
#include <ctime>

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

struct IndexedPoint{
    int idx;
    Point p;
    bool operator<(const IndexedPoint& ip) const{
        return idx < ip.idx;
    }
    bool operator>(const IndexedPoint& ip) const{
        return idx > ip.idx;
    }
    bool operator==(const IndexedPoint& ip) const{
        return idx == ip.idx;
    }
};

class RandomGenerator{
private:
public:
    RandomGenerator(){
        srand((unsigned)time(NULL));
    }
    int getRandom(int s, int e){
        return rand() % (e - s + 1) + s;
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
    std::queue<Point> eraseQueue, insertQueue;
    std::set<Point> snakePoints;

    Point getNewHead();
    void pushFront(Point body);
    void popBack();
    bool checkValidity();
public:
    Snake(Point headPoint={0, 0}, int length=3, int direction=0);

    bool move();
    bool growthMove();
    bool poisonMove();
    bool portal(Point p);

    bool checkPoint(Point p);

    int getDirection();
    void setDirection(int direction);
};

class GameRunner{
private:
    std::vector<std::vector<Item>> gameMap;
    Snake snake;

    int frames = 0;

    const int ITEM_MAX_TIME = 10;
    const int MAX_NUMBER_OF_GROWTH = 3;
    const int MAX_NUMBER_OF_POISON = 3;

    RandomGenerator randomGenerator;

    int totalNumberOfItemCandidates;
    int totalNumberOfPortalCandidates;
    std::set<IndexedPoint> itemsCandidates;
    std::set<IndexedPoint> portalCandidates;

    std::queue<std::pair<IndexedPoint, int>> growthTimeQ, poisonTimeQ;
    std::multiset<Point> itemsUsed;
    int numberOfGrowth = 0;
    int numberOfPoison = 0;

    int portalTime = -1;
    std::vector<Point> portals;

    Point getRandomItemPoint(std::queue<std::pair<IndexedPoint, int>>& timeQ);

    void updateGrowth();
    void updatePoison();
    void updatePortal();
public:
    GameRunner(const std::vector<std::vector<Item>>& gameMap, Point startPoint, int length=3, int direction=0);

    bool nextFrame();
};

#endif