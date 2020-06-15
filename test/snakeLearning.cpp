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
    SnakeAILearner ai("./mapData/1.txt", eachHiddenLayer, 2000, 0.1);
    long double prevMax = -1;
    for(int g = 1; ; ++g){
        pair<long double, long double> score = ai.nextGen();
        if(prevMax <= score.second){
            cout << "generation " << g << ": " << score.first << ", " << score.second << "\n";
            vector<long double> bestGene = ai.getBestGene();
            for(int j = 0; j < bestGene.size(); ++j){
                cout << bestGene[j] << ",";
            } cout << endl;
            prevMax = score.second;
        }
    }
    
    return 0;
}