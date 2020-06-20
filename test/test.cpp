#include <iostream>
#include <fstream>
#include <string>
#include "Snake-Game/Snake-Game.h"
#include "Snake-Game/FileManager.h"

using namespace std;

int main(){
    FileManager test;
    MapItem items = test.readMap("./mapData/2.txt");
    UserItem users = test.readUser("hi"); 
    cout << "맵 이름: " << items.name << endl << "맵 설명: " << items.detail << endl;
    cout << "사용자 정보: " << users.name << " " << users.id << " " << users.highScore << endl;
    cout << "B:" << items.goalBody << " +:" << items.goalGrowth << " -:" 
         << items.goalPoison << " G:" << items.goalGate << endl;
    test.writeMap(items, "./mapData/write.txt");
    test.scanDir("mapData");
    
//     GameRunner myGame(items.gameMap, items.startPoint);
//     int direction = 0;
//     cout << "started!!" << endl;
//     do{
//         cout << endl;
//         const MapManager nowMap = myGame.getMap();
//         for(int y = 0; y < nowMap.height; ++y){
//             for(int x = 0; x < nowMap.width; ++x){
//                 switch(nowMap.get(x, y)){
//                     case EMPTY:
//                         cout << " ";
//                         break;
//                     case WALL:
//                     case IMWALL:
//                         cout << "#";
//                         break;
//                     case SNAKE:
//                         cout << "@";
//                         break;
//                     case GROWTH:
//                         cout << "+";
//                         break;
//                     case POISON:
//                         cout << "-";
//                         break;
//                     case GATE:
//                         cout << "O";
//                         break;
//                     default:
//                         cout << "?";
//                 }
//             }
//             cout << endl;
//         }
//         cin >> direction;
//     }while(myGame.nextFrame(direction));
    return 0;
}