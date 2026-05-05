#include "game_types.h"

#define ACTION_LEFT_CD 3
#define ACTION_RIGHT_CD 3
#define ACTION_SHOOT_CD 12

#define MOVE_BULLET_INTERVAL 4

static void spawn_bullet(Game *game, unsigned int x, unsigned int y, BulletDirection direction);
static void despawn_bullet(Game *game, int unsigned index);
static void move_bullets(Game *game);

void playing_handle_action(Game *game, Action *action, unsigned int actions_cooldown[], double frame_scale) {
    switch (*action) {
        case ACTION_LEFT:
            if (actions_cooldown[ACTION_LEFT] == 0) {
                if (game->player.x > 0) {
                    game->player.x--;
                    actions_cooldown[ACTION_LEFT] = ACTION_LEFT_CD * frame_scale;
                }
            }
            break;
        case ACTION_RIGHT:
            if (actions_cooldown[ACTION_RIGHT] == 0) {
                if (game->player.x < game->width - 1) {
                    game->player.x++;
                    actions_cooldown[ACTION_RIGHT] = ACTION_RIGHT_CD * frame_scale;
                }
            }
            break;
        case ACTION_SHOOT:
            if (actions_cooldown[ACTION_SHOOT] == 0) {
                actions_cooldown[ACTION_SHOOT] = ACTION_SHOOT_CD * frame_scale;
                spawn_bullet(game, game->player.x, game->player.y - 1, BULLET_UP);
            }
            break;
        default:
            break;
    }

    *action = ACTION_NONE;
}

void playing_update(Game *game, double frame_scale, unsigned int actions_cooldown[]) {
    int i;
    for (i = 0; i < ACTION_COUNT; i++) {
        if (actions_cooldown[i] > 0)
            actions_cooldown[i]--;
    }

    game->frame_count++;

    unsigned int interval = MOVE_BULLET_INTERVAL * frame_scale;
    if (interval == 0)
        interval = 1;
    if (game->frame_count % interval == 0)
        move_bullets(game);

}

static void move_bullets(Game *game) {
    unsigned int i;
    i = 0;

    while (i < game->bullet_count) {
        if (game->bullets[i].direction == BULLET_UP) {
            if (game->bullets[i].pos_y == 0)
                despawn_bullet(game, i);
            else {
                game->bullets[i].pos_y--;
                i++;
            }
        } else if (game->bullets[i].direction == BULLET_DOWN) {
            if (game->bullets[i].pos_y == game->height - 1)
                despawn_bullet(game, i);
            else {
                game->bullets[i].pos_y++;
                i++;
            }
        } else
            i++;
    }
}

static void spawn_bullet(Game *game, unsigned int x, unsigned int y, BulletDirection direction) {
    if (game->bullet_count < MAX_BULLETS) {
        Bullet *b = &game->bullets[game->bullet_count++];

        b->pos_x = x;
        b->pos_y = y;
        b->direction = direction;

    }
}

static void despawn_bullet(Game *game, unsigned int index) {
    if (game->bullet_count > 0 && index < game->bullet_count) {
        game->bullets[index] = game->bullets[game->bullet_count - 1];
        game->bullet_count--;
    }
}
