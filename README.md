# Snake-Game
Team project: Snake Game - 2020-1 C++ Programming

## 보고서

### 1 단계

* NCurses Library 함수들을 사용하여 2 차원 배열로 된 Snake Map을 Game 화면으로 표시하는 프로그램을 완성한다. 그림의 세부사항은 각자 정한다.
    * Map은 21x21 을 최소 크기로 한다.
* 주의 : Wall 과 Immune Wall 을 잘 구분할 것

#### 1단계 구현 내용

```cpp
/* 
 * title: Primitives/Item.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SNAKE_ITEM
#define SNAKE_ITEM

enum Item{
    ERROR,
    EMPTY,
    WALL,
    IMWALL,
    GATE,
    GROWTH,
    POISON,
    SNAKE,
};

#endif
```

```cpp
/* 
 * title: Primitives/Point.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SNAKE_POINT
#define SNAKE_POINT

struct Point{
    int x, y;
    Point(int x=0, int y=0){
        this->x = x;
        this->y = y;
    }
    bool isValid(int width, int height) const{
        return x >= 0 && y >= 0 && x < width && y < height;
    }
    Point& operator+=(const Point& p){
        x += p.x;
        y += p.y;
        return *this;
    }
    Point operator+(const Point& p) const{
        Point ret = *this;
        ret += p;
        return ret;
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

#endif
```

```cpp
/* 
 * title: Primitives/MapManager.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SNAKE_MAP_MANAGER
#define SNAKE_MAP_MANAGER

#include "Primitives/Item.h"
#include "Primitives/Point.h"
#include <vector>

struct MapManager{
    int width, height;
    std::vector<std::vector<Item>> gameMap;

    MapManager(int width=0, int height=0){
        this->width = width;
        this->height = height;
        gameMap.resize(height);
        for(int y = 0; y < height; ++y)
            gameMap[y].resize(width, EMPTY);
    }
    Item get(int x, int y) const{
        if(x < 0 || y < 0 || x >= width || y >= height)
            return ERROR;
        return gameMap[y][x];
    }
    Item get(Point p) const{
        return get(p.x, p.y);
    }
    void set(int x, int y, Item v){
        if(x < 0 || y < 0 || x >= width || y >= height)
            return;
        gameMap[y][x] = v;
    }
    void set(Point p, Item v){
        set(p.x, p.y, v);
    }
};

#endif
```

```cpp
/* 
 * title: Primitives/MapItem.h
 * author: 
 */

#ifndef SNAKE_MAP_ITEM
#define SNAKE_MAP_ITEM

#include "Primitives/MapManager.h"
#include <string>
#include <vector>

struct MapItem{
    std::string name;
    std::string author;
    std::string date;
    std::string detail;

    int goalBody;
    int goalGrowth;
    int goalPoison;
    int goalGate;
    int MAX_SCORE_BODY;
    int startDirection;

    MapManager gameMap;
    Point startPoint;
};

#endif
```

### 2 단계

* 1단계의 맵 위에 Snake 를 표시하고, 화살표를 입력받아 Snake 가 움직이도록 프로그램을 완성한다.
* Snake는 규칙 #1을 준수해야 한다.

#### 2단계 구현 내용

```cpp
/* 
 * title: Snake-Game.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SNAKE_SNAKE_GAME
#define SNAKE_SNAKE_GAME

#include "Primitives/Item.h"
#include "Primitives/Point.h"
#include "Primitives/MapManager.h"
#include "Primitives/MapItem.h"
#include "FileManager.h"
#include <algorithm>
#include <vector>
#include <deque>
#include <queue>
#include <set>
#include <cstdlib>
#include <ctime>

const Point DIRECTIONS[4] = {{-1, 0}, {0, -1}, {1, 0}, {0, 1}};

class Snake{
private:
    int MAX_LENGTH = 10;                 // 현재 Snake의 최대 길이

    int length = 3;                            // 현재 Snake의 길이
    int direction = 0;                         // 현재 Snake의 이동 방향
    std::deque<Point> snakeBody;               // 현재 Snake의 몸통 좌표. Head부터 이웃한 순서대로 front에 위치
    std::queue<Point> eraseQueue, insertQueue; // 제거/추가될 몸통 좌표
    std::set<Point> snakePoints;               // 몸통 좌표 중복 체크
    
    Point getNewHead();                        // 이동 방향에 대한 새로운 좌표(head) 가져오기
    void pushFront(Point body);                // Head쪽에 새로운 몸통 (새로운 head) 추가
    void popBack();                            // 꼬리쪽의 몸통 제거
    bool checkValidity(MapManager& gameMap);   // Snake 스스로 몸통이 겹치는지 검증

    void generalMove(Point newH);              // 위치 newH로 일반적인 이동
public:
    Snake(){}
    Snake(MapManager& gameMap, Point headPoint={0, 0}, int length=3, int direction=0, int MAX_LENGTH=10);

    // direction방향으로 portals의 포탈을 가진 gameMap맵에서 이동연산 수행
    // 섭취한 아이템을 반환, 유효하지 않은 상황이 발생할 경우 ERROR 반환
    Item move(int direction, MapManager& gameMap);

    Point getHead();            // 현재 Snake의 머리(head)의 좌표를 반환
    bool checkPoint(Point p);   // 위치 p가 Snake가 존재하는 위치에 속하는지 여부를 반환
    int getDirection();
};

class GameRunner{
private:
    MapManager gameMap;                     // 현재 진행중인 게임의 맵 객체
    Snake snake;                            // 현재 진행중인 게임의 Snake 객체

    int frames = 0;                         // 현재 진행중인 게임의 frame 수
public:
    // gameMap의 맵을 가지고 Snake의 초기 위치가 startPoint이며 길이가 length이고 방향이 direction인 게임을 생성
    GameRunner(const MapItem& gameMapInfo, int length=3);

    bool nextFrame(int direction);          // Snake가 direction방향으로 이동하는 다음 프레임을 계산하고 game over여부를 반환
    const MapManager& getMap();             // 현재 진행중인 게임의 맵을 반환
    int getDirection();                     // 현재 Snake의 진행중인 방향 반환
};

#endif
```
```cpp
/* 
 * title: Snake-Game.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "Snake-Game/Snake-Game.h"

Snake::Snake(MapManager& gameMap, Point headPoint, int length, int direction, int MAX_LENGTH): MAX_LENGTH(MAX_LENGTH){
    this->length = length;
    this->direction = direction;
    for(int j = 0; j < length; ++j){
        snakeBody.push_back(headPoint);
        snakePoints.insert(headPoint);
        gameMap.set(headPoint, SNAKE);
        headPoint += DIRECTIONS[(direction + 2) % 4];
    }
}
Point Snake::getNewHead(){
    Point newH = snakeBody.front();
    newH += DIRECTIONS[direction];
    return newH;
}
void Snake::pushFront(Point body){
    insertQueue.push(body);
    snakeBody.push_front(body);
    ++length;
}
void Snake::popBack(){
    eraseQueue.push(snakeBody.back());
    snakeBody.pop_back();
    --length;
}
bool Snake::checkValidity(MapManager& gameMap){
    bool ret = true;
    while(!eraseQueue.empty()){
        snakePoints.erase(eraseQueue.front());
        gameMap.set(eraseQueue.front(), EMPTY);
        eraseQueue.pop();
    }
    while(!insertQueue.empty()){
        if(snakePoints.find(insertQueue.front()) != snakePoints.end())
            ret = false;
        snakePoints.insert(insertQueue.front());
        gameMap.set(insertQueue.front(), SNAKE);
        insertQueue.pop();
    }
    return ret;
}
Item Snake::move(int direction, MapManager& gameMap){
    this->direction = direction;
    Point newH = getNewHead();
    Item ret = EMPTY;
    switch(gameMap.get(newH)){
        case SNAKE:
        case EMPTY:
            generalMove(newH);
            break;
        case GROWTH:
            break;
        case POISON:
            break;
        case WALL:
            ret = ERROR;
            break;
        case GATE:
            break;
    }
    if(!checkValidity(gameMap))
        ret = ERROR;
    return ret;
}
void Snake::generalMove(Point newH){
    pushFront(newH);
    popBack();
}
Point Snake::getHead(){
    return snakeBody.front();
}
int Snake::getDirection(){
    return direction;
}

GameRunner::GameRunner(const MapItem& gameMapInfo, int length){
    this->gameMap = gameMapInfo.gameMap;
    snake = Snake(this->gameMap, gameMapInfo.startPoint, length, gameMapInfo.startDirection, gameMapInfo.MAX_SCORE_BODY);
}
bool GameRunner::nextFrame(int direction){
    switch(snake.move(direction, gameMap)){
        case ERROR:
            return false;
    }

    ++frames;
    return true;
}
const MapManager& GameRunner::getMap(){
    return gameMap;
}
int GameRunner::getDirection(){
    return snake.getDirection();
}
```

