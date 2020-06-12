/* 
 * title: MLP.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef MULTI_LAYER_PERCEPTRON
#define MULTI_LAYER_PERCEPTRON

#include "Primitives/Sigmoid.h"
#include <vector>
#include <cmath>

template <class ActivationFunction>
class Perceptron{
private:
    int numberOfWeights;
    std::vector<long double> weights;
    long double bias;

    long double out;

    long double activationFunction(long double x);
public:
    Perceptron(int numberOfWeights=0);

    long double calc(const std::vector<long double>& inputs);
    long double get();
};

template <class ActivationFunction>
class MLP{
private:
    int inputLayerNodes, hiddenLayers, outputLayerNodes;
    std::vector<std::vector<Perceptron<ActivationFunction>>> network;

public:
    MLP(const std::vector<int>& eachLayer);

    std::vector<long double>& run(const std::vector<long double>& inputs);
};

#endif