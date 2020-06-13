/* 
 * title: Snake-AI-Learner.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "Snake-AI-Learner.h"

SnakeAILearner::SnakeAILearner(std::string mapDir, const std::vector<int>& eachHiddenLayer, int population, double mutProb){
    mapItem = FileManager().readMap(mapDir);

    inputLayerNodes = 8 * 3 + 4 * 1;
    outputLayerNodes = 4;
    hiddenLayers = eachHiddenLayer.size();
    eachLayer.push_back(inputLayerNodes);
    for(int j = 0; j < hiddenLayers; ++j)
        eachLayer.push_back(eachHiddenLayer[j]);
    eachLayer.push_back(outputLayerNodes);
    for(int j = 0; j < population; ++j)
        mlps.push_back(MLP<Sigmoid, dSigmoid>(eachLayer));

    this->population = population;
    int geneLength = 0;
    for(int layer = 1; layer < eachLayer.size(); ++layer)
        geneLength += (1 + eachLayer[layer - 1]) * eachLayer[layer];
    ga.buildGA(population, geneLength, mutProb);
}
void SnakeAILearner::setWeights(){
    const std::vector<std::vector<long double>>& genes = ga.getGenes();
    for(int j = 0; j < population; ++j){
        int geneidx = 0;
        for(int layer = 1; layer < eachLayer.size(); ++layer){
            for(int p = 0; p < eachLayer[layer]; ++p){
                std::vector<long double> weights;
                for(int i = 0; i < eachLayer[layer - 1]; ++i)
                    weights.push_back(genes[j][geneidx++]);
                mlps[j].setWeights(layer, p, weights, genes[j][geneidx++]);
            }
        }
    }
}
int SnakeAILearner::runSimulation(int geneIdx){
    GameRunner gameRunner(mapItem);
    int direction = 0;

    auto getIdxOfDirection = [&](Point item, Point snake) -> int {
        if(item.x > snake.x){
            if(item.y > snake.y)
                return 4;
            else if(item.y < snake.y)
                return 2;
            else
                return 3;
        }else if(item.x < snake.x){
            if(item.y > snake.y)
                return 6;
            else if(item.y < snake.y)
                return 0;
            else
                return 7;
        }else{
            if(item.y > snake.y)
                return 5;
            else if(item.y < snake.y)
                return 1;
        }
    };
    auto getDist = [&](Point p1, Point p2) -> int {
        return abs(p1.x - p2.x) + abs(p1.y - p2.y);
    };
    int frames = 0;
    do{
        direction = gameRunner.getDirection();
        const MapManager& m = gameRunner.getMap();

        std::vector<long double> inputs(inputLayerNodes, 0);
        int growthIdx = 0;
        int poisonIdx = growthIdx + 8;
        int portalIdx = poisonIdx + 8;
        int obsIdx = portalIdx + 8;
        Point head = gameRunner.getHead();
        /*
        for(int y = 0; y < m.height; ++y){
            for(int x = 0; x < m.width; ++x){
                switch(m.get(x, y)){
                    case GROWTH: {
                        long double& distGrowth = inputs[growthIdx + getIdxOfDirection(Point(x, y), head)];
                        distGrowth = std::min(distGrowth, (long double)getDist(Point(x, y), head));
                        break;
                    }
                    case POISON: {
                        long double& distPoison = inputs[poisonIdx + getIdxOfDirection(Point(x, y), head)];
                        distPoison = std::min(distPoison, (long double)getDist(Point(x, y), head));
                        break;
                    }
                    case GATE: {
                        long double& distGate = inputs[portalIdx + getIdxOfDirection(Point(x, y), head)];
                        distGate = std::min(distGate, (long double)getDist(Point(x, y), head));
                        break;
                    }
                }
            }
        }
        */
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

        std::vector<long double> outputs = mlps[geneIdx].run(inputs);
        for(int j = 0; j < 4; ++j){
            if(outputs[j] >= 0 && outputs[j] > outputs[direction])
                direction = j;
        }
        ++frames;
    }while(gameRunner.nextFrame(direction));

    int apple = gameRunner.getStatus().getScore().scoreGrowth;
    int poison = gameRunner.getStatus().getScore().scorePoison;
    int portal = gameRunner.getStatus().getScore().scoreGate;
    int score = apple * apple - poison * poison + frames + portal * 2;
    return score;
}
long double SnakeAILearner::nextGen(){
    setWeights();
    long double bestScore = 0, bestIdx = 0;
    std::vector<long double> scores;
    for(int j = 0; j < population; ++j){
        long double meanScore = 0;

        for(int k = 0; k < ITERATION_FOR_ONE_GENE; ++k){
            long double score = runSimulation(j);
            meanScore += score;
        }
        meanScore /= (long double)ITERATION_FOR_ONE_GENE;

        if(bestScore < meanScore){
            bestScore = meanScore;
            bestIdx = j;
        }
        scores.push_back(meanScore);
    }
    bestGene.clear();
    for(int j = 0; j < ga.getGenes()[bestIdx].size(); ++j)
        bestGene.push_back(ga.getGenes()[bestIdx][j]);
    ga.nextGen(scores);
    return bestScore;
}
std::vector<long double> SnakeAILearner::getBestGene(){
    return bestGene;
}