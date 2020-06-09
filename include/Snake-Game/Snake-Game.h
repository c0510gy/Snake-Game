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