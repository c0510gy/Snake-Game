#include <iostream>
#include <fstream>
#include <string>
#include "Snake-Game/Snake-Game.h"

using namespace std;

int main(){
    FileManager test;
    MapItem items = test.readMap("./testMap2.txt");
    cout << items.name << "&" << items.author << endl;
    cout << items.date << "&" << items.detail << endl;
    GameRunner myGame(items.gameMap, items.startPoint);
    int direction = 0;
    cout << "started2" << endl;
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