/* 
 * title: Primitives/Sigmoid.h
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#ifndef SIGMOID_FUNCTION
#define SIGMOID_FUNCTION

#include <cmath>

struct Sigmoid{
    long double operator()(long double x){
        return (long double)1 / (1 + exp(-x));
    }
};
struct dSigmoid{
    long double operator()(long double x){
        return Sigmoid()(x) * (1 - Sigmoid()(x));
    }
};

#endif