### 3 단계

* 2단계 프로그램에서, Map 위에 Growth Item과 Poison Item이 출현하도록 수정한다.
* 게임 규칙 #2를 준수해야 한다.
* Growth Item과 Poison Item을 Map 배열에 표현할 때 값을 정한다
    * 화면상에 표현시, 색이나 기호를 달리하여 구분할 수 있도록 한다
    * Map Data에서 Growth Item은 5, Poison Item은 6 과 같이 구분할 수 있도록 한다

#### 3단계 구현 내용

```cpp
/* 
 * title: Snake-Game.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SNAKE_SNAKE_GAME
#define SNAKE_SNAKE_GAME

#include "Primitives/Item.h"
#include "Primitives/Point.h"
#include "Primitives/MapManager.h"
#include "Primitives/MapItem.h"
#include "FileManager.h"
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
    // 범위 [s, e]에서 균등 분포로 랜덤한 값 하나를 반환
    int getRandom(int s, int e){
        return rand() % (e - s + 1) + s;
    }
};

class Snake{
private:
    int MAX_LENGTH = 10;                 // 현재 Snake의 최대 길이

    int length = 3;                            // 현재 Snake의 길이
    int direction = 0;                         // 현재 Snake의 이동 방향
    std::deque<Point> snakeBody;               // 현재 Snake의 몸통 좌표. Head부터 이웃한 순서대로 front에 위치
    std::queue<Point> eraseQueue, insertQueue; // 제거/추가될 몸통 좌표
    std::set<Point> snakePoints;               // 몸통 좌표 중복 체크
    
    Point getNewHead();                        // 이동 방향에 대한 새로운 좌표(head) 가져오기
    void pushFront(Point body);                // Head쪽에 새로운 몸통 (새로운 head) 추가
    void popBack();                            // 꼬리쪽의 몸통 제거
    bool checkValidity(MapManager& gameMap);   // Snake 스스로 몸통이 겹치는지 검증

    void generalMove(Point newH);              // 위치 newH로 일반적인 이동
    void growthMove(Point newH);               // 위치 newH로 성장 이동 - Growth 아이템 효과
    void poisonMove(Point newH);               // 위치 newH로 독 이동 - Poison 아이템 효과
public:
    Snake(){}
    Snake(MapManager& gameMap, Point headPoint={0, 0}, int length=3, int direction=0, int MAX_LENGTH=10);

    // direction방향으로 portals의 포탈을 가진 gameMap맵에서 이동연산 수행
    // 섭취한 아이템을 반환, 유효하지 않은 상황이 발생할 경우 ERROR 반환
    Item move(int direction, MapManager& gameMap);

    Point getHead();            // 현재 Snake의 머리(head)의 좌표를 반환
    bool checkPoint(Point p);   // 위치 p가 Snake가 존재하는 위치에 속하는지 여부를 반환
    int getDirection();
};

class GameRunner{
private:
    MapManager gameMap;                     // 현재 진행중인 게임의 맵 객체
    Snake snake;                            // 현재 진행중인 게임의 Snake 객체

    int frames = 0;                         // 현재 진행중인 게임의 frame 수

    const int ITEM_MAX_TIME = 100;           // 아이템이 사라지지 않는 시간
    const int MAX_NUMBER_OF_GROWTH = 3;     // Growth 아이템의 출현 최대 개수
    const int MAX_NUMBER_OF_POISON = 3;     // Poison 아이템의 출현 최대 개수

    RandomGenerator randomGenerator;        // 랜덤 정수 생성기

    int totalNumberOfItemCandidates = 0;    // 아이템이 나타날 수 있는 위치의 최대 개수
    std::set<IndexedPoint> itemsCandidates; // 아이템이 나타날 수 있는 위치

    std::queue<std::pair<IndexedPoint, int>> growthTimeQ, poisonTimeQ; // 존재 가능 시간을 초과한 아이템을 관리하기 위한 큐
    std::multiset<Point> itemsUsed;         // 현재 Snake에 의해 아이템이 사용된 좌표
    int numberOfGrowth = 0;                 // 현재 존재하는 Growth 아이템 개수
    int numberOfPoison = 0;                 // 현재 존재하는 Poison 아이템 개수

    // itemsCandidates에서 유요한 위치를 균등 랜덤으로 뽑아 반환
    // 최대 100회 시도를 통해 유효한 위치를 뽑는데 실패했다면 {-1, -1}을 반환
    Point getRandomItemPoint(std::queue<std::pair<IndexedPoint, int>>& timeQ);

    void updateGrowth();                    // Growth 아이템 업데이트
    void updatePoison();                    // Poison 아이템 업데이트
public:
    // gameMap의 맵을 가지고 Snake의 초기 위치가 startPoint이며 길이가 length이고 방향이 direction인 게임을 생성
    GameRunner(const MapItem& gameMapInfo, int length=3);

    bool nextFrame(int direction);          // Snake가 direction방향으로 이동하는 다음 프레임을 계산하고 game over여부를 반환
    const MapManager& getMap();             // 현재 진행중인 게임의 맵을 반환
    int getDirection();                     // 현재 Snake의 진행중인 방향 반환
};

#endif
```

