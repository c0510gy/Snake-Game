/* 
 * title: MapEditor.h
 * author: ty
 */

#ifndef SNAKE_GAME_MAP_EDITOR
#define SNAKE_GAME_MAP_EDITOR

#include <ncurses.h>
#include <unistd.h>
#include "Snake-Game.h"
#include "FileManager.h"

#define WINDOW_OFFSET 3

class MapEditor {
public:
    MapEditor(const MapItem& map);
    ~MapEditor();
    void edit(MapItem& map);
private:
    int maxHeight, maxWidth;
    int gameMapHeight, gameMapWidth;

    void initializeWindow(const MapItem& map);
    void validateWindow(const MapItem& map);
    void initDrawMap(const MapManager& objMap);
    void initializeColors();
};

#endif

