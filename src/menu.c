#include <stdlib.h>
#include "game_types.h"

void menu_handle_action(Game *game, Action *action, double frame_scale) {
    (void)frame_scale;
    switch (*action) {
        case ACTION_UP:
            if (game->menu_selection == MENU_SLCT_QUIT)
                game->menu_selection = MENU_SLCT_START;
            break;
        case ACTION_DOWN:
            if (game->menu_selection == MENU_SLCT_START)
                game->menu_selection = MENU_SLCT_QUIT;
            break;
        case ACTION_CONFIRM:
            if (game->menu_selection == MENU_SLCT_START)
                game->game_state = GAME_STATE_PLAYING;
            else if (game->menu_selection == MENU_SLCT_QUIT)
                game->game_state = GAME_STATE_QUIT;
            break;
    }

    *action = ACTION_NONE;
}
