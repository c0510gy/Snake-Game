/* 
 * title: MLP.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "MLP.h"

template <class ActivationFunction>
Perceptron<ActivationFunction>::Perceptron(int numberOfWeights){
    this->numberOfWeights = numberOfWeights;
    wieghts.resize(numberOfWeights);
}
template <class ActivationFunction>
long double Perceptron<ActivationFunction>::activationFunction(long double x){
    return ActivationFunction()(x);
}
template <class ActivationFunction>
long double Perceptron<ActivationFunction>::calc(const std::vector<long double>& inputs){
    out = bias;
    for(int j = 0; j < numberOfWeights; ++j)
        out += inputs[j] * weights[j];
    out = activationFunction(out);
    return out;
}
template <class ActivationFunction>
long double Perceptron<ActivationFunction>::get(){
    return out;
}

template <class ActivationFunction>
MLP<ActivationFunction>::MLP(const std::vector<int>& eachLayer){
    this->inputLayerNodes = eachLayer.front().size();
    this->hiddenLayers = eachLayer.size() - 2;
    this->outputLayerNodes = eachLayer.back().size();
    network.resize(hiddenLayers + 1);

    int preNodes = inputLayerNodes;
    for(int layer = 0; layer < hiddenLayers + 1; ++layer){
        for(int p = 0; p < eachLayer[layer]; ++p)
            network[layer].push_back(Perceptron(preNodes));
        preNodes = eachLayer[layer];
    }
}
template <class ActivationFunction>
std::vector<long double>& MLP<ActivationFunction>::run(const std::vector<long double>& inputs){
    for(int p = 0; p < network[0].size(); ++p)
        network[0][p].calc(inputs);
    for(int layer = 1; layer < hiddenLayers + 1; ++layer){
        std::vector<long double> preOuts;
        for(int p = 0; p < network[layer - 1].size(); ++p)
            preOuts.push_back(network[layer - 1][p].get());
        for(int p = 0; p < network[layer].size(); ++p)
            network[layer][p].calc(preOuts);
    }
    std::vector<long double> outputs;
    for(int p = 0; p < outputLayerNodes; ++p)
        outputs.push_back(network.back()[p].get());
    return outputs;
}