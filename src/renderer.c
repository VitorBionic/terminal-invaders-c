#include <stdlib.h>
#include "game_types.h"
#include <unistd.h>

#define CLEAR_SCREEN "\x1b[2J"
#define CURSOR_TO_TOP "\x1b[H"
#define HIDE_CURSOR "\x1b[?25l"
#define SHOW_CURSOR "\x1b[?25h"
#define MENU_TITLE "TERMINAL INVADERS"
#define MENU_START "START"
#define MENU_QUIT "QUIT"
#define MENU_CURSOR "> "
#define MENU_NON_CURSOR "  "
#define END_LINE "\r\n"

static void render_menu(Game *game);

static char *screen = NULL;
static int width, height;

int renderer_init(int w, int h) {
    width = w;
    height = h;
    screen = malloc((size_t)w * (size_t)h);
    if (screen == NULL)
        return 0;

    memset(screen, ' ', (size_t)w * (size_t)h);
    return 1;
}

void renderer_destroy() {
    free(screen);
}

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
    write(STDOUT_FILENO, HIDE_CURSOR, sizeof(HIDE_CURSOR) - 1);
    write(STDOUT_FILENO, CLEAR_SCREEN, sizeof(CLEAR_SCREEN) - 1);
}

void reset_screen() {
    write(STDOUT_FILENO, SHOW_CURSOR, sizeof(SHOW_CURSOR) - 1);
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
