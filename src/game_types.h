#ifndef GAME_TYPES_H
#define GAME_TYPES_H

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
    GameState game_state;
    MenuSelection menu_selection;
    unsigned int width;
    unsigned int height;
    unsigned int player_x;
    unsigned int player_y;

} Game;

#endif
