/* 
 * title: FileManager.cpp
 * author: ty
 */

#include "Snake-Game/FileManager.h"
using namespace std;

// FileManager::FileManager()
// {
// }

void FileManager::writeMap(const MapItem &map, string filePath)
{
}

MapItem FileManager::readMap(string filePath)
{
    int width, height;
    MapItem items;

    ifstream inFile(filePath);
    if (inFile.is_open())
    {
        string lineScan;
        bool mapDataflag = false;
        while (getline(inFile, lineScan))
        {
            if (!mapDataflag)
            {
                int separatorIdx = lineScan.find("=");
                string key = lineScan.substr(0, separatorIdx);
                string value = lineScan.substr(separatorIdx + 1);
                if (key == "name")
                    items.name = value;
                else if (key == "author")
                    items.author = value;
                else if (key == "date")
                    items.date = value;
                else if (key == "detail")
                    items.detail = value;
                if (key == "W")
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
}

void FileManager::writeUser(const UserManager &user, string filePath)
{
}
UserManager FileManager::readUser(string filePath)
{
}