/* 
 * title: Snake-AI.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SANAKE_AI
#define SANAKE_AI

#include "Snake-Game/Snake-Game.h"
#include "GA-MLP/Primitives/Sigmoid.h"
#include "GA-MLP/MLP.h"
#include <string>
#include <vector>

class SnakeAI: public GameRunner{
private:
    int inputLayerNodes, hiddenLayers, outputLayerNodes;
    std::vector<int> eachLayer;
    MLP<Sigmoid, dSigmoid> mlp;

    void setWeights(const std::vector<long double>& modelWeights);
public:
    SnakeAI(const std::vector<int>& eachHiddenLayer, const std::vector<long double>& modelWeights, const MapItem& gameMapInfo, int length=3);

    bool nextFrame();
};

#endif