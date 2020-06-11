#include "Snake-Game/GlobalStateManager.h"

int main(int argc, char *argv[])
{
    GlobalStateManager globalStateManager;
    globalStateManager.run();
    endwin();
    return 0;
}