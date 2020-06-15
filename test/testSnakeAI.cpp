/* 
 * title: testSnakeAI.cpp
 * author: 윤상건 - ggj06281@kookmin.ac.kr
 */

#include "Snake-AI.h"
#include "FileManager.h"
#include <iostream>
#include <vector>
#include <string>
using namespace std;

vector<long double> getGene(string gene){
    vector<long double> ret;
    vector<string> splitted;
    string tmp = "";
    for(int j = 0; j < gene.size(); ++j){
        if(gene[j] == ','){
            splitted.push_back(tmp);
            tmp = "";
            continue;
        }
        tmp += gene[j];
    }
    for(int j = 0; j < splitted.size(); ++j)
        ret.push_back(stod(splitted[j]));
    return ret;
}
int main(){
    vector<int> eachHiddenLayer = {20, 12};
    vector<long double> modelWeights;
    freopen("./ai8.txt", "r", stdin);
    string gene; cin >> gene;
    modelWeights = getGene(gene);
    SnakeAI ai(eachHiddenLayer, modelWeights, FileManager().readMap("./mapData/1.txt"));
    do{
        const MapManager& gameMap = ai.getMap();
        
        cout << endl << endl;
        for(int y = 0; y < gameMap.height; ++y){
            for(int x = 0; x < gameMap.width; ++x){
                switch(gameMap.get(x, y)){
                    case EMPTY:
                        cout << (' ');
                        break;
                    case WALL:
                    case IMWALL:
                        cout << ('#');
                        break;
                    case SNAKE:
                        cout << ('@');
                        break;
                    case GROWTH:
                        cout << ('+');
                        break;
                    case POISON:
                        cout << ('-');
                        break;
                    case GATE:
                        cout << ('O');
                        break;
                    default:
                        cout << ('?');
                }
            }
            cout << endl;
        }

        system("pause");
    }while(ai.nextFrame());
    return 0;
}