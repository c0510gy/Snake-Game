/* 
 * title: snakeLearning.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "Snake-AI-Learner.h"
#include <iostream>
#include <string>
using namespace std;

int main(){
    vector<int> eachHiddenLayer = {30, 20};
    SnakeAILearner ai("./mapData/1.txt", eachHiddenLayer, 100);
    for(int g = 1; g < 10000; ++g){
        cout << "generation " << g << ": " << ai.nextGen() << "\n";
    }
    vector<long double> bestGene = ai.getBestGene();
    for(int j = 0; j < bestGene.size(); ++j){
        cout << bestGene[j] << ",";
    }
    return 0;
}