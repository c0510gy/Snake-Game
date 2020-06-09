/* 
 * title: Primitives/Score.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SNAKE_SCORE
#define SNAKE_SCORE

struct Score{
    const int MAX_SCORE_BODY;
    int scoreBody;
    int scoreGrowth;
    int scorePoison;
    int scoreGate;

    Score(int maxBody=10): MAX_SCORE_BODY(maxBody){
        scoreBody = 0;
        scoreGrowth = 0;
        scorePoison = 0;
        scoreGate = 0;
    }
};

#endif