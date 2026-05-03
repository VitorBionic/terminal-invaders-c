#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "game_types.h"

#define IDX(row, col) ((row) * width + (col))
#define CLEAR_SCREEN "\x1b[2J"
#define CURSOR_TO_TOP "\x1b[H"
#define HIDE_CURSOR "\x1b[?25l"
#define SHOW_CURSOR "\x1b[?25h"
#define MENU_TITLE "TERMINAL INVADERS"
#define MENU_START "START"
#define MENU_QUIT "QUIT"
#define END_ROW "\r\n"

static void build_menu_screen(Game *game);
static void build_playing_screen(Game *game);
static void render_screen();
static int find_center_row(int lines);
static int find_center_col(int len);

static char *screen = NULL;
static int width, height;

int renderer_init(int w, int h) {
    width = w;
    height = h;
    screen = malloc((size_t)w * (size_t)h);
    if (screen == NULL)
        return 0;

    return 1;
}

void renderer_destroy() {
    free(screen);
}

void render(Game *game) {
    memset(screen, ' ', (size_t)width * (size_t)height);
    write(STDOUT_FILENO, CURSOR_TO_TOP, sizeof(CURSOR_TO_TOP) - 1);
    switch (game->game_state) {
        case GAME_STATE_MENU:
            build_menu_screen(game);
            break;
        case GAME_STATE_PLAYING:
            build_playing_screen(game);
            break;
        default:
            break;
    }
    render_screen();
}

void clear_screen() {
    write(STDOUT_FILENO, HIDE_CURSOR, sizeof(HIDE_CURSOR) - 1);
    write(STDOUT_FILENO, CLEAR_SCREEN, sizeof(CLEAR_SCREEN) - 1);
}

void reset_screen() {
    write(STDOUT_FILENO, SHOW_CURSOR, sizeof(SHOW_CURSOR) - 1);
}

static void build_menu_screen(Game *game) {

    const char *cp = MENU_TITLE;
    int center_col = find_center_col(sizeof(MENU_TITLE));
    int center_row = find_center_row(4);

    if (center_row == -1 || center_col == -1)
        return;

    while (*cp) {
        screen[IDX(center_row, center_col++)] = *cp;
        cp++;
    }
    center_row += 2;
    
    cp = MENU_START;
    center_col = find_center_col(sizeof(MENU_START) + 2);
    if (center_row == -1 || center_col == -1)
        return;

    if (game->menu_selection == MENU_SLCT_START)
        screen[IDX(center_row, center_col++)] = '>';
    else
        screen[IDX(center_row, center_col++)] = ' ';

    screen[IDX(center_row, center_col++)] = ' ';

    while (*cp) {
        screen[IDX(center_row, center_col++)] = *cp;
        cp++;
    }
    center_row++;

    cp = MENU_QUIT;
    center_col = find_center_col(sizeof(MENU_QUIT) + 2);
    if (center_row == -1 || center_col == -1)
        return;

    if (game->menu_selection == MENU_SLCT_QUIT)
        screen[IDX(center_row, center_col++)] = '>';
    else
        screen[IDX(center_row, center_col++)] = ' ';

    screen[IDX(center_row, center_col++)] = ' ';

    while (*cp) {
        screen[IDX(center_row, center_col++)] = *cp;
        cp++;
    }
        
}

static void build_playing_screen(Game *game) {
    screen[IDX(game->player.y, game->player.x)] = 'A';
}

static int find_center_row(int lines) {

    if (lines > height)
        return -1;

    return (height - lines) / 2;
}


static int find_center_col(int len) {

    if (len > width)
        return -1;

    return (width - len) / 2;
}

static void render_screen() {

    int i, len;
    char *screenp = screen;
    len = width * height;

    i = 0;
    while (++i <= len) {
        write(STDOUT_FILENO, screenp++, 1);
        if (i % width == 0)
            write(STDOUT_FILENO, END_ROW, sizeof(END_ROW) - 1);
    }
    write(STDOUT_FILENO, END_ROW, sizeof(END_ROW) - 1);
}

