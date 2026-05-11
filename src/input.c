#include <unistd.h>
#include <ctype.h>
#include "game_types.h"

Action input_action(GameState gs) {

    char c;

    if (read(STDIN_FILENO, &c, 1) <= 0)
        return ACTION_NONE;

    c = tolower(c);

    if (gs == GAME_STATE_MENU) {
        switch (c) {
            case 'w':
                return ACTION_UP;
            case 's':
                return ACTION_DOWN;
            case ' ':
                return ACTION_CONFIRM;
            default:
                return ACTION_NONE;
        }
    } else if (gs == GAME_STATE_PLAYING) {
        switch (c) {
            case 'a':
                return ACTION_LEFT;
            case 'd':
                return ACTION_RIGHT;
            case ' ':
                return ACTION_SHOOT;
            default:
                return ACTION_NONE;
        }
    } else if (gs == GAME_STATE_GAME_OVER) {
        switch (c) {
            case ' ':
                return ACTION_CONFIRM;
            default:
                return ACTION_NONE;
        }
    }

    return ACTION_NONE;
}
