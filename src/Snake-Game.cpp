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