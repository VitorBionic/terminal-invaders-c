#include "game_types.h"
#include "game.h"

#define ACTION_LEFT_CD 3
#define ACTION_RIGHT_CD 3
#define ACTION_SHOOT_CD 30

#define MOVE_BULLET_INTERVAL 4

static void speed_up(Game *game);
static void spawn_bullet(Game *game, unsigned int x, unsigned int y, BulletDirection direction);
static void despawn_bullet(Game *game, int unsigned index);
static void move_bullets(Game *game);
static void move_enemies(Game *game);
static void despawn_enemy(Game *game, unsigned int index);
static void handle_bullet_collisions(Game *game);
static void check_game_over(Game *game);

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

    interval = game->enemy_move_interval * frame_scale;
        if (interval == 0)
            interval = 1;
    if (game->frame_count % interval == 0)
        move_enemies(game);

    handle_bullet_collisions(game);

    check_game_over(game);

}

static void check_game_over(Game *game) {
    if (game->player.lives == 0)
        game->game_state = GAME_STATE_GAME_OVER;
    else if (game->enemy_count == 0)
        start_game(game);
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

static void despawn_enemy(Game *game, unsigned int index) {
    if (game->enemy_count > 0 && index < game->enemy_count) {
        game->enemies[index] = game->enemies[game->enemy_count - 1];
        game->enemy_count--;
    }
}

static void move_enemies(Game *game) {
    if (game->enemy_count == 0)
        return;

    unsigned int i;

    if (game->enemy_direction == ENEMY_DIRECTION_LEFT) {
        Enemy most_left_enemy = game->enemies[0];
        for (i = 1; i < game->enemy_count; i++) {
            if (game->enemies[i].x < most_left_enemy.x) 
                most_left_enemy = game->enemies[i];
        }

        if (most_left_enemy.x <= 1) {
            for (i = 0; i < game->enemy_count; i++) {
                if (game->enemies[i].y < game->height - 1) {
                    game->enemies[i].y++;
                    if (game->enemies[i].y == game->player.y)
                        game->player.lives = 0;
                }
            }
            game->enemy_direction = ENEMY_DIRECTION_RIGHT;
        } else {
            for (i = 0; i < game->enemy_count; i++)
                game->enemies[i].x--;
        }
    } else if (game->enemy_direction == ENEMY_DIRECTION_RIGHT) {
        Enemy most_right_enemy = game->enemies[0];
        for (i = 1; i < game->enemy_count; i++) {
            if (game->enemies[i].x > most_right_enemy.x) 
                most_right_enemy = game->enemies[i];
        }

        if (most_right_enemy.x >= game->width - 1) {
            for (i = 0; i < game->enemy_count; i++) {
                if (game->enemies[i].y < game->height - 1) {
                    game->enemies[i].y++;
                    if (game->enemies[i].y == game->player.y)
                        game->player.lives = 0;
                }
            }
            game->enemy_direction = ENEMY_DIRECTION_LEFT;
        } else {
            for (i = 0; i < game->enemy_count; i++)
                game->enemies[i].x++;
        }
    }

}

static void handle_bullet_collisions(Game *game) {
    unsigned int i, j, collision;
    
    i = 0;
    while (i < game->bullet_count) {
        if (game->bullets[i].direction == BULLET_UP) {
            collision = 0;
            j = 0;
            while (j < game->enemy_count) {
                if (game->bullets[i].pos_x == game->enemies[j].x && game->bullets[i].pos_y == game->enemies[j].y) {
                    despawn_bullet(game, i);
                    despawn_enemy(game, j);
                    collision = 1;
                    speed_up(game);
                    break;
                }
                j++;
            }

            if (!collision)
                i++;
        } else if (game->bullets[i].direction == BULLET_DOWN) {
            if (game->bullets[i].pos_x == game->player.x && game->bullets[i].pos_y == game->player.y) {
                if (game->player.lives > 0) {
                    game->player.lives--;
                    game->player.x = 0;
                }
                despawn_bullet(game, i);
            } else
                i++;
        }
    }
}

static void speed_up(Game *game) {
    if (game->enemy_count == 39)
        game->enemy_move_interval = 24;
    else if (game->enemy_count == 29)
        game->enemy_move_interval = 18;
    else if (game->enemy_count == 19)
        game->enemy_move_interval = 12;
    else if (game->enemy_count == 9)
        game->enemy_move_interval = 8;
    else if (game->enemy_count == 4)
        game->enemy_move_interval = 5;
}
