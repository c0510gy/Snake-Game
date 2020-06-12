/* 
 * title: MLP.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef MULTI_LAYER_PERCEPTRON
#define MULTI_LAYER_PERCEPTRON

#include "Primitives/Sigmoid.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>

template <class ActivationFunction, class DActivationFunction>
class Perceptron{
private:
    int numberOfWeights;
    std::vector<long double> weights;
    long double bias;

    long double yout; // Activation function을 거치지 않은 결과값
    long double error;

    long double activationFunction(long double x);
    long double dActivationFunction(long double x);
public:
    Perceptron(int numberOfWeights=0);

    long double calc(const std::vector<long double>& inputs);
    long double getCalc();

    void gradientDescent(const std::vector<long double>& inputs, long double learningRate=0.0015);
    void setError(long double error); // 현재 퍼셉트론에 대한 에러 설정
    long double getNextError(int inputIdx); // 이전 레이어의 inputIdx 퍼셉트론에 대한 에러 반환
};

template <class ActivationFunction, class DActivationFunction>
class MLP{
private:
    int inputLayerNodes, hiddenLayers, outputLayerNodes;
    std::vector<long double> inputLayer;
    std::vector<std::vector<Perceptron<ActivationFunction, DActivationFunction>>> network;

    void backpropagation(const std::vector<long double>& errors, long double learningRate=0.0015);
public:
    MLP(const std::vector<int>& eachLayer);

    std::vector<long double> run(const std::vector<long double>& inputs);
    long double train(const std::vector<std::vector<long double>>& inputDataSet, const std::vector<int>& classifyDataSet, long double learningRate=0.0015);
};

#endif