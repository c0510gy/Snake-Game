#include "Snake-Game/Snake-Game.h"

Snake::Snake(Point headPoint, int length, int direction){
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
bool Snake::portal(int x, int y){
    Point newH = {x, y};
    pushFront(newH);
    popBack();
    return checkValidity();
}
int Snake::getDirection(){
    return direction;
}
void Snake::setDirection(int direction){
    this->direction = direction;
}

GameRunner::GameRunner(const std::vector<std::vector<Item>>& gameMap){

}
void GameRunner::updateGrowth(){

}
void GameRunner::updatePoison(){

}
void GameRunner::updatePortal(){

}
bool GameRunner::nextFrame(){

}