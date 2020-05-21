/* 
 * title: Snake-Game.h
 * 
 */

#ifndef SNAKE_SNAKE_GAME
#define SNAKE_SNAKE_GAME

#include "Primitives/Item.h"
#include "Primitives/Point.h"
#include "Primitives/MapManager.h"
#include <algorithm>
#include <vector>
#include <deque>
#include <queue>
#include <set>
#include <cstdlib>
#include <ctime>

const Point DIRECTIONS[4] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

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

class Snake{
private:
    int length = 3;
    int direction = 0;
    int portalRemaining = 0;
    std::deque<Point> snakeBody;
    std::queue<Point> eraseQueue, insertQueue;
    std::set<Point> snakePoints;
    
    Point getNewHead();
    void pushFront(Point body);
    void popBack();
    bool checkValidity(MapManager& gameMap);

    void generalMove(Point newH);
    void growthMove(Point newH);
    void poisonMove(Point newH);
public:
    Snake(){}
    Snake(MapManager& gameMap, Point headPoint={0, 0}, int length=3, int direction=0);

    Item move(int direction, MapManager& gameMap, std::vector<Point>& portals);

    Point getHead();
    bool checkPoint(Point p);
    bool isInPortal();
};

class GameRunner{
private:
    MapManager gameMap;
    Snake snake;

    int frames = 0;

    const int ITEM_MAX_TIME = 10;
    const int MAX_NUMBER_OF_GROWTH = 3;
    const int MAX_NUMBER_OF_POISON = 3;

    RandomGenerator randomGenerator;

    int totalNumberOfItemCandidates = 0;
    std::set<IndexedPoint> itemsCandidates;
    std::vector<Point> portalCandidates;

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
    GameRunner(const MapManager& gameMap, Point startPoint, int length=3, int direction=0);

    bool nextFrame(int direction);
    const MapManager& getMap();
};

#endif