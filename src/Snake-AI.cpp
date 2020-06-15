/* 
 * title: Snake-AI.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "Snake-AI.h"

SnakeAI::SnakeAI(const std::vector<int>& eachHiddenLayer, const std::vector<long double>& modelWeights, const MapItem& gameMapInfo, int length): GameRunner(gameMapInfo, length){
    inputLayerNodes = 8 * 3 + 4 * 1;
    outputLayerNodes = 4;
    hiddenLayers = eachHiddenLayer.size();
    eachLayer.push_back(inputLayerNodes);
    for(int j = 0; j < hiddenLayers; ++j)
        eachLayer.push_back(eachHiddenLayer[j]);
    eachLayer.push_back(outputLayerNodes);
    mlp = MLP<Sigmoid, dSigmoid>(eachLayer);

    setWeights(modelWeights);
}
void SnakeAI::setWeights(const std::vector<long double>& modelWeights){
    int geneidx = 0;
    for(int layer = 1; layer < eachLayer.size(); ++layer){
        for(int p = 0; p < eachLayer[layer]; ++p){
            std::vector<long double> weights;
            for(int i = 0; i < eachLayer[layer - 1]; ++i)
                weights.push_back(modelWeights[geneidx++]);
            mlp.setWeights(layer, p, weights, modelWeights[geneidx++]);
        }
    }
}
// To do (Sang-geon Yun): refactor majority of this class & Snake-AI-Learner
bool SnakeAI::nextFrame(){
    int nextDirection = getDirection();
    const MapManager& m = getMap();

    std::vector<long double> inputs(inputLayerNodes, 0);
    int growthIdx = 0;
    int poisonIdx = growthIdx + 8;
    int portalIdx = poisonIdx + 8;
    int obsIdx = portalIdx + 8;
    Point head = getHead();

    int itemDirection[8][2] = {{-1, -1}, {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}};
    for(int j = 0; j < 8; ++j){
        int x = head.x + itemDirection[j][0];
        int y = head.y + itemDirection[j][1];
        bool existGrowth = false;
        bool existPoison = false;
        bool existPortal = false;
        while(m.get(x, y) != ERROR && m.get(x, y) != WALL && m.get(x, y) != IMWALL && m.get(x, y) != SNAKE){
            if(m.get(x, y) == GROWTH)
                existGrowth = true;
            if(m.get(x, y) == POISON)
                existPoison = true;
            if(m.get(x, y) == GATE)
                existPortal = true;
            
            x += itemDirection[j][0];
            y += itemDirection[j][1];
        }
        if(existGrowth)
            inputs[growthIdx + j] = 1;
        if(existPoison)
            inputs[poisonIdx + j] = 1;
        if(existPortal)
            inputs[portalIdx + j] = 1;
    }
    int obsDirection[4][2] = {{-1, 0}, {1, 0}, {0, 1}, {0, -1}};
    for(int j = 0; j < 4; ++j){
        int x = head.x + obsDirection[j][0];
        int y = head.y + obsDirection[j][1];
        int dist = 0;
        while(m.get(x, y) != ERROR && m.get(x, y) != WALL && m.get(x, y) != IMWALL && m.get(x, y) != SNAKE){
            x += obsDirection[j][0];
            y += obsDirection[j][1];
            ++dist;
        }
        inputs[obsIdx + j] = dist;
    }

    std::vector<long double> outputs = mlp.run(inputs);
    for(int j = 0; j < 4; ++j)
        if(outputs[j] >= 0 && outputs[j] > outputs[nextDirection])
            nextDirection = j;
    
    return GameRunner::nextFrame(nextDirection);
}