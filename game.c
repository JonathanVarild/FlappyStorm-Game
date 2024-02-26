#include "./gameengine.h"
#include <stdbool.h>
#include "icons.h"

struct entity *player;

void jump()
{
    set_entity_velocity(player, (struct vector2D){0, -3});
}

void go_left()
{
    set_entity_velocity(player, (struct vector2D){-1, player->velocity.y});
}

void go_right()
{
    set_entity_velocity(player, (struct vector2D){1, player->velocity.y});
}

void init_gamescene()
{
    button_4_click = jump;
	button_3_click = go_left;
	button_2_click = go_right;

    struct label *test_lbl = create_label("FlappyStorm", (struct vector2D){64, 0}, true, true);

    player = create_entity((struct vector2D){0, 20}, 10, 8);
    set_entity_graphic(player, icon_bird);
    set_entity_velocity(player, (struct vector2D){10, 0});
}