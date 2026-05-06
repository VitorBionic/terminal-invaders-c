#ifndef GAME_H
#define GAME_H

#include "game_types.h"

void game_loop(void);
void game_request_quit(int sig);
void start_game(Game *game);

#endif
