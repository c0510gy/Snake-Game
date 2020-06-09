/* 
 * title: FileManager.cpp
 * author: ty
 */

#include "Snake-Game/FileManager.h"
#include <iostream>

// FileManager::FileManager()
// {
// }

// void FileManager::writeMap(const MapItem &map, string filePath)
// {
// }

MapItem FileManager::readMap(std::string filePath)
{
    int width, height;
    struct MapItem items;

    std::ifstream inFile(filePath);
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

void FileManager::writeUser(const UserManager &user, std::string filePath)
{
    std::ifstream inFile(filePath);
    if (inFile.is_open())
    {
    }
    else{
        std::ofstream out(filePath);

    }
    
}
// UserManager FileManager::readUser(string filePath)
// {
// }