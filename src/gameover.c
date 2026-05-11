#include "game_types.h"
#include "game.h"

#define ACTION_CONFIRM_CD 12

void gameover_handle_action(Game *game, Action *action, unsigned int actions_cooldown[], double frame_scale) {
    switch (*action) {
        case ACTION_CONFIRM:
            if (actions_cooldown[ACTION_CONFIRM] == 0) {
                game->game_state = GAME_STATE_MENU;
                actions_cooldown[ACTION_CONFIRM] = ACTION_CONFIRM_CD * frame_scale;
            }
            break;
        default:
            break;
    }

    *action = ACTION_NONE;
}

void gameover_update(Game *game, double frame_scale, unsigned int actions_cooldown[]) {
    int i;
    for (i = 0; i < ACTION_COUNT; i++) {
        if (actions_cooldown[i] > 0)
            actions_cooldown[i]--;
    }
    (void)game;
    (void)frame_scale;
}

