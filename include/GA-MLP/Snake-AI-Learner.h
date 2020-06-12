/* 
 * title: Snake-AI-Learner.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SANAKE_AI_LEARNER
#define SANAKE_AI_LEARNER

#include "Snake-Game/Snake-Game.h"
#include "GA-MLP/Primitives/Sigmoid.h"
#include "GA-MLP/MLP.h"
#include "GA-MLP/GA.h"
#include <string>
#include <algorithm>

class SnakeAILearner{
private:
    int inputLayerNodes, hiddenLayers, outputLayerNodes;
    int population;
    std::vector<int> eachLayer;
    std::vector<MLP<Sigmoid, dSigmoid>> mlps;
    GA ga;
    MapItem mapItem;

    void setWeights();
    int runSimulation(int geneIdx);

public:
    SnakeAILearner(std::string mapDir, const std::vector<int>& eachHiddenLayer, int population, double mutProb=0.05);

    int nextGen();
};

#endif