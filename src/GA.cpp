/* 
 * title: GA.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "GA.h"

namespace GA_RANDOM{
    long double getRandomNum(){
        return ((double) rand() / (RAND_MAX)) * 2 - 1;
    }
    int getRandomNum(int s, int e){
        return rand() % (e - s + 1) + s;
    }
};

void GA::buildGA(int population, int geneLength, double mutProb){
    this->population = population;
    this->geneLength = geneLength;
    this->mutProb = mutProb;

    genes.clear();
    genes.resize(population);
    for(int j = 0; j < population; ++j)
        for(int i = 0; i < geneLength; ++i)
            genes[j].push_back(GA_RANDOM::getRandomNum() * RANDOM_RANGE);
}
GA::GA(int population, int geneLength, double mutProb){
    srand(time(NULL));
    buildGA(population, geneLength, mutProb);
}
void GA::crossOver(int i1, int i2, int i3, long double prop){
    for(int j = 0; j < geneLength; ++j){
        long double g;
        if(prop >= (GA_RANDOM::getRandomNum() + 1) / 2)
            g = genes[i2][j];
        else
            g = genes[i3][j];
        if((GA_RANDOM::getRandomNum() + 1) / 2 <= mutProb)
            g = GA_RANDOM::getRandomNum() * RANDOM_RANGE;
        genes[i1][j] = g;
    }
}
void GA::nextGen(const std::vector<long double>& scores){
    std::vector<int> idx;
    for(int j = 0; j < population; ++j) idx.push_back(j);

    std::sort(idx.begin(), idx.end(), [&](int i1, int i2) -> bool {
        return scores[i1] < scores[i2];
    });

    for(int j = 0; j < population / 2; ++j){
        int i1 = GA_RANDOM::getRandomNum(population / 2, population - 1);
        int i2;
        do{
            i2 = GA_RANDOM::getRandomNum(population / 2, population - 1);
        }while(i1 == i2);
        
        i1 = idx[i1];
        i2 = idx[i2];
        
        long double prop = 0.5;
        if(scores[i1] + scores[i2] > 0)
            prop = scores[i1] / (scores[i1] + scores[i2]);

        crossOver(idx[j], i1, i2, prop);
    }
}
const std::vector<std::vector<long double>>& GA::getGenes(){
    return genes;
}