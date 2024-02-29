#include "gameengine.h"
#include <stdbool.h>
#include "icons.h"
#include "utilities.h"
#include "scenes.h"
#include "player_manager.h"

// Define base objects
static Label *top_label;
static Label *middle_label;
static Entity *player;
static Game_object *ground;

// Define arrays for dynamic objects.
static Game_object *obstacles[20] = {0};
static Game_object *lightnings[20] = {0};
static Entity *rain_drops[20] = {0};
static Game_object *powerups[20] = {0};

// Define game variables.
static int player_ID;
static bool alive = true;
static int start_time;
static int score;
static char score_text[4];
static double movement_speed;
static double spawn_rate;
static double next_obstacle;
static double next_lightning;
static int invincible_until = 0;

// Define arrays for dynamic objects.
static int next_lightnings[20] = {0};
static int next_rain_drops[20] = {0};

// Create enumeration for obstacle types.
enum obstacle_type
{
    PIPE,
    CLOUD,
    CLOUD_LIGHTNING,
    LIGHTNING,
};

// Function to remove all objects.
static void remove_all_objects()
{
    // Check if the player exists.
    if (player != NULL && player->active)
    {
        // Remove the player.
        remove_entity(player);
    }

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

        // Check if the lightning is active.
        if (lightnings[i] != NULL && lightnings[i]->active)
        {
            // Remove the lightning.
            remove_game_object(lightnings[i]);
        }

        // Check if the rain drop is active.
        if (rain_drops[i] != NULL && rain_drops[i]->active)
        {
            // Remove the rain drop.
            remove_entity(rain_drops[i]);
        }

        // Check if the powerup is active.
        if (powerups[i] != NULL && powerups[i]->active)
        {
            // Remove the powerup.
            remove_game_object(powerups[i]);
        }

        // Set the objects to NULL.
        obstacles[i] = NULL;
        lightnings[i] = NULL;
        rain_drops[i] = NULL;
        powerups[i] = NULL;
    }
}

// Function to get the current score.
static void get_score(char s[4])
{
    // Calculate the score.
    score = (int)(get_game_uptime() - start_time);

    // Convert the score to a string.
    s[0] = '0' + (score / 100) % 10;
    s[1] = '0' + (score / 10) % 10;
    s[2] = '0' + (score / 1) % 10;
    s[3] = '\0';
}

// Function to make the player jump and start game.
static void jump()
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
            load_gamescene(player_ID);
        }
        else
        {
            // Unpause the game.
            set_game_paused(false);
        }
    }

    // Update random seed.
    set_random_seed(get_game_uptime() * 100 * (player->position.x + player->position.y + player->velocity.x + player->velocity.y));

    // Make the bird fly.
    set_entity_velocity(player, (Vector2D){player->velocity.x, -3});
}

// Function to move the player left.
static void go_left()
{
    // Check if the game is paused.
    if (get_game_paused())
    {
        // Unload the game scene and load the menu scene.
        unload_gamescene();
        load_menuscene();
    }
    else
    {
        // Update random seed.
        set_random_seed(get_game_uptime() * 100 * (player->position.x + player->position.y + player->velocity.x + player->velocity.y));

        // Make the bird fly left.
        set_entity_velocity(player, (Vector2D){-3, player->velocity.y});
    }
}

// Function to move the player right.
static void go_right()
{
    // Check if the game is paused.
    if (get_game_paused())
    {
        // Unload the game scene and load the menu scene.
        unload_gamescene();
        load_menuscene();
    }
    else
    {
        // Update random seed.
        set_random_seed(get_game_uptime() * 100 * (player->position.x + player->position.y + player->velocity.x + player->velocity.y));

        // Make the bird fly right.
        set_entity_velocity(player, (Vector2D){3, player->velocity.y});
    }
}

// Function to end the game.
static void game_over()
{
    // Stop if the player is invincible.
    if (invincible_until > get_game_uptime())
    {
        return;
    }

    // Remove all objects.
    remove_all_objects();

    // Check if the player has a new high score.
    if (score > highscores[player_ID])
    {
        // Set the high score.
        set_highscore(player_ID, score);

        // Create the middle label.
        set_label_text(top_label, "New Highscore", true);
    }
    else {

        // Set the top label to game over.
        set_label_text(top_label, "Game Over", true);

    }

    // Create the middle label.
    middle_label = create_label(score_text, (Vector2D){64, 16}, true, false);

    // Stop the game.
    set_game_paused(true);
    alive = false;
}

