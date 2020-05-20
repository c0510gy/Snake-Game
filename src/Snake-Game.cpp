#include "Snake-Game/Snake-Game.h"

Snake::Snake(Point headPoint, int length, int direction){
    this->length = length;
    this->direction = direction;
    for(int j = 0; j < length; ++j){
        snakeBody.push_back(headPoint);
        headPoint += DIR[direction];
    }
}
bool Snake::move(){
    Point newH = snakeBody.front();
    newH += DIR[direction];
    snakeBody.push_front(newH);
    snakeBody.pop_back();
}
void Snake::growth(){
    Point newH = snakeBody.front();
    newH += DIR[direction];
    snakeBody.push_front(newH);
}
void Snake::poison(){
    Point newH = snakeBody.front();
    newH += DIR[direction];
    snakeBody.push_front(newH);
    snakeBody.pop_back();
    snakeBody.pop_back();
}
void Snake::portal(int x, int y){

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