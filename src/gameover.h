#ifndef GAMEOVER_H
#define GAMEOVER_H

#include "game_types.h"

void gameover_handle_action(Game *game, Action *action, unsigned int actions_cooldown[], double frame_scale);
void gameover_update(Game *game, double frame_scale, unsigned int actions_cooldown[]);

#endif
