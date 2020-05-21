/* 
 * title: Primitives/MapManager.h
 * 
 */

#ifndef SNAKE_MAP_MANAGER
#define SNAKE_MAP_MANAGER

#include "Primitives/Item.h"
#include "Primitives/Point.h"
#include <vector>

struct MapManager{
    int width, height;
    std::vector<std::vector<Item>> gameMap;

    MapManager(int width=0, int height=0){
        this->width = width;
        this->height = height;
        gameMap.resize(height);
        for(int y = 0; y < height; ++y)
            gameMap[y].resize(width, EMPTY);
    }
    Item get(int x, int y) const{
        if(x < 0 || y < 0 || x >= width || y >= height)
            return ERROR;
        return gameMap[y][x];
    }
    Item get(Point p) const{
        return get(p.x, p.y);
    }
    void set(int x, int y, Item v){
        if(x < 0 || y < 0 || x >= width || y >= height)
            return;
        gameMap[y][x] = v;
    }
    void set(Point p, Item v){
        set(p.x, p.y, v);
    }
};

#endif