#ifndef MENU_H
#define MENU_H

#include "game_types.h"

void menu_handle_action(Game *game, Action *action, unsigned int actions_cooldown[], double frame_scale);
void menu_update(Game *game, double frame_scale, unsigned int actions_cooldown[]);

#endif
