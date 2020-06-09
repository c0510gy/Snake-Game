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