/* 
 * title: StatusManager.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "Snake-Game/StatusManager.h"

StatusManager::StatusManager(int goalBody, int goalGrowth, int goalPoison, int goalGate):
        score(), mission(Mission(goalBody, goalGrowth, goalPoison, goalGate)){
}

void StatusManager::scoreBody(){
    score.scoreBody++;
    score.maxBodyScore = std::max(score.maxBodyScore, score.scoreBody);
    if(mission.goalBody <= score.maxBodyScore)
        mission.achBody = true;
}
void StatusManager::descoreBody(){
    score.scoreBody--;
    if(score.scoreBody < 0)
        score.scoreBody++;
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