/* 
 * title: snakeLearning.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "Snake-AI-Learner.h"
#include <iostream>
#include <string>
using namespace std;

int main(){
    vector<int> eachHiddenLayer = {20, 12};
    SnakeAILearner ai("./mapData/1.txt", eachHiddenLayer, 1000);
    for(int g = 1; ; ++g){
        long double score = ai.nextGen();
        cout << "generation " << g << ": " << score << "\n";
        if(score > 10000)
            break;
    }
    vector<long double> bestGene = ai.getBestGene();
    for(int j = 0; j < bestGene.size(); ++j){
        cout << bestGene[j] << ",";
    }
    return 0;
}