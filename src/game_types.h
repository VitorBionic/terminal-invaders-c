#ifndef GAME_TYPES_H
#define GAME_TYPES_H

#include <stddef.h>

#define MAX_BULLETS 10

typedef enum {
    ACTION_UP,
    ACTION_DOWN,
    ACTION_LEFT,
    ACTION_RIGHT,
    ACTION_SHOOT,
    ACTION_CONFIRM,
    ACTION_COUNT,
    ACTION_NONE
} Action;

typedef enum {
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_GAME_OVER,
    GAME_STATE_QUIT
} GameState;

typedef enum {
    MENU_SLCT_START,
    MENU_SLCT_QUIT,
} MenuSelection;

typedef struct {
    unsigned int x;
    unsigned int y;
    unsigned int lives;
} Player;

typedef enum {
    BULLET_UP,
    BULLET_DOWN
} BulletDirection;

typedef struct {
    unsigned int pos_x, pos_y;
    BulletDirection direction;
} Bullet;

typedef struct {
    GameState game_state;
    MenuSelection menu_selection;

    size_t frame_count;

    unsigned int width;
    unsigned int height;

    Player player;

    Bullet bullets[MAX_BULLETS];
    unsigned int bullet_count;

} Game;

#endif
