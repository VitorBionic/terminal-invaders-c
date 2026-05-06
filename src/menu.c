#include "game_types.h"
#include "game.h"

#define ACTION_UP_CD 8
#define ACTION_DOWN_CD 8
#define ACTION_CONFIRM_CD 12

static void change_menu_selection(Game *game, Action action);

void menu_handle_action(Game *game, Action *action, unsigned int actions_cooldown[], double frame_scale) {
    switch (*action) {
        case ACTION_UP:
            if (actions_cooldown[ACTION_UP] == 0) {
                change_menu_selection(game, *action);
                actions_cooldown[ACTION_UP] = ACTION_UP_CD * frame_scale;
            }
            break;
        case ACTION_DOWN:
            if (actions_cooldown[ACTION_DOWN] == 0) {
                change_menu_selection(game, *action);
                actions_cooldown[ACTION_DOWN] = ACTION_DOWN_CD * frame_scale;
            }
            break;
        case ACTION_CONFIRM:
            if (actions_cooldown[ACTION_CONFIRM] == 0) {
                change_menu_selection(game, *action);
                actions_cooldown[ACTION_CONFIRM] = ACTION_CONFIRM_CD * frame_scale;
            }
            break;
        default:
            break;
    }

    *action = ACTION_NONE;
}

void menu_update(Game *game, double frame_scale, unsigned int actions_cooldown[]) {
    int i;
    for (i = 0; i < ACTION_COUNT; i++) {
        if (actions_cooldown[i] > 0)
            actions_cooldown[i]--;
    }
    (void)game;
    (void)frame_scale;
}

static void change_menu_selection(Game *game, Action action) {
    switch (action) {
        case ACTION_UP:
            if (game->menu_selection == MENU_SLCT_QUIT)
                game->menu_selection = MENU_SLCT_START;
            break;
        case ACTION_DOWN:
            if (game->menu_selection == MENU_SLCT_START)
                game->menu_selection = MENU_SLCT_QUIT;
            break;
        case ACTION_CONFIRM:
            if (game->menu_selection == MENU_SLCT_START) {
                game->game_state = GAME_STATE_PLAYING;
                start_game(game);
            } else if (game->menu_selection == MENU_SLCT_QUIT)
                game->game_state = GAME_STATE_QUIT;
            break;
        default:
            break;
    }
}
