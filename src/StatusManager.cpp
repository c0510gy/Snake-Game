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

const Score& StatusManager::getScore(){
    return score;
}
const Mission& StatusManager::getMission(){
    return mission;
}