#include "gameengine.h"
#include <stdbool.h>
#include "icons.h"

// Give entities/labels/game objects a bigger scope.
struct label *score_label;
struct entity *player;

struct game_object *cloud;

struct game_object *lightning;
double next_lightning;

struct game_object *lower_pipe;
struct game_object *upper_pipe;

struct game_object *ground_1;
struct game_object *ground_2;

// Give these functions a bigger scope.
void(init_gamescene)();
void(unload_gamescene)();
void(update_gamescene)();

// Variables used in game scene.
int startTime;
int score;
char score_text[4];

// Function to get the current score.
void get_score(char s[4])
{
    // Calculate the score.
    score = (int)(get_game_uptime() - startTime);

    // Convert the score to a string.
    s[0] = '0' + (score / 100) % 10;
    s[1] = '0' + (score / 10) % 10;
    s[2] = '0' + (score / 1) % 10;
    s[3] = '\0';
}

// Function to make the player jump and start game.
void jump()
{
    // Check if the game is paused.
    if (get_game_state())
    {
        // Check if the player is on the ground.
        if (player->on_ground) {
            // Reset the game scene.
            unload_gamescene();
            init_gamescene();
        }
        else {
            // Unpause the game.
            set_game_state(false);
        }

    }

    // Make the bird fly.
    set_entity_velocity(player, (struct vector2D){player->velocity.x, -3});
}

// Function to move the player left.
void go_left()
{
    set_entity_velocity(player, (struct vector2D){-3, player->velocity.y});
}

// Function to move the player right.
void go_right()
{
    set_entity_velocity(player, (struct vector2D){3, player->velocity.y});
}

// Function to end the game.
void game_over() {
    set_label_text(score_label, "Game Over", true);
    set_game_state(true);
}

// Function to update the game scene. This function is called every game tick.
void update_gamescene()
{
    // Move cloud to the right.
    set_game_object_position(cloud, (struct vector2D){cloud->position.x + 0.2, cloud->position.y});
    
    // Update location of lightning if there is any.
    if (lightning != NULL && lightning->active)
    {
        set_game_object_position(lightning, (struct vector2D){lightning->position.x + 0.2, lightning->position.y});
    }

    // Update location of pipes.
    set_game_object_position(upper_pipe, (struct vector2D){upper_pipe->position.x - 0.2, upper_pipe->position.y});
    set_game_object_position(lower_pipe, (struct vector2D){lower_pipe->position.x - 0.2, lower_pipe->position.y});

    // Update location of ground.
    set_game_object_position(ground_1, (struct vector2D){ground_1->position.x - 0.2, ground_1->position.y});
    set_game_object_position(ground_2, (struct vector2D){ground_2->position.x - 0.2, ground_2->position.y});

    // Update location of ground if it goes off screen.
    if (ground_1->position.x <= -64)
    {
        set_game_object_position(ground_1, (struct vector2D){ground_1->position.x + ground_1->width * 2, 32});
    }
    else if (ground_2->position.x <= -64)
    {
        set_game_object_position(ground_2, (struct vector2D){ground_2->position.x + ground_2->width * 2, 32});
    }

    // Get the current score.
    get_score(score_text);

    // Update label if the score has changed.
    if (score_label->text != score_text && !get_game_state()){
        set_label_text(score_label, score_text, true);
    }

    // Check for the next lightning.
    if (get_game_uptime() > next_lightning)
    {
        // Set the next lightning.
        next_lightning = get_game_uptime() + 5;

        // Create a new lightning.
        lightning = create_game_object((struct vector2D){cloud->position.x + icon_lightning_width / 2, cloud->position.y + icon_lightning_height + 3}, icon_lightning_width, icon_lightning_height);
        set_game_object_graphic(lightning, icon_lightning);
    }

    // Remove the lightning if its older than 0.3 seconds.
    if (lightning != NULL && lightning->age + 0.3 < get_game_uptime())
    {
        remove_game_object(lightning);
    }

    // Check if the player is on the ground.
    if (player->on_ground) {
        game_over();
    }
}

void init_gamescene()
{
    // Get the start time.
    startTime = get_game_uptime();

    // Set engine functions for buttons.
    button_4_click = jump;
	button_3_click = go_left;
	button_2_click = go_right;

    // Set engine functions for game tick.
    on_game_tick = update_gamescene;

    // Set the next lightning time.
    next_lightning = get_game_uptime() + 5;

    // Create the score label.
    score_label = create_label("BTN4 to Start", (struct vector2D){64, 0}, true, false);

    // Create the player.
    player = create_entity((struct vector2D){20, 20}, icon_bird_width, icon_bird_height);
    set_entity_graphic(player, icon_bird);

    // Create the ground.
    ground_1 = create_game_object((struct vector2D){64, 32}, icon_ground_width, icon_ground_height);
    ground_2 = create_game_object((struct vector2D){192, 32}, icon_ground_width, icon_ground_height);
    set_game_object_graphic(ground_1, icon_ground);
    set_game_object_graphic(ground_2, icon_ground);

    // Set the engine ground level.
    game_set_ground_level(30);

    // Create the cloud.
    cloud = create_game_object((struct vector2D){-10, 10}, icon_cloud_width, icon_cloud_height);
    set_game_object_graphic(cloud, icon_cloud);

    // Create the pipes.
    upper_pipe = create_game_object((struct vector2D){128, 7}, icon_pipe_upper_width, icon_pipe_upper_height);
    lower_pipe = create_game_object((struct vector2D){128, 35}, icon_pipe_lower_width, icon_pipe_lower_height);
    set_game_object_graphic(upper_pipe, icon_pipe_upper);
    set_game_object_graphic(lower_pipe, icon_pipe_lower);

    // Update the game state.
    game_tick();
    game_draw();

    // Pause the game.
    set_game_state(true);
}

// Function to unload the game scene.
void unload_gamescene()
{
    // Remove all game objects and entities.
    remove_entity(player);
    remove_game_object(cloud);
    remove_game_object(upper_pipe);
    remove_game_object(lower_pipe);
    remove_label(score_label);
    remove_game_object(ground_1);
    remove_game_object(ground_2);

    // Set the engine functions to NULL.
    on_game_tick = NULL;
    button_4_click = NULL;
    button_3_click = NULL;
    button_2_click = NULL;

    // Set the ground level.
    game_set_ground_level(0);
}