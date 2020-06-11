/* 
 * title: Primitives/Score.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SNAKE_SCORE
#define SNAKE_SCORE

struct Score{
    int maxBodyScore;
    int scoreBody;
    int scoreGrowth;
    int scorePoison;
    int scoreGate;

    Score(){
        maxBodyScore = 0;
        scoreBody = 0;
        scoreGrowth = 0;
        scorePoison = 0;
        scoreGate = 0;
    }
};

#endif