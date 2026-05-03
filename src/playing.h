#ifndef PLAYING_H
#define PLAYING_H

#include "game_types.h"

void playing_handle_action(Game *game, Action *action, unsigned int actions_cooldown[], double frame_scale);
void playing_update(Game *game, double frame_scale, unsigned int actions_cooldown[]);

#endif