```cpp
/* 
 * title: Snake-Game.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "Snake-Game/Snake-Game.h"

Snake::Snake(MapManager& gameMap, Point headPoint, int length, int direction, int MAX_LENGTH): MAX_LENGTH(MAX_LENGTH){
    this->length = length;
    this->direction = direction;
    for(int j = 0; j < length; ++j){
        snakeBody.push_back(headPoint);
        snakePoints.insert(headPoint);
        gameMap.set(headPoint, SNAKE);
        headPoint += DIRECTIONS[(direction + 2) % 4];
    }
}
Point Snake::getNewHead(){
    Point newH = snakeBody.front();
    newH += DIRECTIONS[direction];
    return newH;
}
void Snake::pushFront(Point body){
    insertQueue.push(body);
    snakeBody.push_front(body);
    ++length;
}
void Snake::popBack(){
    eraseQueue.push(snakeBody.back());
    snakeBody.pop_back();
    --length;
}
bool Snake::checkValidity(MapManager& gameMap){
    bool ret = true;
    while(!eraseQueue.empty()){
        snakePoints.erase(eraseQueue.front());
        gameMap.set(eraseQueue.front(), EMPTY);
        eraseQueue.pop();
    }
    while(!insertQueue.empty()){
        if(snakePoints.find(insertQueue.front()) != snakePoints.end())
            ret = false;
        snakePoints.insert(insertQueue.front());
        gameMap.set(insertQueue.front(), SNAKE);
        insertQueue.pop();
    }
    if(length < 3)
        ret = false;
    return ret;
}
Item Snake::move(int direction, MapManager& gameMap){
    this->direction = direction;
    Point newH = getNewHead();
    Item ret = EMPTY;
    switch(gameMap.get(newH)){
        case SNAKE:
        case EMPTY:
            generalMove(newH);
            break;
        case GROWTH:
            ret = GROWTH;
            growthMove(newH);
            break;
        case POISON:
            ret = POISON;
            poisonMove(newH);
            break;
        case WALL:
            ret = ERROR;
            break;
        case GATE:
            break;
    }
    while(length > MAX_LENGTH)
        popBack();
    if(!checkValidity(gameMap))
        ret = ERROR;
    return ret;
}
void Snake::generalMove(Point newH){
    pushFront(newH);
    popBack();
}
void Snake::growthMove(Point newH){
    pushFront(newH);
}
void Snake::poisonMove(Point newH){
    pushFront(newH);
    popBack();
    popBack();
}
Point Snake::getHead(){
    return snakeBody.front();
}
bool Snake::checkPoint(Point p){
    return snakePoints.find(p) != snakePoints.end();
}
int Snake::getDirection(){
    return direction;
}

GameRunner::GameRunner(const MapItem& gameMapInfo, int length){
    this->gameMap = gameMapInfo.gameMap;
    for(int y = 0; y < this->gameMap.height; ++y){
        for(int x = 0; x < this->gameMap.width; ++x){
            switch(this->gameMap.get(x, y)){
                case EMPTY:
                    itemsCandidates.insert({totalNumberOfItemCandidates++, Point(x, y)});
                    break;
                case WALL:
                    break;
            }
        }
    }
    snake = Snake(this->gameMap, gameMapInfo.startPoint, length, gameMapInfo.startDirection, gameMapInfo.MAX_SCORE_BODY);
}
Point GameRunner::getRandomItemPoint(std::queue<std::pair<IndexedPoint, int>>& timeQ){
    IndexedPoint ip = {-1, {-1, -1}};
    for(int maxCounter = 0; maxCounter < 100 && !itemsCandidates.empty(); ++maxCounter){
        int idx = randomGenerator.getRandom(0, totalNumberOfItemCandidates - 1);
        auto itr = itemsCandidates.lower_bound({idx, Point()});
        if(itr == itemsCandidates.end())
            itr = itemsCandidates.begin();
        if(!snake.checkPoint(itr->p)){
            itemsCandidates.erase(itr);
            ip = *itr;
            timeQ.push({ip, frames});
            break;
        }
    }
    return ip.p;
}
void GameRunner::updateGrowth(){
    while(!growthTimeQ.empty()
            && growthTimeQ.front().second + ITEM_MAX_TIME < frames){
        IndexedPoint t = growthTimeQ.front().first;
        growthTimeQ.pop();
        auto itr = itemsUsed.find(t.p);
        if(itr == itemsUsed.end()){
            --numberOfGrowth;
            gameMap.set(t.p, EMPTY);
        }else
            itemsUsed.erase(itr);
        itemsCandidates.insert(t);
    }
    int cnt = randomGenerator.getRandom(0, MAX_NUMBER_OF_GROWTH - numberOfGrowth);
    for(int j = 0; j < cnt && !itemsCandidates.empty(); ++j){
        Point p = getRandomItemPoint(growthTimeQ);
        if(p.x == -1 && p.y == -1)
            break;
        gameMap.set(p, GROWTH);
        ++numberOfGrowth;
    }
}
void GameRunner::updatePoison(){
    while(!poisonTimeQ.empty()
            && poisonTimeQ.front().second + ITEM_MAX_TIME < frames){
        IndexedPoint t = poisonTimeQ.front().first;
        poisonTimeQ.pop();
        auto itr = itemsUsed.find(t.p);
        if(itr == itemsUsed.end()){
            --numberOfPoison;
            gameMap.set(t.p, EMPTY);
        }else
            itemsUsed.erase(itr);
        itemsCandidates.insert(t);
    }
    int cnt = randomGenerator.getRandom(0, MAX_NUMBER_OF_POISON - numberOfPoison);
    for(int j = 0; j < cnt && !itemsCandidates.empty(); ++j){
        Point p = getRandomItemPoint(poisonTimeQ);
        if(p.x == -1 && p.y == -1)
            break;
        gameMap.set(p, POISON);
        ++numberOfPoison;
    }
}
bool GameRunner::nextFrame(int direction){
    switch(snake.move(direction, gameMap)){
        case ERROR:
            return false;
        case GROWTH:
            --numberOfGrowth;
            itemsUsed.insert(snake.getHead());
            break;
        case POISON:
            --numberOfPoison;
            itemsUsed.insert(snake.getHead());
            break;
    }

    ++frames;
    updateGrowth();
    updatePoison();
    return true;
}
const MapManager& GameRunner::getMap(){
    return gameMap;
}
int GameRunner::getDirection(){
    return snake.getDirection();
}
```

