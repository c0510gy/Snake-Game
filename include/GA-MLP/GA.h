/* 
 * title: GA.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef GENETIC_ALGORITHM
#define GENETIC_ALGORITHM

#include <algorithm>
#include <vector>
// For random
#include <cstdlib>
#include <ctime>

class GA{
private:
    const int RANDOM_RANGE = 10;
    
    int population;
    int geneLength;
    double mutProb;
    std::vector<std::vector<long double>> genes;

    void crossOver(int i1, int i2, int i3, long double prop); // i1 <- 교배(i2, i3)
public:
    GA(int population=20, int geneLength=10, double mutProb=0.05);

    void nextGen(const std::vector<long double>& scores);
    const std::vector<std::vector<long double>>& getGenes();
};

#endif