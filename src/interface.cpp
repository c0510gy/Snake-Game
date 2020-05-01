#include "Snake-Game/interface.h"
#include <ncurses.h>

int getKeyCodeFromKeyboard() {
    return getch();
}