### 4 단계

* 3단계 프로그램에서, Map의 Wall의 임의의 위치에 한 쌍의 Gate가 출현할 수 있도록 변경하고, 각 Gate에 Snake가 통과할 수 있도록 수정한다.
* 게임 규칙 #3, #4, #5를 준수해야한다
* Wall(Immune Wall 포함)과 Gate를 Map배열에 표현할 때 값을 결정한다
    * 화면상에 표현시, Gate는 Wall과 구분될 수 있도록한다
    * Map Data에서 Gate는 7과 같이 하여, 다른 요소와 구분할 수 있도록 한다

#### 4단계 구현 내용

```cpp
/* 
 * title: Snake-Game.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SNAKE_SNAKE_GAME
#define SNAKE_SNAKE_GAME

#include "Primitives/Item.h"
#include "Primitives/Point.h"
#include "Primitives/MapManager.h"
#include "Primitives/MapItem.h"
#include "FileManager.h"
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
    // 범위 [s, e]에서 균등 분포로 랜덤한 값 하나를 반환
    int getRandom(int s, int e){
        return rand() % (e - s + 1) + s;
    }
};

class Snake{
private:
    int MAX_LENGTH = 10;                 // 현재 Snake의 최대 길이

    int length = 3;                            // 현재 Snake의 길이
    int direction = 0;                         // 현재 Snake의 이동 방향
    int portalRemaining = 0;                   // 현재 Snake가 portal에 의해 이동되지 않은 길이
    std::deque<Point> snakeBody;               // 현재 Snake의 몸통 좌표. Head부터 이웃한 순서대로 front에 위치
    std::queue<Point> eraseQueue, insertQueue; // 제거/추가될 몸통 좌표
    std::set<Point> snakePoints;               // 몸통 좌표 중복 체크
    
    Point getNewHead();                        // 이동 방향에 대한 새로운 좌표(head) 가져오기
    void pushFront(Point body);                // Head쪽에 새로운 몸통 (새로운 head) 추가
    void popBack();                            // 꼬리쪽의 몸통 제거
    bool checkValidity(MapManager& gameMap);   // Snake 스스로 몸통이 겹치는지 검증

    void generalMove(Point newH);              // 위치 newH로 일반적인 이동
    void growthMove(Point newH);               // 위치 newH로 성장 이동 - Growth 아이템 효과
    void poisonMove(Point newH);               // 위치 newH로 독 이동 - Poison 아이템 효과
public:
    Snake(){}
    Snake(MapManager& gameMap, Point headPoint={0, 0}, int length=3, int direction=0, int MAX_LENGTH=10);

    // direction방향으로 portals의 포탈을 가진 gameMap맵에서 이동연산 수행
    // 섭취한 아이템을 반환, 유효하지 않은 상황이 발생할 경우 ERROR 반환
    Item move(int direction, MapManager& gameMap, std::vector<Point>& portals);

    Point getHead();            // 현재 Snake의 머리(head)의 좌표를 반환
    bool checkPoint(Point p);   // 위치 p가 Snake가 존재하는 위치에 속하는지 여부를 반환
    bool isInPortal();          // 현재 Snake가 Portal를 타고 있는지 여부를 반환
    int getDirection();
};

class GameRunner{
private:
    MapManager gameMap;                     // 현재 진행중인 게임의 맵 객체
    Snake snake;                            // 현재 진행중인 게임의 Snake 객체

    int frames = 0;                         // 현재 진행중인 게임의 frame 수

    const int ITEM_MAX_TIME = 100;           // 아이템이 사라지지 않는 시간
    const int MAX_NUMBER_OF_GROWTH = 3;     // Growth 아이템의 출현 최대 개수
    const int MAX_NUMBER_OF_POISON = 3;     // Poison 아이템의 출현 최대 개수

    RandomGenerator randomGenerator;        // 랜덤 정수 생성기

    int totalNumberOfItemCandidates = 0;    // 아이템이 나타날 수 있는 위치의 최대 개수
    std::set<IndexedPoint> itemsCandidates; // 아이템이 나타날 수 있는 위치
    std::vector<Point> portalCandidates;    // 포탈이 나타날 수 있는 위치

    std::queue<std::pair<IndexedPoint, int>> growthTimeQ, poisonTimeQ; // 존재 가능 시간을 초과한 아이템을 관리하기 위한 큐
    std::multiset<Point> itemsUsed;         // 현재 Snake에 의해 아이템이 사용된 좌표
    int numberOfGrowth = 0;                 // 현재 존재하는 Growth 아이템 개수
    int numberOfPoison = 0;                 // 현재 존재하는 Poison 아이템 개수

    int portalTime = -1;                    // 포탈이 나타난 마지막 시간(프레임)
    std::vector<Point> portals;             // 현재 존재하는 포탈의 위치 (2개)

    // itemsCandidates에서 유요한 위치를 균등 랜덤으로 뽑아 반환
    // 최대 100회 시도를 통해 유효한 위치를 뽑는데 실패했다면 {-1, -1}을 반환
    Point getRandomItemPoint(std::queue<std::pair<IndexedPoint, int>>& timeQ);

    void updateGrowth();                    // Growth 아이템 업데이트
    void updatePoison();                    // Poison 아이템 업데이트
    void updatePortal();                    // Portal 업데이트
public:
    // gameMap의 맵을 가지고 Snake의 초기 위치가 startPoint이며 길이가 length이고 방향이 direction인 게임을 생성
    GameRunner(const MapItem& gameMapInfo, int length=3);

    bool nextFrame(int direction);          // Snake가 direction방향으로 이동하는 다음 프레임을 계산하고 game over여부를 반환
    const MapManager& getMap();             // 현재 진행중인 게임의 맵을 반환
    int getDirection();                     // 현재 Snake의 진행중인 방향 반환
};

#endif
```

