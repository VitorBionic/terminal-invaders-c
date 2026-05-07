#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <time.h>
#include "util.h"
#include "input.h"
#include "game_types.h"
#include "menu.h"
#include "playing.h"
#include "renderer.h"

#define FPS 60
#define GAME_WIDTH 50
#define GAME_HEIGHT 20
#define PLAYER_START_LIVES 3
#define ENEMIES_LINES 5
#define ENEMIES_PER_LINE 11
#define V_FRONTLINE 0
#define W_FRONTLINE 2

static int sleep_time(struct timespec start, struct timespec end, struct timespec fr_time, struct timespec *diffp);
void game_request_quit(int sig);
static void update(Game *game, Action *action, double frame_scale);

static volatile sig_atomic_t stop = 0;

static unsigned int actions_cooldown[ACTION_COUNT];

void game_loop() {
    struct timespec target_frame_time;
    struct timespec start_frame;
    struct timespec end_frame;
    struct timespec diff_frame;

    Game game;
    game.game_state = GAME_STATE_MENU;
    game.menu_selection = MENU_SLCT_START;
    Action frame_action = ACTION_NONE;
    game.width = GAME_WIDTH;
    game.height = GAME_HEIGHT;

    if (!renderer_init(game.width, game.height))
        game_request_quit(0);
    
    if (fps_to_timespec(FPS, &target_frame_time) == -1)
        game_request_quit(0);

    double frame_scale = FPS / 60.0;

    clear_screen();

    while (!stop && game.game_state != GAME_STATE_QUIT) {
        if (clock_gettime(CLOCK_MONOTONIC, &start_frame) == -1)
            break;

        frame_action = input_action(game.game_state);
        update(&game, &frame_action, frame_scale);
        render(&game);

        if (clock_gettime(CLOCK_MONOTONIC, &end_frame) == -1)
            break;

        if (sleep_time(start_frame, end_frame, target_frame_time, &diff_frame) == -1)
            break;
        nanosleep(&diff_frame, NULL);
        
    }

    reset_screen();
    renderer_destroy();
}

void game_request_quit(int sig) {
    (void)sig;
    stop = 1;
}

void start_game(Game *game) {
    game->frame_count = 0;
    game->player.x = 0;
    game->player.y = game->height - 1;
    game->player.lives = PLAYER_START_LIVES;
    game->bullet_count = 0;

    int i, j, cur;
    cur = 0;
    for (i = 0; i < ENEMIES_LINES; i++) {
        for (j = 0; j < ENEMIES_PER_LINE; j++) {
                Enemy enemy;
                enemy.x = j * 2 + 1;
                enemy.y = i;
            if (i <= V_FRONTLINE)
                enemy.type = ENEMY_TYPE_V;
            else if (i <= W_FRONTLINE)
                enemy.type = ENEMY_TYPE_W;
            else
                enemy.type = ENEMY_TYPE_M;
            game->enemies[cur] = enemy;
            ++cur;
        }
    }

    game->enemy_count = cur;
    game->enemy_direction = ENEMY_DIRECTION_RIGHT;

    for (i = 0; i < ACTION_COUNT; i++)
        actions_cooldown[i] = 0;
}

static int sleep_time(struct timespec start, struct timespec end, struct timespec fr_time, struct timespec *diffp) {
    struct timespec diff;

    if (timespec_diff(end, start, &diff) == -1)
        return -1;
    
    if (timespec_diff(fr_time, diff, diffp) == -1) {
        diffp->tv_sec = 0;
        diffp->tv_nsec = 0;
    }

    return 0;
}

static void update(Game *game, Action *action, double frame_scale) {
    if (*action != ACTION_NONE) {
        switch (game->game_state) {
            case GAME_STATE_MENU:
                menu_handle_action(game, action, actions_cooldown, frame_scale);
                break;
            case GAME_STATE_PLAYING:
                playing_handle_action(game, action, actions_cooldown, frame_scale);
                break;
            default:
                break;
        }
    }

    switch (game->game_state) {
        case GAME_STATE_MENU:
            menu_update(game, frame_scale, actions_cooldown);
            break;
        case GAME_STATE_PLAYING:
            playing_update(game, frame_scale, actions_cooldown);
            break;
        default:
            break;
    }

}
