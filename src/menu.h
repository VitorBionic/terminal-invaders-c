#ifndef MENU_H
#define MENU_H

#include "game_types.h"

void menu_handle_action(Game *game, Action *action, double frame_scale);
void menu_update(Game *game, Action *action, double frame_scale);

#endif