```cpp
/* 
 * title: Snake-Game.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "Snake-Game/Snake-Game.h"

Snake::Snake(MapManager& gameMap, Point headPoint, int length, int direction, int MAX_LENGTH): MAX_LENGTH(MAX_LENGTH){
    this->length = length;
    this->direction = direction;
    portalRemaining = 0;
    for(int j = 0; j < length; ++j){
        snakeBody.push_back(headPoint);
        snakePoints.insert(headPoint);
        gameMap.set(headPoint, SNAKE);
        headPoint += DIRECTIONS[(direction + 2) % 4];
    }
}
Point Snake::getNewHead(){
    Point newH = snakeBody.front();
    newH += DIRECTIONS[direction];
    return newH;
}
void Snake::pushFront(Point body){
    insertQueue.push(body);
    snakeBody.push_front(body);
    ++length;
}
void Snake::popBack(){
    eraseQueue.push(snakeBody.back());
    snakeBody.pop_back();
    --length;
}
bool Snake::checkValidity(MapManager& gameMap){
    bool ret = true;
    while(!eraseQueue.empty()){
        snakePoints.erase(eraseQueue.front());
        gameMap.set(eraseQueue.front(), EMPTY);
        eraseQueue.pop();
    }
    while(!insertQueue.empty()){
        if(snakePoints.find(insertQueue.front()) != snakePoints.end())
            ret = false;
        snakePoints.insert(insertQueue.front());
        gameMap.set(insertQueue.front(), SNAKE);
        insertQueue.pop();
    }
    if(length < 3)
        ret = false;
    return ret;
}
Item Snake::move(int direction, MapManager& gameMap, std::vector<Point>& portals){
    this->direction = direction;
    Point newH = getNewHead();
    Item ret = EMPTY;
    if(gameMap.get(newH) == GATE){
        Point portal = portals.front(), teleportP;
        if(portal == newH)
            portal = portals.back();
        int dirIdx[] = {this->direction, (this->direction + 1) % 4, (this->direction - 1 + 4) % 4, (this->direction + 2) % 4};
        for(int idx: dirIdx){
            teleportP = portal + DIRECTIONS[idx];
            if(!teleportP.isValid(gameMap.width, gameMap.height))
                continue;
            if(gameMap.get(teleportP) != WALL && gameMap.get(teleportP) != IMWALL && gameMap.get(teleportP) != GATE){
                this->direction = idx;
                break;
            }
        }
        newH = teleportP;
        portalRemaining = length;
    }
    switch(gameMap.get(newH)){
        case SNAKE:
        case EMPTY:
            generalMove(newH);
            break;
        case GROWTH:
            ret = GROWTH;
            growthMove(newH);
            break;
        case POISON:
            ret = POISON;
            poisonMove(newH);
            break;
        case WALL:
            ret = ERROR;
            break;
        case GATE:
            break;
    }
    while(length > MAX_LENGTH)
        popBack();
    if(!checkValidity(gameMap))
        ret = ERROR;
    return ret;
}
void Snake::generalMove(Point newH){
    pushFront(newH);
    popBack();
    portalRemaining = std::max(0, portalRemaining - 1);
}
void Snake::growthMove(Point newH){
    pushFront(newH);
}
void Snake::poisonMove(Point newH){
    pushFront(newH);
    popBack();
    popBack();
    portalRemaining = std::max(0, portalRemaining - 2);
}
Point Snake::getHead(){
    return snakeBody.front();
}
bool Snake::checkPoint(Point p){
    return snakePoints.find(p) != snakePoints.end();
}
bool Snake::isInPortal(){
    return portalRemaining;
}
int Snake::getDirection(){
    return direction;
}

GameRunner::GameRunner(const MapItem& gameMapInfo, int length){
    this->gameMap = gameMapInfo.gameMap;
    for(int y = 0; y < this->gameMap.height; ++y){
        for(int x = 0; x < this->gameMap.width; ++x){
            switch(this->gameMap.get(x, y)){
                case EMPTY:
                    itemsCandidates.insert({totalNumberOfItemCandidates++, Point(x, y)});
                    break;
                case WALL:
                    portalCandidates.push_back(Point(x, y));
                    break;
            }
        }
    }
    snake = Snake(this->gameMap, gameMapInfo.startPoint, length, gameMapInfo.startDirection, gameMapInfo.MAX_SCORE_BODY);
}
Point GameRunner::getRandomItemPoint(std::queue<std::pair<IndexedPoint, int>>& timeQ){
    IndexedPoint ip = {-1, {-1, -1}};
    for(int maxCounter = 0; maxCounter < 100 && !itemsCandidates.empty(); ++maxCounter){
        int idx = randomGenerator.getRandom(0, totalNumberOfItemCandidates - 1);
        auto itr = itemsCandidates.lower_bound({idx, Point()});
        if(itr == itemsCandidates.end())
            itr = itemsCandidates.begin();
        if(!snake.checkPoint(itr->p)){
            itemsCandidates.erase(itr);
            ip = *itr;
            timeQ.push({ip, frames});
            break;
        }
    }
    return ip.p;
}
void GameRunner::updateGrowth(){
    while(!growthTimeQ.empty()
            && growthTimeQ.front().second + ITEM_MAX_TIME < frames){
        IndexedPoint t = growthTimeQ.front().first;
        growthTimeQ.pop();
        auto itr = itemsUsed.find(t.p);
        if(itr == itemsUsed.end()){
            --numberOfGrowth;
            gameMap.set(t.p, EMPTY);
        }else
            itemsUsed.erase(itr);
        itemsCandidates.insert(t);
    }
    int cnt = randomGenerator.getRandom(0, MAX_NUMBER_OF_GROWTH - numberOfGrowth);
    for(int j = 0; j < cnt && !itemsCandidates.empty(); ++j){
        Point p = getRandomItemPoint(growthTimeQ);
        if(p.x == -1 && p.y == -1)
            break;
        gameMap.set(p, GROWTH);
        ++numberOfGrowth;
    }
}
void GameRunner::updatePoison(){
    while(!poisonTimeQ.empty()
            && poisonTimeQ.front().second + ITEM_MAX_TIME < frames){
        IndexedPoint t = poisonTimeQ.front().first;
        poisonTimeQ.pop();
        auto itr = itemsUsed.find(t.p);
        if(itr == itemsUsed.end()){
            --numberOfPoison;
            gameMap.set(t.p, EMPTY);
        }else
            itemsUsed.erase(itr);
        itemsCandidates.insert(t);
    }
    int cnt = randomGenerator.getRandom(0, MAX_NUMBER_OF_POISON - numberOfPoison);
    for(int j = 0; j < cnt && !itemsCandidates.empty(); ++j){
        Point p = getRandomItemPoint(poisonTimeQ);
        if(p.x == -1 && p.y == -1)
            break;
        gameMap.set(p, POISON);
        ++numberOfPoison;
    }
}
void GameRunner::updatePortal(){
    if(snake.isInPortal() || portalCandidates.size() < 2 || portalTime + ITEM_MAX_TIME >= frames)
        return;
    for(int j = 0; j < portals.size(); ++j)
        gameMap.set(portals[j], WALL);
    portals.clear();

    int cnt = randomGenerator.getRandom(0, 1);
    if(!cnt)
        return;

    int idx1 = randomGenerator.getRandom(0, portalCandidates.size() - 1);
    int idx2;
    do
        idx2 = randomGenerator.getRandom(0, portalCandidates.size() - 1);
    while(idx1 == idx2);

    portals.push_back(portalCandidates[idx1]);
    portals.push_back(portalCandidates[idx2]);
    for(int j = 0; j < portals.size(); ++j)
        gameMap.set(portals[j], GATE);
    portalTime = frames;
}
bool GameRunner::nextFrame(int direction){
    switch(snake.move(direction, gameMap, portals)){
        case ERROR:
            return false;
        case GROWTH:
            --numberOfGrowth;
            itemsUsed.insert(snake.getHead());
            break;
        case POISON:
            --numberOfPoison;
            itemsUsed.insert(snake.getHead());
            break;
    }

    ++frames;
    updateGrowth();
    updatePoison();
    updatePortal();
    return true;
}
const MapManager& GameRunner::getMap(){
    return gameMap;
}
int GameRunner::getDirection(){
    return snake.getDirection();
}
```

