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
    MapEditor(MapItem& map);
    ~MapEditor();

    void edit();
    void showInputWindow();
private:
    int maxHeight, maxWidth;
    int gameMapHeight, gameMapWidth;

    MapItem userMapItem;

    WINDOW* windowInfoInput;

    void initializeWindow();
    void validateWindow();
    void initDrawMap();
    void initSetMap();
    void initializeColors();
    void autoSetWall();
};

#endif

