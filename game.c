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

struct game_object *ground_1;
struct game_object *ground_2;

void(init_gamescene)();
void(unload_gamescene)();
void(update_gamescene)();

int startTime;
int score;
char score_text[4];

void get_score(char s[4])
{
    score = (int)(get_game_uptime() - startTime);

    s[0] = '0' + (score / 100) % 10;
    s[1] = '0' + (score / 10) % 10;
    s[2] = '0' + (score / 1) % 10;
    s[3] = '\0';
}

void jump()
{
    if (get_game_state())
    {

        if (player->on_ground) {
            set_label_text(score_label, "BTN4 to start", true);
            unload_gamescene();
            init_gamescene();
        }
        else {
            set_game_state(false);
        }

    }
    set_entity_velocity(player, (struct vector2D){player->velocity.x, -3});
}

void go_left()
{
    set_entity_velocity(player, (struct vector2D){-3, player->velocity.y});
}

void go_right()
{
    set_entity_velocity(player, (struct vector2D){3, player->velocity.y});
}

void game_over() {
    set_label_text(score_label, "Game Over", true);
    set_game_state(true);
}

void update_gamescene()
{
    set_game_object_position(cloud, (struct vector2D){cloud->position.x + 0.2, cloud->position.y});
    
    if (lightning != NULL && lightning->active)
    {
        set_game_object_position(lightning, (struct vector2D){lightning->position.x + 0.2, lightning->position.y});
    }

    set_game_object_position(upper_pipe, (struct vector2D){upper_pipe->position.x - 0.2, upper_pipe->position.y});
    set_game_object_position(lower_pipe, (struct vector2D){lower_pipe->position.x - 0.2, lower_pipe->position.y});

    set_game_object_position(ground_1, (struct vector2D){ground_1->position.x - 0.2, ground_1->position.y});
    set_game_object_position(ground_2, (struct vector2D){ground_2->position.x - 0.2, ground_2->position.y});

    if (ground_1->position.x <= -64)
    {
        set_game_object_position(ground_1, (struct vector2D){ground_1->position.x + ground_1->width * 2, 32});
    }
    else if (ground_2->position.x <= -64)
    {
        set_game_object_position(ground_2, (struct vector2D){ground_2->position.x + ground_2->width * 2, 32});
    }

    get_score(score_text);

    if (score_label->text != score_text && !get_game_state()){
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

    if (player->on_ground) {
        game_over();
    }
}

void init_gamescene()
{
    startTime = get_game_uptime();

    button_4_click = jump;
	button_3_click = go_left;
	button_2_click = go_right;

    on_game_tick = update_gamescene;

    next_lightning = get_game_uptime() + 5;

    score_label = create_label("BTN4 to Start", (struct vector2D){64, 0}, true, false);

    player = create_entity((struct vector2D){20, 20}, icon_bird_width, icon_bird_height);
    set_entity_graphic(player, icon_bird);

    ground_1 = create_game_object((struct vector2D){64, 32}, icon_ground_width, icon_ground_height);
    ground_2 = create_game_object((struct vector2D){192, 32}, icon_ground_width, icon_ground_height);

    set_game_object_graphic(ground_1, icon_ground);
    set_game_object_graphic(ground_2, icon_ground);

    game_set_ground_level(30);

    cloud = create_game_object((struct vector2D){-10, 10}, icon_cloud_width, icon_cloud_height);
    set_game_object_graphic(cloud, icon_cloud);

    upper_pipe = create_game_object((struct vector2D){128, 7}, icon_pipe_upper_width, icon_pipe_upper_height);
    set_game_object_graphic(upper_pipe, icon_pipe_upper);

    lower_pipe = create_game_object((struct vector2D){128, 35}, icon_pipe_lower_width, icon_pipe_lower_height);
    set_game_object_graphic(lower_pipe, icon_pipe_lower);

    game_tick();
    game_draw();

    set_game_state(true);
}

void unload_gamescene()
{
    remove_entity(player);
    remove_game_object(cloud);
    remove_game_object(upper_pipe);
    remove_game_object(lower_pipe);
    remove_label(score_label);
    remove_game_object(ground_1);
    remove_game_object(ground_2);
}