### 5 단계

* 4단계 프로그램에서, 우측에 게임 점수를 표시하는 화면을 구성한다.
* 게임 점수는 게임 규칙 #6을 준수한다
* Mission
    * 구성한 Map의 정의에 고정 값을 주거나
    * 매 게임마다 임의의 값을 주는 방식으로 처리한다
* Mission을 달성하면 다음 Map으로 진행하도록 프로그램을 완성한다
    * Stage는 최소 4개로 구성하고, 각 Stage의 Map은 서로 달라야 한다

#### 5단계 구현 내용

```cpp
/* 
 * title: Primitives/Score.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SNAKE_SCORE
#define SNAKE_SCORE

struct Score{
    const int MAX_SCORE_BODY;
    int scoreBody;
    int scoreGrowth;
    int scorePoison;
    int scoreGate;

    Score(int maxBody=10): MAX_SCORE_BODY(maxBody){
        scoreBody = 0;
        scoreGrowth = 0;
        scorePoison = 0;
        scoreGate = 0;
    }
};

#endif
```

```cpp
/* 
 * title: Primitives/Mission.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SNAKE_MISSION
#define SNAKE_MISSION

struct Mission{
    const int goalBody;
    const int goalGrowth;
    const int goalPoison;
    const int goalGate;

    bool achBody;
    bool achGrowth;
    bool achPoison;
    bool achGate;

    Mission(int goalBody=10, int goalGrowth=5, int goalPoison=2, int goalGate=1): 
        goalBody(goalBody), goalGrowth(goalGrowth), goalPoison(goalPoison), goalGate(goalGate){
        
        achBody = false;
        achGrowth = false;
        achPoison = false;
        achGate = false;
    }

    bool achieved() const{
        return achBody && achGrowth && achPoison && achGate;
    }
};

#endif
```

```cpp
/* 
 * title: StatusManager.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SNAKE_STATUS_MANAGER
#define SNAKE_STATUS_MANAGER

#include "Primitives/Score.h"
#include "Primitives/Mission.h"

class StatusManager{
private:
    Score score;
    Mission mission;
public:
    StatusManager(int maxBody=10, int goalBody=10, int goalGrowth=5, int goalPoison=2, int goalGate=1);

    void scoreBody();
    void descoreBody();
    void scoreGrowth();
    void scorePoison();
    void scoreGate();

    const Score& getScore() const;
    const Mission& getMission() const;
};

#endif
```

```cpp
/* 
 * title: StatusManager.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "Snake-Game/StatusManager.h"

StatusManager::StatusManager(int maxBody, int goalBody, int goalGrowth, int goalPoison, int goalGate):
        score(Score(maxBody)), mission(Mission(goalBody, goalGrowth, goalPoison, goalGate)){
}

void StatusManager::scoreBody(){
    score.scoreBody++;
    if(score.scoreBody > score.MAX_SCORE_BODY)
        score.scoreBody--;
    if(mission.goalBody <= score.scoreBody)
        mission.achBody = true;
    else
        mission.achBody = false;
}
void StatusManager::descoreBody(){
    score.scoreBody--;
    if(score.scoreBody < 0)
        score.scoreBody++;
    if(mission.goalBody <= score.scoreBody)
        mission.achBody = true;
    else
        mission.achBody = false;
}
void StatusManager::scoreGrowth(){
    score.scoreGrowth++;
    if(mission.goalGrowth <= score.scoreGrowth)
        mission.achGrowth = true;
}
void StatusManager::scorePoison(){
    score.scorePoison++;
    if(mission.goalPoison <= score.scorePoison)
        mission.achPoison = true;
}
void StatusManager::scoreGate(){
    score.scoreGate++;
    if(mission.goalGate <= score.scoreGate)
        mission.achGate = true;
}

const Score& StatusManager::getScore() const{
    return score;
}
const Mission& StatusManager::getMission() const{
    return mission;
}
```

