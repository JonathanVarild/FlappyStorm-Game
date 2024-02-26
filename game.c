#include "gameengine.h"
#include <stdbool.h>
#include "icons.h"

struct label *score_label;
struct entity *player;
struct game_object *cloud;
struct game_object *lightning;
double next_lightning;

struct game_object *lower_pipe;
struct game_object *upper_pipe;

void jump()
{
    set_entity_velocity(player, (struct vector2D){player->velocity.x, -3});
}

void go_left()
{
    set_entity_velocity(player, (struct vector2D){-5, player->velocity.y});
}

void go_right()
{
    set_entity_velocity(player, (struct vector2D){5, player->velocity.y});
}

int score;
char score_text[4];

void get_score(char s[4])
{
    score = (int)(get_game_uptime());

    s[0] = '0' + (score / 100) % 10;
    s[1] = '0' + (score / 10) % 10;
    s[2] = '0' + (score / 1) % 10;
    s[3] = '\0';
}

void update_gamescene()
{
    set_game_object_position(cloud, (struct vector2D){cloud->position.x + 0.1, cloud->position.y});
    set_game_object_position(upper_pipe, (struct vector2D){upper_pipe->position.x - 0.1, upper_pipe->position.y});
    set_game_object_position(lower_pipe, (struct vector2D){lower_pipe->position.x - 0.1, lower_pipe->position.y});

    get_score(score_text);

    if (score_label->text != score_text) {
        set_label_text(score_label, score_text, true);
    }

    if (get_game_uptime() > next_lightning)
    {
        next_lightning = get_game_uptime() + 5;
        lightning = create_game_object((struct vector2D){cloud->position.x + icon_lightning_width / 2, cloud->position.y + icon_lightning_height + 3}, icon_lightning_width, icon_lightning_height);
        set_game_object_graphic(lightning, icon_lightning);
    }

    if (lightning != NULL && lightning->age + 0.3 < get_game_uptime())
    {
        remove_game_object(lightning);
    }
}

void init_gamescene()
{
    button_4_click = jump;
	button_3_click = go_left;
	button_2_click = go_right;

    on_game_tick = update_gamescene;

    next_lightning = get_game_uptime() + 5;

    score_label = create_label("0000", (struct vector2D){64, 0}, true, false);

    player = create_entity((struct vector2D){0, 20}, icon_bird_width, icon_bird_height);
    set_entity_graphic(player, icon_bird);
    set_entity_velocity(player, (struct vector2D){10, 0});

    cloud = create_game_object((struct vector2D){-10, 20}, icon_cloud_width, icon_cloud_height);
    set_game_object_graphic(cloud, icon_cloud);

    upper_pipe = create_game_object((struct vector2D){128, 7}, icon_pipe_upper_width, icon_pipe_upper_height);
    set_game_object_graphic(upper_pipe, icon_pipe_upper);

    lower_pipe = create_game_object((struct vector2D){128, 35}, icon_pipe_lower_width, icon_pipe_lower_height);
    set_game_object_graphic(lower_pipe, icon_pipe_lower);
}

void unload_gamescene()
{
    remove_entity(player);
}