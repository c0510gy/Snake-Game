#include "Snake-Game/Snake-Game.h"

Snake::Snake(MapManager& gameMap, Point headPoint, int length=3, int direction=0){
    this->length = length;
    this->direction = direction;
    portalRemaining = 0;
    for(int j = 0; j < length; ++j){
        snakeBody.push_back(headPoint);
        snakePoints.insert(headPoint);
        gameMap.set(headPoint, SNAKE);
        headPoint += DIR[direction];
    }
}
Point Snake::getNewHead(){
    Point newH = snakeBody.front();
    newH += DIR[direction];
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
bool Snake::move(int direction, MapManager& gameMap, std::vector<Point>& portals){
    Point newH = getNewHead();
    bool valid = true;
    switch(gameMap.get(newH)){
        case EMPTY:
            generalMove();
            break;
        case GROWTH:
            growthMove();
            break;
        case POISON:
            poisonMove();
            break;
        case WALL:
            valid = false;
            break;
        case GATE:
            Point portal = portals.front(), teleportP;
            if(portal == newH)
                portal = portals.back();
            int dirIdx[] = {direction, (direction + 1) % 4, (direction - 1 + 4) % 4, (direction + 2) % 4};
            for(int idx: dirIdx){
                teleportP = portal + DIR[idx];
                if(!teleportP.isValid(gameMap.width, gameMap.height))
                    continue;
                if(gameMap.get(teleportP) == EMPTY){
                    direction = idx;
                    break;
                }
            }
            portalMove(teleportP);
            break;
    }
    if(!checkValidity(gameMap))
        valid = false;
    return valid;
}
void Snake::generalMove(){
    Point newH = getNewHead();
    pushFront(newH);
    popBack();
    portalRemaining = std::max(0, portalRemaining - 1);
}
void Snake::growthMove(){
    Point newH = getNewHead();
    pushFront(newH);
}
void Snake::poisonMove(){
    Point newH = getNewHead();
    pushFront(newH);
    popBack();
    popBack();
    portalRemaining = std::max(0, portalRemaining - 2);
}
void Snake::portalMove(Point p){
    Point newH = p;
    pushFront(newH);
    popBack();
    portalRemaining = length - 1;
}
bool Snake::checkPoint(Point p){
    return snakePoints.find(p) != snakePoints.end();
}
bool Snake::isInPortal(){
    return portalRemaining;
}

GameRunner::GameRunner(const MapManager& gameMap, Point startPoint, int length=3, int direction=0){
    this->gameMap = gameMap;
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
    snake = Snake(this->gameMap, startPoint, length, direction);
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
            itemsCandidates.insert(t);
        }else
            itemsUsed.erase(itr);
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
            itemsCandidates.insert(t);
        }else
            itemsUsed.erase(itr);
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
    if(snake.isInPortal() || portalCandidates.size() < 2 || portalTime + ITEM_MAX_TIME < frames)
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
    if(!snake.move(direction, gameMap, portals))
        return false;

    ++frames;
    updateGrowth();
    updatePoison();
    updatePortal();
    return true;
}