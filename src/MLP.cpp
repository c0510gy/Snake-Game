/* 
 * title: MLP.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "MLP.h"

template <class ActivationFunction, class dActivationFunction>
Perceptron<ActivationFunction, dActivationFunction>::Perceptron(int numberOfWeights){
    this->numberOfWeights = numberOfWeights;
    wieghts.resize(numberOfWeights);
}
template <class ActivationFunction, class dActivationFunction>
long double Perceptron<ActivationFunction, dActivationFunction>::activationFunction(long double x){
    return ActivationFunction()(x);
}template <class ActivationFunction, class dActivationFunction>
long double Perceptron<ActivationFunction, dActivationFunction>::dActivationFunction(long double x){
    return dActivationFunction()(x);
}
template <class ActivationFunction, class dActivationFunction>
long double Perceptron<ActivationFunction, dActivationFunction>::calc(const std::vector<long double>& inputs){
    out = bias;
    for(int j = 0; j < numberOfWeights; ++j)
        out += inputs[j] * weights[j];
    out = activationFunction(out);
    return out;
}
template <class ActivationFunction, class dActivationFunction>
long double Perceptron<ActivationFunction, dActivationFunction>::getCalc(){
    return out;
}
template <class ActivationFunction, class dActivationFunction>
void Perceptron<ActivationFunction, dActivationFunction>::gradientDescent(const std::vector<long double>& inputs, long double learningRate){
    for(int j = 0; j < numberOfWeights; ++j)
        weights[j] -= inputs[j] * dActivationFunction(getCalc()) * error * learningRate;
    bias -= dActivationFunction(getCalc()) * error * learningRate;
}
template <class ActivationFunction, class dActivationFunction>
void Perceptron<ActivationFunction, dActivationFunction>::setError(long double error){
    this->error = error;
}
template <class ActivationFunction, class dActivationFunction>
long double Perceptron<ActivationFunction, dActivationFunction>::getNextError(int inputIdx){
    return weights[inputIdx] * dActivationFunction(getCalc()) * error;
}

template <class ActivationFunction, class dActivationFunction>
MLP<ActivationFunction, dActivationFunction>::MLP(const std::vector<int>& eachLayer){
    this->inputLayerNodes = eachLayer.front().size();
    this->hiddenLayers = eachLayer.size() - 2;
    this->outputLayerNodes = eachLayer.back().size();
    network.resize(hiddenLayers + 1);
    inputLayer.resize(inputLayerNodes);

    int preNodes = inputLayerNodes;
    for(int layer = 0; layer < hiddenLayers + 1; ++layer){
        for(int p = 0; p < eachLayer[layer]; ++p)
            network[layer].push_back(Perceptron(preNodes));
        preNodes = eachLayer[layer];
    }
}
template <class ActivationFunction, class dActivationFunction>
std::vector<long double>& MLP<ActivationFunction, dActivationFunction>::run(const std::vector<long double>& inputs){
    for(int p = 0; p < inputLayerNodes; ++p)
        inputLayer[p] = inputs[p];
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
        outputs.push_back(network.back()[p].getCalc());
    return outputs;
}
template <class ActivationFunction, class dActivationFunction>
void MLP<ActivationFunction, dActivationFunction>::backpropagation(const std::vector<long double>& errors, long double learningRate){
    std::vector<long double> nextErrors = errors;
    for(int layer = hiddenLayers; layer >= 0; --layer){
        std::vector<long double> tempErrors, inputs;
        if(layer){
            tempErrors.resize(network[layer - 1].size());
            for(int p = 0; p < network[layer - 1].size(); ++p)
                inputs.push_back(network[layer - 1][p].getCalc());
        }else{
            for(int p = 0; p < inputLayerNodes; ++p)
                inputs.push_back(inputLayer[p]);
        }
        for(int p = 0; p < network[layer].size(); ++p){
            network[layer][p].setError(nextErrors[p]);

            if(layer)
                for(int np = 0; np < network[layer - 1].size(); ++np)
                    tempErrors[np] += network[layer][p].getNextError(np);
            
            network[layer][p].gradientDescent(inputs, learningRate);
        }
        nextErrors = tempErrors;
        for(int j = 0; j < nextErrors.size(); ++j)
            nextErrors[j] /= network[layer].size();
    }
}