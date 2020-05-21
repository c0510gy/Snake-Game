#include <iostream>
#include <fstream>
#include <string>
#include "Snake-Game/Snake-Game.h"
using namespace std;

int main(){
    ifstream inFile("./testMap.txt");
    Point startPoint;
    int width, height;
    inFile >> width >> height;
    MapManager myMap(width, height);
    inFile >> startPoint.x >> startPoint.y;
    for(int y = 0; y < myMap.height; ++y){
        string str; inFile >> str;
        for(int x = 0; x < myMap.width; ++x){
            int v = str[x] - '0';
            switch(v){
                case 0:
                    myMap.set(x, y, EMPTY);
                    break;
                case 1:
                    myMap.set(x, y, WALL);
                    break;
                case 2:
                    myMap.set(x, y, IMWALL);
                    break;
            }
        }
    }
    GameRunner myGame(myMap, startPoint);
    int direction = 0;
    cout << "started" << endl;
    do{
        cout << endl;
        const MapManager nowMap = myGame.getMap();
        for(int y = 0; y < nowMap.height; ++y){
            for(int x = 0; x < nowMap.width; ++x){
                switch(nowMap.get(x, y)){
                    case EMPTY:
                        cout << " ";
                        break;
                    case WALL:
                    case IMWALL:
                        cout << "#";
                        break;
                    case SNAKE:
                        cout << "@";
                        break;
                    case GROWTH:
                        cout << "+";
                        break;
                    case POISON:
                        cout << "-";
                        break;
                    case GATE:
                        cout << "O";
                        break;
                    default:
                        cout << "?";
                }
            }
            cout << endl;
        }
        cin >> direction;
    }while(myGame.nextFrame(direction));
    return 0;
}