```cpp
/* 
 * title: Snake-Game.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SNAKE_SNAKE_GAME
#define SNAKE_SNAKE_GAME

#include "Primitives/Item.h"
#include "Primitives/Point.h"
#include "Primitives/MapManager.h"
#include "Primitives/MapItem.h"
#include "Primitives/UserItem.h"
#include "StatusManager.h"
#include "FileManager.h"
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
    // 범위 [s, e]에서 균등 분포로 랜덤한 값 하나를 반환
    int getRandom(int s, int e){
        return rand() % (e - s + 1) + s;
    }
};

class Snake{
private:
    int MAX_LENGTH = 10;                 // 현재 Snake의 최대 길이

    int length = 3;                            // 현재 Snake의 길이
    int direction = 0;                         // 현재 Snake의 이동 방향
    int portalRemaining = 0;                   // 현재 Snake가 portal에 의해 이동되지 않은 길이
    std::deque<Point> snakeBody;               // 현재 Snake의 몸통 좌표. Head부터 이웃한 순서대로 front에 위치
    std::queue<Point> eraseQueue, insertQueue; // 제거/추가될 몸통 좌표
    std::set<Point> snakePoints;               // 몸통 좌표 중복 체크
    
    Point getNewHead();                        // 이동 방향에 대한 새로운 좌표(head) 가져오기
    void pushFront(Point body);                // Head쪽에 새로운 몸통 (새로운 head) 추가
    void popBack();                            // 꼬리쪽의 몸통 제거
    bool checkValidity(MapManager& gameMap);   // Snake 스스로 몸통이 겹치는지 검증

    void generalMove(Point newH);              // 위치 newH로 일반적인 이동
    void growthMove(Point newH);               // 위치 newH로 성장 이동 - Growth 아이템 효과
    void poisonMove(Point newH);               // 위치 newH로 독 이동 - Poison 아이템 효과
public:
    Snake(){}
    Snake(MapManager& gameMap, Point headPoint={0, 0}, int length=3, int direction=0, int MAX_LENGTH=10);

    // direction방향으로 portals의 포탈을 가진 gameMap맵에서 이동연산 수행
    // 섭취한 아이템을 반환, 유효하지 않은 상황이 발생할 경우 ERROR 반환
    Item move(int direction, MapManager& gameMap, StatusManager& status, std::vector<Point>& portals);

    Point getHead();            // 현재 Snake의 머리(head)의 좌표를 반환
    bool checkPoint(Point p);   // 위치 p가 Snake가 존재하는 위치에 속하는지 여부를 반환
    bool isInPortal();          // 현재 Snake가 Portal를 타고 있는지 여부를 반환
    int getDirection();
};

class GameRunner{
private:
    MapManager gameMap;                     // 현재 진행중인 게임의 맵 객체
    Snake snake;                            // 현재 진행중인 게임의 Snake 객체
    StatusManager status;

    int frames = 0;                         // 현재 진행중인 게임의 frame 수

    const int ITEM_MAX_TIME = 100;           // 아이템이 사라지지 않는 시간
    const int MAX_NUMBER_OF_GROWTH = 3;     // Growth 아이템의 출현 최대 개수
    const int MAX_NUMBER_OF_POISON = 3;     // Poison 아이템의 출현 최대 개수

    RandomGenerator randomGenerator;        // 랜덤 정수 생성기

    int totalNumberOfItemCandidates = 0;    // 아이템이 나타날 수 있는 위치의 최대 개수
    std::set<IndexedPoint> itemsCandidates; // 아이템이 나타날 수 있는 위치
    std::vector<Point> portalCandidates;    // 포탈이 나타날 수 있는 위치

    std::queue<std::pair<IndexedPoint, int>> growthTimeQ, poisonTimeQ; // 존재 가능 시간을 초과한 아이템을 관리하기 위한 큐
    std::multiset<Point> itemsUsed;         // 현재 Snake에 의해 아이템이 사용된 좌표
    int numberOfGrowth = 0;                 // 현재 존재하는 Growth 아이템 개수
    int numberOfPoison = 0;                 // 현재 존재하는 Poison 아이템 개수

    int portalTime = -1;                    // 포탈이 나타난 마지막 시간(프레임)
    std::vector<Point> portals;             // 현재 존재하는 포탈의 위치 (2개)

    // itemsCandidates에서 유요한 위치를 균등 랜덤으로 뽑아 반환
    // 최대 100회 시도를 통해 유효한 위치를 뽑는데 실패했다면 {-1, -1}을 반환
    Point getRandomItemPoint(std::queue<std::pair<IndexedPoint, int>>& timeQ);

    void updateGrowth();                    // Growth 아이템 업데이트
    void updatePoison();                    // Poison 아이템 업데이트
    void updatePortal();                    // Portal 업데이트
public:
    // gameMap의 맵을 가지고 Snake의 초기 위치가 startPoint이며 길이가 length이고 방향이 direction인 게임을 생성
    GameRunner(const MapItem& gameMapInfo, int length=3);

    bool nextFrame(int direction);          // Snake가 direction방향으로 이동하는 다음 프레임을 계산하고 game over여부를 반환
    const MapManager& getMap();             // 현재 진행중인 게임의 맵을 반환
    const StatusManager& getStatus();       // 현재 진행중인 게임의 상태 (점수, 미션) 반환
    int getDirection();                     // 현재 Snake의 진행중인 방향 반환
};

#endif
```

