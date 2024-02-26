#include "gameengine.h"
#include <stdbool.h>
#include "icons.h"

struct entity *player;
struct game_object *cloud;

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

void update_gamescene()
{
    set_game_object_position(cloud, (struct vector2D){cloud->position.x + 0.1, cloud->position.y});
}

void init_gamescene()
{
    button_4_click = jump;
	button_3_click = go_left;
	button_2_click = go_right;

    on_game_tick = update_gamescene;

    player = create_entity((struct vector2D){0, 20}, icon_bird_width, icon_bird_height);
    set_entity_graphic(player, icon_bird);
    set_entity_velocity(player, (struct vector2D){10, 0});

    cloud = create_game_object((struct vector2D){-10, 20}, icon_cloud_width, icon_cloud_height);
    set_game_object_graphic(cloud, icon_cloud);
}

void unload_gamescene()
{
    remove_entity(player);
}