#include "../include/Snake-Game/Interface.h"
#include <ncurses.h>

int getKeyCodeFromKeyboard() {
    return getch();
}