```cpp
/* 
 * title: Snake-Game.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "Snake-Game/Snake-Game.h"

Snake::Snake(MapManager& gameMap, Point headPoint, int length, int direction, int MAX_LENGTH): MAX_LENGTH(MAX_LENGTH){
    this->length = length;
    this->direction = direction;
    portalRemaining = 0;
    for(int j = 0; j < length; ++j){
        snakeBody.push_back(headPoint);
        snakePoints.insert(headPoint);
        gameMap.set(headPoint, SNAKE);
        headPoint += DIRECTIONS[(direction + 2) % 4];
    }
}
Point Snake::getNewHead(){
    Point newH = snakeBody.front();
    newH += DIRECTIONS[direction];
    return newH;
}
void Snake::pushFront(Point body){
    insertQueue.push(body);
    snakeBody.push_front(body);
    ++length;
}
void Snake::popBack(){
    eraseQueue.push(snakeBody.back());
    snakeBody.pop_back();
    --length;
}
bool Snake::checkValidity(MapManager& gameMap){
    bool ret = true;
    while(!eraseQueue.empty()){
        snakePoints.erase(eraseQueue.front());
        gameMap.set(eraseQueue.front(), EMPTY);
        eraseQueue.pop();
    }
    while(!insertQueue.empty()){
        if(snakePoints.find(insertQueue.front()) != snakePoints.end())
            ret = false;
        snakePoints.insert(insertQueue.front());
        gameMap.set(insertQueue.front(), SNAKE);
        insertQueue.pop();
    }
    if(length < 3)
        ret = false;
    return ret;
}
Item Snake::move(int direction, MapManager& gameMap, StatusManager& status, std::vector<Point>& portals){
    this->direction = direction;
    Point newH = getNewHead();
    Item ret = EMPTY;
    if(gameMap.get(newH) == GATE){
        Point portal = portals.front(), teleportP;
        if(portal == newH)
            portal = portals.back();
        int dirIdx[] = {this->direction, (this->direction + 1) % 4, (this->direction - 1 + 4) % 4, (this->direction + 2) % 4};
        for(int idx: dirIdx){
            teleportP = portal + DIRECTIONS[idx];
            if(!teleportP.isValid(gameMap.width, gameMap.height))
                continue;
            if(gameMap.get(teleportP) != WALL && gameMap.get(teleportP) != IMWALL && gameMap.get(teleportP) != GATE){
                this->direction = idx;
                break;
            }
        }
        newH = teleportP;
        portalRemaining = length;
        status.scoreGate();
    }
    switch(gameMap.get(newH)){
        case SNAKE:
        case EMPTY:
            generalMove(newH);
            break;
        case GROWTH:
            ret = GROWTH;
            growthMove(newH);
            status.scoreGrowth();
            status.scoreBody();
            break;
        case POISON:
            ret = POISON;
            poisonMove(newH);
            status.scorePoison();
            status.descoreBody();
            break;
        case WALL:
            ret = ERROR;
            break;
        case GATE:
            break;
    }
    while(length > MAX_LENGTH)
        popBack();
    if(!checkValidity(gameMap))
        ret = ERROR;
    return ret;
}
void Snake::generalMove(Point newH){
    pushFront(newH);
    popBack();
    portalRemaining = std::max(0, portalRemaining - 1);
}
void Snake::growthMove(Point newH){
    pushFront(newH);
}
void Snake::poisonMove(Point newH){
    pushFront(newH);
    popBack();
    popBack();
    portalRemaining = std::max(0, portalRemaining - 2);
}
Point Snake::getHead(){
    return snakeBody.front();
}
bool Snake::checkPoint(Point p){
    return snakePoints.find(p) != snakePoints.end();
}
bool Snake::isInPortal(){
    return portalRemaining;
}
int Snake::getDirection(){
    return direction;
}

GameRunner::GameRunner(const MapItem& gameMapInfo, int length):
    status(StatusManager(gameMapInfo.MAX_SCORE_BODY, gameMapInfo.goalBody, gameMapInfo.goalGrowth, gameMapInfo.goalPoison, gameMapInfo.goalGate)){
    this->gameMap = gameMapInfo.gameMap;
    for(int y = 0; y < this->gameMap.height; ++y){
        for(int x = 0; x < this->gameMap.width; ++x){
            switch(this->gameMap.get(x, y)){
                case EMPTY:
                    itemsCandidates.insert({totalNumberOfItemCandidates++, Point(x, y)});
                    break;
                case WALL:
                    portalCandidates.push_back(Point(x, y));
                    break;
            }
        }
    }
    for(int i = 0; i < length; ++i) status.scoreBody();
    snake = Snake(this->gameMap, gameMapInfo.startPoint, length, gameMapInfo.startDirection, gameMapInfo.MAX_SCORE_BODY);
}
Point GameRunner::getRandomItemPoint(std::queue<std::pair<IndexedPoint, int>>& timeQ){
    IndexedPoint ip = {-1, {-1, -1}};
    for(int maxCounter = 0; maxCounter < 100 && !itemsCandidates.empty(); ++maxCounter){
        int idx = randomGenerator.getRandom(0, totalNumberOfItemCandidates - 1);
        auto itr = itemsCandidates.lower_bound({idx, Point()});
        if(itr == itemsCandidates.end())
            itr = itemsCandidates.begin();
        if(!snake.checkPoint(itr->p)){
            itemsCandidates.erase(itr);
            ip = *itr;
            timeQ.push({ip, frames});
            break;
        }
    }
    return ip.p;
}
void GameRunner::updateGrowth(){
    while(!growthTimeQ.empty()
            && growthTimeQ.front().second + ITEM_MAX_TIME < frames){
        IndexedPoint t = growthTimeQ.front().first;
        growthTimeQ.pop();
        auto itr = itemsUsed.find(t.p);
        if(itr == itemsUsed.end()){
            --numberOfGrowth;
            gameMap.set(t.p, EMPTY);
        }else
            itemsUsed.erase(itr);
        itemsCandidates.insert(t);
    }
    int cnt = randomGenerator.getRandom(0, MAX_NUMBER_OF_GROWTH - numberOfGrowth);
    for(int j = 0; j < cnt && !itemsCandidates.empty(); ++j){
        Point p = getRandomItemPoint(growthTimeQ);
        if(p.x == -1 && p.y == -1)
            break;
        gameMap.set(p, GROWTH);
        ++numberOfGrowth;
    }
}
void GameRunner::updatePoison(){
    while(!poisonTimeQ.empty()
            && poisonTimeQ.front().second + ITEM_MAX_TIME < frames){
        IndexedPoint t = poisonTimeQ.front().first;
        poisonTimeQ.pop();
        auto itr = itemsUsed.find(t.p);
        if(itr == itemsUsed.end()){
            --numberOfPoison;
            gameMap.set(t.p, EMPTY);
        }else
            itemsUsed.erase(itr);
        itemsCandidates.insert(t);
    }
    int cnt = randomGenerator.getRandom(0, MAX_NUMBER_OF_POISON - numberOfPoison);
    for(int j = 0; j < cnt && !itemsCandidates.empty(); ++j){
        Point p = getRandomItemPoint(poisonTimeQ);
        if(p.x == -1 && p.y == -1)
            break;
        gameMap.set(p, POISON);
        ++numberOfPoison;
    }
}
void GameRunner::updatePortal(){
    if(snake.isInPortal() || portalCandidates.size() < 2 || portalTime + ITEM_MAX_TIME >= frames)
        return;
    for(int j = 0; j < portals.size(); ++j)
        gameMap.set(portals[j], WALL);
    portals.clear();

    int cnt = randomGenerator.getRandom(0, 1);
    if(!cnt)
        return;

    int idx1 = randomGenerator.getRandom(0, portalCandidates.size() - 1);
    int idx2;
    do
        idx2 = randomGenerator.getRandom(0, portalCandidates.size() - 1);
    while(idx1 == idx2);

    portals.push_back(portalCandidates[idx1]);
    portals.push_back(portalCandidates[idx2]);
    for(int j = 0; j < portals.size(); ++j)
        gameMap.set(portals[j], GATE);
    portalTime = frames;
}
bool GameRunner::nextFrame(int direction){
    switch(snake.move(direction, gameMap, status, portals)){
        case ERROR:
            return false;
        case GROWTH:
            --numberOfGrowth;
            itemsUsed.insert(snake.getHead());
            break;
        case POISON:
            --numberOfPoison;
            itemsUsed.insert(snake.getHead());
            break;
    }

    ++frames;
    updateGrowth();
    updatePoison();
    updatePortal();
    return true;
}
const MapManager& GameRunner::getMap(){
    return gameMap;
}
const StatusManager& GameRunner::getStatus(){
    return status;
}
int GameRunner::getDirection(){
    return snake.getDirection();
}
```