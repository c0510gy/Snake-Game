/* 
 * title: MapEditor.h
 * author: ty
 */

#ifndef SNAKE_GAME_MAP_EDITOR
#define SNAKE_GAME_MAP_EDITOR

#include <ncurses.h>
#include <unistd.h>
#include "Snake-Game.h"

#define WINDOW_OFFSET 3

#define SCORE_BOARD_WIDTH 15
#define SCORE_BOARD_HEIGHT 10

#define GOAL_BOARD_WIDTH 15
#define GOAL_BOARD_HEIGHT 10


class MapEditor {
public:
    MapEditor(const MapItem& map);
    ~MapEditor();
    void edit(const MapItem& map, std::string filePath);
private:
    int maxHeight, maxWidth;
    int gameMapHeight, gameMapWidth;

    void initializeWindow(const MapItem& map);
    void validateWindow(const MapItem& map);
    void drawMap(const MapItem& map);
    void initializeColors();
};

#endif