// Function to generate a rain drop.
static void generate_rain_drop(Vector2D pos)
{
    // Loop through all rain drops.
    int i;
    for (i = 0; i < 20; i++)
    {
        // Check if the rain drop is not active.
        if (rain_drops[i] == NULL || !rain_drops[i]->active)
        {
            // Create the rain drop.
            rain_drops[i] = create_entity(pos, 1, 1);
            return;
        }
    }
}

// Function to generate a pipe.
static void generate_pipes()
{
    // Get random y offset.
    int y_offset = get_random_int(-2, 2);

    // Variable to store if the next pipe should be an upper pipe.
    bool upper_pipe = true;

    // Loop through all obstacles.
    int i;
    for (i = 0; i < 20; i++)
    {
        // Check if the obstacle is not active.
        if (obstacles[i] == NULL || !obstacles[i]->active)
        {
            // Check if we are creating an upper or lower pipe.
            if (upper_pipe)
            {
                // Create the upper pipe.
                obstacles[i] = create_game_object((Vector2D){128 + icon_pipe_upper_width, 7 + y_offset}, icon_pipe_upper_width, icon_pipe_upper_height);

                // Set the graphic and type of the pipe.
                set_game_object_graphic(obstacles[i], icon_pipe_upper);
                set_game_object_type(obstacles[i], PIPE);

                // Set the next pipe to be a lower pipe.
                upper_pipe = false;
            }
            else
            {
                // Create the lower pipe.
                obstacles[i] = create_game_object((Vector2D){128 + icon_pipe_lower_width, 35 + y_offset}, icon_pipe_lower_width, icon_pipe_lower_height);

                // Set the graphic and type of the pipe.
                set_game_object_graphic(obstacles[i], icon_pipe_lower);
                set_game_object_type(obstacles[i], PIPE);

                // Stop the function.
                return;
            }
        }
    }
}

// Function to generate a cloud.
static void generate_cloud()
{
    // Loop through all obstacles.
    int i;
    for (i = 0; i < 20; i++)
    {
        // Check if the obstacle is not active.
        if (obstacles[i] == NULL || !obstacles[i]->active)
        {
            // Create the cloud.
            obstacles[i] = create_game_object((Vector2D){128 + icon_cloud_width, 10 + get_random_int(-2, 4)}, icon_cloud_width, icon_cloud_height);

            // Set the graphic and type of the cloud.
            set_game_object_graphic(obstacles[i], icon_cloud);
            set_game_object_type(obstacles[i], CLOUD);

            // Set the next lightning and rain drop spawn time.
            next_lightnings[i] = get_game_uptime() + 5;
            next_rain_drops[i] = get_game_uptime() + 1;

            // Stop the function.
            return;
        }
    }
}

// Function to generate a powerup.
static void generate_powerup()
{
    // Loop through all powerups.
    int i;
    for (i = 0; i < 20; i++)
    {
        // Check if the powerup is not active.
        if (powerups[i] == NULL || !powerups[i]->active)
        {
            // Create the powerup.
            powerups[i] = create_game_object((Vector2D){128 + icon_star_width, get_random_int(icon_star_height, 32)}, icon_star_width, icon_star_height);

            // Set the graphic of the powerup.
            set_game_object_graphic(powerups[i], icon_star);

            // Stop the function.
            return;
        }
    }
}

