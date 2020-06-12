/* 
 * title: MLP.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "MLP.h"

long double getRandomNum(){
    return ((double) rand() / (RAND_MAX)) * 2 - 1;
}

template <class ActivationFunction, class DActivationFunction>
Perceptron<ActivationFunction, DActivationFunction>::Perceptron(int numberOfWeights){
    this->numberOfWeights = numberOfWeights;
    bias = getRandomNum();
    weights.resize(numberOfWeights);
    for(int j = 0; j < numberOfWeights; ++j)
        weights[j] = getRandomNum();
}
template <class ActivationFunction, class DActivationFunction>
long double Perceptron<ActivationFunction, DActivationFunction>::activationFunction(long double x){
    return ActivationFunction()(x);
}template <class ActivationFunction, class DActivationFunction>
long double Perceptron<ActivationFunction, DActivationFunction>::dActivationFunction(long double x){
    return DActivationFunction()(x);
}
template <class ActivationFunction, class DActivationFunction>
long double Perceptron<ActivationFunction, DActivationFunction>::calc(const std::vector<long double>& inputs){
    yout = bias;
    for(int j = 0; j < numberOfWeights; ++j)
        yout += inputs[j] * weights[j];
    return activationFunction(yout);
}
template <class ActivationFunction, class DActivationFunction>
long double Perceptron<ActivationFunction, DActivationFunction>::getCalc(){
    return activationFunction(yout);
}
template <class ActivationFunction, class DActivationFunction>
void Perceptron<ActivationFunction, DActivationFunction>::setWeights(const std::vector<long double>& weights, int bias){
    this->bias = bias;
    for(int j = 0; j < this->weights.size(); ++j)
        this->weights[j] = weights[j];
}
template <class ActivationFunction, class DActivationFunction>
void Perceptron<ActivationFunction, DActivationFunction>::gradientDescent(const std::vector<long double>& inputs, long double learningRate){
    for(int j = 0; j < numberOfWeights; ++j){
        weights[j] -= inputs[j] * dActivationFunction(yout) * error * learningRate;
    }
    bias -= dActivationFunction(yout) * error * learningRate;
}
template <class ActivationFunction, class DActivationFunction>
void Perceptron<ActivationFunction, DActivationFunction>::setError(long double error){
    this->error = error;
}
template <class ActivationFunction, class DActivationFunction>
long double Perceptron<ActivationFunction, DActivationFunction>::getNextError(int inputIdx){
    long double ret = weights[inputIdx] * error;
    return ret;
}

template <class ActivationFunction, class DActivationFunction>
MLP<ActivationFunction, DActivationFunction>::MLP(const std::vector<int>& eachLayer){
    srand(time(NULL));

    this->inputLayerNodes = eachLayer.front();
    this->hiddenLayers = eachLayer.size() - 2;
    this->outputLayerNodes = eachLayer.back();
    network.resize(hiddenLayers + 1);
    inputLayer.resize(inputLayerNodes);

    int preNodes = inputLayerNodes;
    for(int layer = 0; layer < hiddenLayers + 1; ++layer){
        for(int p = 0; p < eachLayer[layer + 1]; ++p)
            network[layer].push_back(Perceptron<ActivationFunction, DActivationFunction>(preNodes));
        preNodes = eachLayer[layer + 1];
    }
}
template <class ActivationFunction, class DActivationFunction>
void MLP<ActivationFunction, DActivationFunction>::setWeights(int layer, int pidx, const std::vector<long double>& weights, int bias){
    network[layer - 1][pidx].setWeights(weights, bias);
}
template <class ActivationFunction, class DActivationFunction>
std::vector<long double> MLP<ActivationFunction, DActivationFunction>::run(const std::vector<long double>& inputs){
    for(int p = 0; p < inputLayerNodes; ++p)
        inputLayer[p] = inputs[p];
    for(int p = 0; p < network[0].size(); ++p)
        network[0][p].calc(inputs);
    for(int layer = 1; layer < hiddenLayers + 1; ++layer){
        std::vector<long double> preOuts;
        for(int p = 0; p < network[layer - 1].size(); ++p)
            preOuts.push_back(network[layer - 1][p].getCalc());
        for(int p = 0; p < network[layer].size(); ++p)
            network[layer][p].calc(preOuts);
    }
    std::vector<long double> outputs;
    for(int p = 0; p < outputLayerNodes; ++p){
        outputs.push_back(network.back()[p].getCalc());
    }
    return outputs;
}
template <class ActivationFunction, class DActivationFunction>
void MLP<ActivationFunction, DActivationFunction>::backpropagation(const std::vector<long double>& errors, long double learningRate){
    std::vector<long double> nextErrors(errors);
    for(int layer = hiddenLayers; layer >= 0; --layer){
        std::vector<long double> tempErrors, inputs;
        if(layer){
            tempErrors.resize(network[layer - 1].size(), 0);
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
            nextErrors[j] /= (int)network[layer].size();
    }
}
template <class ActivationFunction, class DActivationFunction>
long double MLP<ActivationFunction, DActivationFunction>::train(const std::vector<std::vector<long double>>& inputDataSet, const std::vector<int>& classifyDataSet, long double learningRatet){
    std::vector<long double> errors(outputLayerNodes, 0);
    for(int j = 0; j < inputDataSet.size(); ++j){
        std::vector<long double> outputs = run(inputDataSet[j]);
        std::vector<long double> target(outputLayerNodes, 0);
        target[classifyDataSet[j]] = 1;
        
        for(int p = 0; p < outputLayerNodes; ++p){
            errors[p] = 2 * (outputs[p] - target[p]);
        }
        
        backpropagation(errors, learningRatet);
    }
    
    long double acc = 0;
    for(int j = 0; j < inputDataSet.size(); ++j){
        std::vector<long double> outputs = run(inputDataSet[j]);
        std::vector<long double> target(outputLayerNodes, 0);
        target[classifyDataSet[j]] = 1;
        
        long double nacc = 0;
        for(int p = 0; p < outputLayerNodes; ++p)
            nacc += 1 - abs(outputs[p] - target[p]);
        acc += nacc / outputLayerNodes;
    }
    acc /= (int)inputDataSet.size();
    return acc;
}

template class Perceptron<Sigmoid, dSigmoid>;
template class MLP<Sigmoid, dSigmoid>;