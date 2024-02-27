#include "gameengine.h"
#include <stdbool.h>
#include "icons.h"

// Give entities/labels/game objects a bigger scope.
struct label *score_label;
struct entity *player;

struct game_object *ground_1;
struct game_object *ground_2;

double next_obstacle;
double next_lightning;
struct game_object *obstacles[20] = {0};
struct game_object *lightnings[20] = {0};
int next_lightnings[20] = {0};

// Give these functions a bigger scope.
void(init_gamescene)();
void(unload_gamescene)();
void(update_gamescene)();

// Create enumeration for obstacle types.
enum obstacle_type
{
    PIPE,
    CLOUD,
    CLOUD_LIGHTNING,
    LIGHTNING,
};

// Variables used in game scene.
bool alive = true;
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
    if (get_game_paused())
    {
        // Check if the player is on the ground.
        if (!alive)
        {
            // Reset the game scene.
            alive = true;
            unload_gamescene();
            init_gamescene();
        }
        else
        {
            // Unpause the game.
            set_game_paused(false);
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
void game_over()
{
    set_label_text(score_label, "Game Over", true);
    set_game_paused(true);
    alive = false;
}

void generate_pipes()
{
    bool upper_pipe = true;
    int i;
    for (i = 0; i < 20; i++)
    {
        if (obstacles[i] == NULL || !obstacles[i]->active)
        {
            if (upper_pipe)
            {
                obstacles[i] = create_game_object((struct vector2D){128 + icon_pipe_upper_width, 7}, icon_pipe_upper_width, icon_pipe_upper_height);
                set_game_object_graphic(obstacles[i], icon_pipe_upper);
                set_game_object_type(obstacles[i], PIPE);
                upper_pipe = false;
            }
            else
            {
                obstacles[i] = create_game_object((struct vector2D){128 + icon_pipe_lower_width, 35}, icon_pipe_lower_width, icon_pipe_lower_height);
                set_game_object_graphic(obstacles[i], icon_pipe_lower);
                set_game_object_type(obstacles[i], PIPE);
                return;
            }
        }
    }
}

void generate_cloud()
{
    int i;
    for (i = 0; i < 20; i++)
    {
        if (obstacles[i] == NULL || !obstacles[i]->active)
        {
            obstacles[i] = create_game_object((struct vector2D){128 + icon_cloud_width, 10}, icon_cloud_width, icon_cloud_height);
            set_game_object_graphic(obstacles[i], icon_cloud);
            set_game_object_type(obstacles[i], CLOUD);

            next_lightnings[i] = get_game_uptime() + 5;
            return;
        }
    }
}

// Function to update the game scene. This function is called every game tick.
void update_gamescene()
{
    // Check if an obstacle should spawn.
    if (next_obstacle < get_game_uptime())
    {
        // Spawn the obstacle and set the next obstacle respawn.
        next_obstacle = get_game_uptime() + 5;
        
        if ((int)(player->position.y) % 2 == 0)
        {
            generate_pipes();
        }
        else
        {
            generate_cloud();
        }
    }

    // Get the player coliision box.
    struct collision_box player_box = get_entity_collision_box(player);

    // Loop through all obstacles.
    int i;
    for (i = 0; i < 20; i++)
    {
        // Check if the obstacle is active.
        if (obstacles[i] != NULL && obstacles[i]->active)
        {
            // Move the obstacle to the left.
            set_game_object_position(obstacles[i], (struct vector2D){obstacles[i]->position.x - 0.2, obstacles[i]->position.y});

            // Check if the player can collide with the obstacle.
            if (obstacles[i]->type == PIPE || obstacles[i]->type == CLOUD_LIGHTNING || obstacles[i]->type == LIGHTNING)
            {
                // Get the collision box of the obstacle.
                struct collision_box obstacle_box = get_game_object_collision_box(obstacles[i]);

                // Check if the player is colliding with the obstacle.
                if (player_box.x_right > obstacle_box.x_left && player_box.x_left < obstacle_box.x_right && player_box.y_bottom > obstacle_box.y_top && player_box.y_top < obstacle_box.y_bottom)
                {
                    game_over();
                }
            }

            // Check if the entity is a active lightning cloud.
            if (obstacles[i]->type == CLOUD_LIGHTNING && lightnings[i] != NULL && lightnings[i]->active)
            {
                set_game_object_position(lightnings[i], (struct vector2D){obstacles[i]->position.x + icon_lightning_width / 2, obstacles[i]->position.y + icon_lightning_height + 3});            
            
                if (lightnings[i]->age + 0.3 < get_game_uptime())
                {
                    remove_game_object(lightnings[i]);
                    set_game_object_type(obstacles[i], CLOUD);
                    lightnings[i] = NULL;
                }
            
            }

            // Check if the cloud should spawn a lightning.
            if (obstacles[i]->type == CLOUD && next_lightnings[i] < get_game_uptime()) {
                next_lightnings[i] = get_game_uptime() + 4;

                lightnings[i] = create_game_object((struct vector2D){obstacles[i]->position.x + icon_lightning_width / 2, obstacles[i]->position.y + icon_lightning_height + 1}, icon_lightning_width, icon_lightning_height);
                set_game_object_graphic(lightnings[i], icon_lightning);
                set_game_object_type(lightnings[i], LIGHTNING);

                set_game_object_type(obstacles[i], CLOUD_LIGHTNING);
            }

            // Check if the obstacle is off screen.
            if (obstacles[i]->position.x < -obstacles[i]->width)
            {
                // Check if the object is an active lightning cloud.
                if (obstacles[i]->type == CLOUD_LIGHTNING && lightnings[i] != NULL && lightnings[i]->active)
                {
                    // Remove the lightning.
                    remove_game_object(lightnings[i]);
                    lightnings[i] = NULL;
                }

                // Remove the object.
                remove_game_object(obstacles[i]);
                obstacles[i] = NULL;
            }
        }
    }

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
    if (score_label->text != score_text && !get_game_paused())
    {
        set_label_text(score_label, score_text, true);
    }

    // Check if the player is on the ground.
    if (player->on_ground)
    {
        game_over();
    }

}

void init_gamescene()
{
    // Set the player as alive.
    alive = true;

    // Pause the game.
    set_game_paused(true);

    // Get the start time.
    startTime = get_game_uptime();

    // Set the next obstacle spawn time.
    next_obstacle = get_game_uptime();

    // Set engine functions for buttons.
    button_4_click = jump;
    button_3_click = go_left;
    button_2_click = go_right;

    // Set engine functions for game tick.
    on_game_tick = update_gamescene;

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

    // Update the game state.
    game_tick();
    game_draw();
}

// Function to unload the game scene.
void unload_gamescene()
{
    // Remove all game objects and entities.
    remove_entity(player);
    remove_label(score_label);
    remove_game_object(ground_1);
    remove_game_object(ground_2);

    // Loop through all obstacles.
    int i;
    for (i = 0; i < 20; i++)
    {
        // Check if the obstacle is active.
        if (obstacles[i] != NULL && obstacles[i]->active)
        {
            // Remove the obstacle.
            remove_game_object(obstacles[i]);
        }
        if (lightnings[i] != NULL && lightnings[i]->active)
        {
            // Remove the lightning.
            remove_game_object(lightnings[i]);
        }

        obstacles[i] = NULL;
        lightnings[i] = NULL;
    }

    // Set the engine functions to NULL.
    on_game_tick = NULL;
    button_4_click = NULL;
    button_3_click = NULL;
    button_2_click = NULL;

    // Set the ground level.
    game_set_ground_level(0);
}