// Function to update the game scene. This function is called every game tick.
static void update_gamescene()
{
    // Check if an obstacle should spawn.
    if (next_obstacle < get_game_uptime())
    {
        // Spawn the obstacle and set the next obstacle respawn.
        next_obstacle = get_game_uptime() + spawn_rate;

        // Increase the difficulty by decreasing the spawn rate.
        if (spawn_rate - 0.15 > 2) {
            spawn_rate -= 0.15;
        }

        // Increase the difficulty by increasing the movement speed.
        if (movement_speed + 0.01 < 0.4) {
            movement_speed += 0.01;
        }

        // Get random number to determine what to spawn.
        int r = get_random_int(0, 20);

        // Generate obstacle based on random number.
        if (r < 2)
        {
            generate_powerup();
        }
        else if (r < 12)
        {
            generate_pipes();
        }
        else
        {
            generate_cloud();
        }
    }

    // Get the player coliision box.
    Collision_box player_box;
    get_entity_collision_box(&player_box, player);

    // Loop through all level objects.
    int i;
    for (i = 0; i < 20; i++)
    {
        // Check if the obstacle is active.
        if (obstacles[i] != NULL && obstacles[i]->active)
        {
            // Move the obstacle to the left.
            set_game_object_position(obstacles[i], (Vector2D){obstacles[i]->position.x - movement_speed, obstacles[i]->position.y});

            // Check if the player can collide with the obstacle.
            if (obstacles[i]->type == PIPE || obstacles[i]->type == CLOUD_LIGHTNING)
            {
                // Get the collision box of the obstacle.
                Collision_box obstacle_box;
                get_game_object_collision_box(&obstacle_box, obstacles[i]);

                // Check if the player is colliding with the obstacle.
                if (player_box.x_right > obstacle_box.x_left && player_box.x_left < obstacle_box.x_right && player_box.y_bottom > obstacle_box.y_top && player_box.y_top < obstacle_box.y_bottom)
                {
                    // End the game.
                    game_over();
                }
            }

            // Check if the entity is a active lightning cloud.
            if (obstacles[i]->type == CLOUD_LIGHTNING && lightnings[i] != NULL && lightnings[i]->active)
            {
                // Set the position of the lightning.
                set_game_object_position(lightnings[i], (Vector2D){obstacles[i]->position.x + icon_lightning_width / 2, obstacles[i]->position.y + icon_lightning_height + 3});

                // Get the collision box of the player.
                Collision_box lightning_box;
                get_game_object_collision_box(&lightning_box ,lightnings[i]);

                // Check if the player is colliding with the lightning.
                if (player_box.x_right > lightning_box.x_left && player_box.x_left < lightning_box.x_right && player_box.y_bottom > lightning_box.y_top && player_box.y_top < lightning_box.y_bottom)
                {
                    game_over();
                }

                // Check if the lightning is old.
                if (lightnings[i]->age + 0.3 < get_game_uptime())
                {
                    // Remove the lightning.
                    remove_game_object(lightnings[i]);

                    // Set the object to a normal cloud.
                    set_game_object_type(obstacles[i], CLOUD);
                    set_game_object_graphic(obstacles[i], icon_cloud);

                    // Set the lightning to NULL.
                    lightnings[i] = NULL;
                }
            }

            // Check if the cloud should spawn a lightning.
            if (obstacles[i]->type == CLOUD && next_lightnings[i] < get_game_uptime())
            {
                // Set the next lightning spawn time.
                next_lightnings[i] = get_game_uptime() + 2 + get_random_int(0, 2);

                // Create the lightning.
                lightnings[i] = create_game_object((Vector2D){obstacles[i]->position.x + icon_lightning_width / 2, obstacles[i]->position.y + icon_lightning_height + 1}, icon_lightning_width, icon_lightning_height);
                
                // Set the graphic and type of the lightning.
                set_game_object_graphic(lightnings[i], icon_lightning);
                set_game_object_type(lightnings[i], LIGHTNING);

                // Set the graphic and type of the cloud.
                set_game_object_graphic(obstacles[i], icon_lightningcloud);
                set_game_object_type(obstacles[i], CLOUD_LIGHTNING);
            }

            // Check if the cloud should spawn a rain drop.
            if (obstacles[i]->type == CLOUD && next_rain_drops[i] < get_game_uptime())
            {
                // Set the next rain drop spawn time.
                next_rain_drops[i] = get_game_uptime() + 0.5 + get_random_int(1, 2);

                // Create the rain drop.
                generate_rain_drop((Vector2D){obstacles[i]->position.x + get_random_int(-icon_cloud_width / 2, icon_cloud_width / 2), obstacles[i]->position.y});
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

        // Check if the powerup is active.
        if (powerups[i] != NULL && powerups[i]->active)
        {
            // Move the powerup to the left.
            set_game_object_position(powerups[i], (Vector2D){powerups[i]->position.x - movement_speed, powerups[i]->position.y});

            // Get the collision box of the powerup.
            Collision_box powerup_box;
            get_game_object_collision_box(&powerup_box ,powerups[i]);

            // Check if the player is colliding with the powerup.
            if (player_box.x_right > powerup_box.x_left && player_box.x_left < powerup_box.x_right && player_box.y_bottom > powerup_box.y_top && player_box.y_top < powerup_box.y_bottom)
            {
                // Remove the powerup.
                remove_game_object(powerups[i]);
                powerups[i] = NULL;

                // Calculate the difficulty offset.
                int difficulty_offset = (int)(get_game_uptime() - start_time) / 10;

                // Set a max offset of 7.
                if (difficulty_offset > 7) {
                    difficulty_offset = 7;
                }

                // Set the invincible time.
                invincible_until = get_game_uptime() + 10 - difficulty_offset;
            }
        }

        // Check if the rain drop is active.
        if (rain_drops[i] != NULL && rain_drops[i]->active)
        {
            // Move the rain drop.
            set_entity_position(rain_drops[i], (Vector2D){rain_drops[i]->position.x - movement_speed, rain_drops[i]->position.y});

            // Check if the rain drop is off screen.
            if (rain_drops[i]->on_ground)
            {
                // Remove the rain drop.
                remove_entity(rain_drops[i]);
                rain_drops[i] = NULL;
            }
        }
    }

    // Update location of ground.
    set_game_object_position(ground, (Vector2D){ground->position.x - movement_speed, ground->position.y});

    // Check if the ground is almost off screen.
    if (ground->position.x <= 0)
    {
        // Move the ground back to the right.
        set_game_object_position(ground, (Vector2D){ground->position.x + ground->width / 2, 32});
    }

    // Get the current score.
    get_score(score_text);

    // Check what to display on the top label.
    if (invincible_until > get_game_uptime())
    {
        // Display "INVINCIBLE" when the player is invincible.
        set_label_text(top_label, "INVINCIBLE", true);
    }
    else if (top_label->text != score_text && !get_game_paused())
    {
        // Display score when the score changes and the game isn't paused.
        set_label_text(top_label, score_text, true);
    }

    // Check if the player is invincible.
    if (invincible_until > get_game_uptime())
    {
        // Make the player flicker.
        set_entity_visibility(player, (int)(get_game_uptime() * 10) % 2);
    }
    else
    {
        // Make the player visible.
        set_entity_visibility(player, true);
    }

    // Check if the player is on the ground.
    if (player->on_ground)
    {
        // End the game.
        game_over();
    }

    // Check if the player is colliding with the world edges.
    if (player->position.x < 0 || player->position.x > 128)
    {
        // Set the player velocity to 0 and set the position to the edge of the world.
        set_entity_velocity(player, (Vector2D){0, player->velocity.y});
        set_entity_position(player, (Vector2D){player->position.x < 0 ? 0 : 128, player->position.y});
    }
}

// Function to load the game scene.
void load_gamescene(int selected_player_ID)
{
    // Set the player.
    player_ID = selected_player_ID;

    // Update random seed.
    set_random_seed((unsigned long)(get_game_uptime() * 100));

    // Set the player as alive.
    alive = true;

    // Set invincible_until.
    invincible_until = 0;

    // Pause the game.
    set_game_paused(true);

    // Get the start time.
    start_time = get_game_uptime();

    // Set the next obstacle spawn time.
    next_obstacle = get_game_uptime();

    // Set the spawn rate.
    spawn_rate = 5;

    // Set the movement speed.
    movement_speed = 0.2;

    // Set engine functions for buttons.
    button_4_click = jump;
    button_3_click = go_left;
    button_2_click = go_right;

    // Set engine functions for game tick.
    on_game_tick = update_gamescene;

    // Create the score label.
    top_label = create_label("BTN4 to Start", (Vector2D){64, 0}, true, false);

    // Create the player.
    player = create_entity((Vector2D){20, 20}, icon_bird_width, icon_bird_height);
    set_entity_graphic(player, icon_bird);

    // Create the ground.
    ground = create_game_object((Vector2D){128, 32}, icon_ground_width, icon_ground_height);
    set_game_object_graphic(ground, icon_ground);

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
    remove_label(top_label);
    remove_game_object(ground);

    // Check if the middle label exists.
    if (middle_label != NULL && middle_label->active)
    {
        // Remove the middle label.
        remove_label(middle_label);
    }

    // Remove all objects.
    remove_all_objects();

    // Set the engine functions to NULL.
    on_game_tick = NULL;
    button_4_click = NULL;
    button_3_click = NULL;
    button_2_click = NULL;

    // Set the ground level.
    game_set_ground_level(0);

    // Resume the game.
    set_game_paused(false);
}