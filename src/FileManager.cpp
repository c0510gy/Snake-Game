/* 
 * title: FileManager.cpp
 * author: ty
 */

#include "Snake-Game/FileManager.h"
#include <iostream>

// FileManager::FileManager()
// {
// }

// void FileManager::writeMap(const MapItem &map, std::string filePath)
// {
//     outFile = std::ofstream(filePath);
//         for(int y = 0; y < map.gameMap.height; ++y){
//         std::string tmp = "";
//         for(int x = 0; x < map.gameMap.width; ++x){
//             switch(map.gameMap.get(x, y)){
//                 case EMPTY:
//                     tmp += "0";
//                     break;
//                 case WALL:
//                     tmp += "1";
//                     break;
//                 case IMWALL:
//                     tmp += "2";
//                     break;
//             }
//         }
//     }
// }

bool FileManager::isFileExist(std::string filePath){
    std::ifstream inFile(filePath);
    return inFile.is_open();
}

MapItem FileManager::readMap(std::string filePath)
{
    int width, height;
    struct MapItem items;

    inFile = std::ifstream(filePath);
    if (inFile.is_open())
    {
        std::string lineScan;
        bool mapDataflag = false;
        while (std::getline(inFile, lineScan))
        {

            int separatorIdx = lineScan.find("=");
            std::string key = lineScan.substr(0, separatorIdx);
            std::string value = lineScan.substr(separatorIdx + 1);

            if (key == "name")
                items.name = value;
            else if (key == "author")
                items.author = value;
            else if (key == "date")
                items.date = value;
            else if (key == "detail")
                items.detail = value;
            else if (key == "W")
                width = stoi(value);
            else if (key == "H")
                height = stoi(value);
            else if (key == "startPointX")
                items.startPoint.x = stoi(value);
            else if (key == "startPointY")
                items.startPoint.y = stoi(value);
            else if (key == "startDirection")
                items.startDirection = stoi(value);
            else if (key == "goalBody")
                items.goalBody = stoi(value);
            else if (key == "goalGrowth")
                items.goalGrowth = stoi(value);
            else if (key == "goalBody")
                items.goalPoison = stoi(value);
            else if (key == "goalGate")
                items.goalGate = stoi(value);
            else if (key == "MAX_SCORE_BODY")
                items.MAX_SCORE_BODY = stoi(value);
            else if (key == "mapData")
            {
                mapDataflag = true;
                break;
            }
        }
        if (mapDataflag)
        {
            MapManager myMap(width, height);
            for (int y = 0; y < height; ++y)
            {
                getline(inFile, lineScan);
                for (int x = 0; x < width; ++x)
                {
                    int v = lineScan[x] - '0';
                    switch (v)
                    {
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
            items.gameMap = myMap;
        }
        return items;
    }
    return items;
}

void FileManager::writeUser(const UserItem &user){
    std::string userFilePath = "userData/" + user.name + ".txt";
    if(isFileExist(userFilePath)){
        outFile = std::ofstream(userFilePath);
        outFile << "userID=" << user.id << "\n";
        outFile << "userName=" << user.name << "\n";
        outFile << "userHighScore=" << user.highScore;
    }
}

UserItem FileManager::readUser(std::string userId)
{
    std::string userFilePath = "./userData/" + userId + ".txt";
    UserItem user;
    if(isFileExist(userFilePath)){
        std::string lineScan;
        inFile = std::ifstream(userFilePath);
        while (std::getline(inFile, lineScan))
        {
            int separatorIdx = lineScan.find("=");
            std::string key = lineScan.substr(0, separatorIdx);
            std::string value = lineScan.substr(separatorIdx + 1);

            if (key == "userID")
                user.id = std::stoi(value);
            else if (key == "userName")
                user.name = value;
            else if (key == "userHighScore")
                user.highScore = std::stoi(value);
        }
    }
    return user;
}