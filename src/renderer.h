#ifndef RENDERER_H
#define RENDERER_H

#include "game_types.h"

int renderer_init(int w, int h);
void renderer_destroy();
void render(Game *game);
void clear_screen();
void reset_screen();

#endif
