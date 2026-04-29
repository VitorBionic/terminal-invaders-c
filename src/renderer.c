#include "game_types.h"
#include <unistd.h>

#define CLEAR_SCREEN "\x1b[2J"
#define CURSOR_TO_TOP "\x1b[H"
#define MENU_TITLE "TERMINAL INVADERS\n"
#define MENU_START "START\n"
#define MENU_QUIT "QUIT\n"
#define MENU_CURSOR "> "
#define MENU_NON_CURSOR "  "

static void render_menu(Game *game);

void render(Game *game) {
    write(STDOUT_FILENO, CURSOR_TO_TOP, sizeof(CURSOR_TO_TOP) - 1);
    switch (game->game_state) {
        case GAME_STATE_MENU:
            render_menu(game);
            break;
        default:
            break;
    }
}

void clear_screen() {
    write(STDOUT_FILENO, CLEAR_SCREEN, sizeof(CLEAR_SCREEN) - 1);
}

void reset_screen() {
    return;
}

static void render_menu(Game *game) {
    write(STDOUT_FILENO, MENU_TITLE, sizeof(MENU_TITLE) - 1);

    if (game->menu_selection == MENU_SLCT_START)
        write(STDOUT_FILENO, MENU_CURSOR, sizeof(MENU_CURSOR) - 1);
    else
        write(STDOUT_FILENO, MENU_NON_CURSOR, sizeof(MENU_NON_CURSOR) - 1);

    write(STDOUT_FILENO, MENU_START, sizeof(MENU_START) - 1);

    if (game->menu_selection == MENU_SLCT_QUIT)
        write(STDOUT_FILENO, MENU_CURSOR, sizeof(MENU_CURSOR) - 1);
    else
        write(STDOUT_FILENO, MENU_NON_CURSOR, sizeof(MENU_NON_CURSOR) - 1);
    write(STDOUT_FILENO, MENU_QUIT, sizeof(MENU_QUIT) - 1);
    
}
