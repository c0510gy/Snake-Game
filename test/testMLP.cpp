/* 
 * title: testMLP.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "Primitives/Sigmoid.h"
#include "MLP.h"
#include <iostream>
#include <string>
using namespace std;

// test MLP algorithm using Wine Data Set from MNIST
int main(){
    vector<int> eachLayer = {11, 15, 11};
    MLP<Sigmoid, dSigmoid> mlp(eachLayer);
    long double learningRate = 0.001;
    freopen("./winequality-red.data", "r", stdin);
    vector<vector<long double>> inputs;
    vector<int> outputs;
    string line;
    while(getline(cin, line)){
        vector<string> splitted;
        vector<long double> input;
        string tmp = "";
        for(int j = 0; j < line.size(); ++j){
            if(line[j] == ';'){
                splitted.push_back(tmp);
                tmp = "";
                continue;
            }
            tmp += line[j];
        }
        splitted.push_back(tmp);
        outputs.push_back(stoi(splitted.back()));
        for(int j = 0; j < splitted.size() - 1; ++j)
            input.push_back(stod(splitted[j]));
        inputs.push_back(input);
    }
    outputs.resize(100);
    inputs.resize(100);
    long double acc;
    for(int t = 0; t < 100000; ++t){
        acc = mlp.train(inputs, outputs, learningRate);
        cout << acc << endl;
        if(acc > 0.95)
            break;
    }
    cout << acc;

    for(int j = 0; j < inputs.size(); ++j){
        vector<long double> output = mlp.run(inputs[j]);
        cout << outputs[j] << ": ";
        for(int i = 0; i < output.size(); ++i){
            cout << output[i] << ", ";
        }cout <<endl;
    }
    return 0;
}