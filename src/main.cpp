#include <iostream>

#include "../include/Snake-Game/interface.h"
// for testing
#include <ncurses.h>
#define WORLD_WIDTH 50
#define WORLD_HEIGHT 20
using namespace std;
int main(int argc, char *argv[])
{
	WINDOW *snakeys_world;
    int offsetx, offsety;
 
    initscr();
    refresh();
 
    offsetx = (COLS - WORLD_WIDTH) / 2;
    offsety = (LINES - WORLD_HEIGHT) / 2;
 
    snakeys_world = newwin(WORLD_HEIGHT,
                           WORLD_WIDTH,
                           offsety,
                           offsetx);
 
    box(snakeys_world, 0 , 0);
 
    wrefresh(snakeys_world);
 
    int a = getKeyCodeFromKeyboard();
 
    delwin(snakeys_world);
 
    endwin();
 
    return 0;
	
}