#include "game_types.h"

#define ACTION_LEFT_CD 4
#define ACTION_RIGHT_CD 4
#define ACTION_SHOOT_CD 12

void playing_handle_action(Game *game, Action *action, unsigned int actions_cooldown[], double frame_scale) {
    switch (*action) {
        case ACTION_LEFT:
            if (actions_cooldown[ACTION_LEFT] == 0) {
                if (game->player.x > 0) {
                    game->player.x--;
                    actions_cooldown[ACTION_LEFT] = ACTION_LEFT_CD * frame_scale;
                }
            }
            break;
        case ACTION_RIGHT:
            if (actions_cooldown[ACTION_RIGHT] == 0) {
                if (game->player.x < game->width - 1) {
                    game->player.x++;
                    actions_cooldown[ACTION_RIGHT] = ACTION_RIGHT_CD * frame_scale;
                }
            }
            break;
        case ACTION_SHOOT:
            if (actions_cooldown[ACTION_SHOOT] == 0) {
                actions_cooldown[ACTION_SHOOT] = ACTION_SHOOT_CD * frame_scale;
            }
            break;
        default:
            break;
    }

    *action = ACTION_NONE;
}

void playing_update(Game *game, double frame_scale, unsigned int actions_cooldown[]) {
    int i;
    for (i = 0; i < ACTION_COUNT; i++) {
        if (actions_cooldown[i] > 0)
            actions_cooldown[i]--;
    }
    (void)game;
    (void)frame_scale;
}

