/* 
 * title: Primitives/MapItem.h
 * author: 
 */

#ifndef SNAKE_MAP_ITEM
#define SNAKE_MAP_ITEM

#include "Primitives/MapManager.h"
#include <string>
#include <vector>

struct MapItem{
    std::string name;
    std::string author;
    std::string date;
    std::string detail;

    int goalBody;
    int goalGrowth;
    int goalPoison;
    int goalGate;
    int maxBodyScore;
    int startDirection;

    MapManager gameMap;
    Point startPoint;
};

#endif