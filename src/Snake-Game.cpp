#include "Snake-Game/Snake-Game.h"

Snake::Snake(Point headPoint, int length=3, int direction=0){
    this->length = length;
    this->direction = direction;
    for(int j = 0; j < length; ++j){
        snakeBody.push_back(headPoint);
        snakePoints.insert(headPoint);
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
bool Snake::checkValidity(){
    bool ret = true;
    while(!eraseQueue.empty()){
        snakePoints.erase(eraseQueue.front());
        eraseQueue.pop();
    }
    while(!insertQueue.empty()){
        if(snakePoints.find(insertQueue.front()) != snakePoints.end())
            ret = false;
        snakePoints.insert(insertQueue.front());
        insertQueue.pop();
    }
    return ret;
}
bool Snake::move(){
    Point newH = getNewHead();
    pushFront(newH);
    popBack();
    return checkValidity();
}
bool Snake::growthMove(){
    Point newH = getNewHead();
    pushFront(newH);
    return checkValidity();
}
bool Snake::poisonMove(){
    Point newH = getNewHead();
    pushFront(newH);
    popBack();
    popBack();
    return checkValidity();
}
bool Snake::portal(Point p){
    Point newH = p;
    pushFront(newH);
    popBack();
    return checkValidity();
}
bool Snake::checkPoint(Point p){
    return snakePoints.find(p) != snakePoints.end();
}
int Snake::getDirection(){
    return direction;
}
void Snake::setDirection(int direction){
    this->direction = direction;
}

GameRunner::GameRunner(const std::vector<std::vector<Item>>& gameMap, Point startPoint, int length=3, int direction=0){
    this->gameMap.resize(gameMap.size());
    for(int y = 0; y < gameMap.size(); ++y)
        for(int x = 0; x < gameMap[y].size(); ++x)
            this->gameMap[y].push_back(gameMap[y][x]);
    
    snake = Snake(startPoint, length, direction);
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
            itemsCandidates.insert(t);
        }else
            itemsUsed.erase(itr);
    }
    int cnt = randomGenerator.getRandom(0, MAX_NUMBER_OF_GROWTH - numberOfGrowth);
    for(int j = 0; j < cnt && !itemsCandidates.empty(); ++j){
        Point p = getRandomItemPoint(growthTimeQ);
        if(p.x == -1 && p.y == -1)
            break;
        gameMap[p.y][p.x] = GROWTH;
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
            itemsCandidates.insert(t);
        }else
            itemsUsed.erase(itr);
    }
    int cnt = randomGenerator.getRandom(0, MAX_NUMBER_OF_POISON - numberOfPoison);
    for(int j = 0; j < cnt && !itemsCandidates.empty(); ++j){
        Point p = getRandomItemPoint(poisonTimeQ);
        if(p.x == -1 && p.y == -1)
            break;
        gameMap[p.y][p.x] = POISON;
        ++numberOfPoison;
    }
}
void GameRunner::updatePortal(){

}
bool GameRunner::nextFrame(){
    updateGrowth();
    updatePoison();
    updatePortal();
}