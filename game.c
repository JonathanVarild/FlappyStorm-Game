#include "gameengine.h"
#include <stdbool.h>
#include "icons.h"
#include "utilities.h"

// Give entities/labels/game objects a bigger scope.
struct label *top_label;
struct label *middle_label;
struct entity *player;

struct game_object *ground;

double next_obstacle;
double next_lightning;
struct game_object *obstacles[20] = {0};
struct game_object *lightnings[20] = {0};
struct entity *rain_drops[20] = {0};
struct game_object *powerups[20] = {0};

int next_lightnings[20] = {0};
int next_rain_drops[20] = {0};

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
int invincible_until = false;
int startTime;
int score;
char score_text[4];

// Function to remove all objects.
void remove_all_objects()
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

        obstacles[i] = NULL;
        lightnings[i] = NULL;
        rain_drops[i] = NULL;
        powerups[i] = NULL;
    }
}

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

    // Update random seed.
    set_random_seed(get_game_uptime() * 100 * (player->position.x + player->position.y + player->velocity.x + player->velocity.y));

    // Make the bird fly.
    set_entity_velocity(player, (struct vector2D){player->velocity.x, -3});
}

// Function to move the player left.
void go_left()
{
    set_random_seed(get_game_uptime() * 100 * (player->position.x + player->position.y + player->velocity.x + player->velocity.y));
    set_entity_velocity(player, (struct vector2D){-3, player->velocity.y});
}

// Function to move the player right.
void go_right()
{
    set_random_seed(get_game_uptime() * 100 * (player->position.x + player->position.y + player->velocity.x + player->velocity.y));
    set_entity_velocity(player, (struct vector2D){3, player->velocity.y});
}

// Function to end the game.
void game_over()
{
    // Check if the player is invincible.
    if (invincible_until > get_game_uptime()) {
        return;
    }

    // Remove all objects.
    remove_all_objects();

    // Set the top label to game over.
    set_label_text(top_label, "Game Over", true);

    // Create the middle label.
    middle_label = create_label(score_text, (struct vector2D){64, 16}, true, false);

    // Stop the game.
    set_game_paused(true);
    alive = false;
}

void generate_rain_drop(struct vector2D pos)
{
    int i;
    for (i = 0; i < 20; i++)
    {
        if (rain_drops[i] == NULL || !rain_drops[i]->active)
        {
            rain_drops[i] = create_entity(pos, 1, 1);
            return;
        }
    }
}

void generate_pipes()
{
    int y_offset = get_random_int(-2, 2);
    bool upper_pipe = true;
    int i;
    for (i = 0; i < 20; i++)
    {
        if (obstacles[i] == NULL || !obstacles[i]->active)
        {
            if (upper_pipe)
            {
                obstacles[i] = create_game_object((struct vector2D){128 + icon_pipe_upper_width, 7 + y_offset}, icon_pipe_upper_width, icon_pipe_upper_height);
                set_game_object_graphic(obstacles[i], icon_pipe_upper);
                set_game_object_type(obstacles[i], PIPE);
                upper_pipe = false;
            }
            else
            {
                obstacles[i] = create_game_object((struct vector2D){128 + icon_pipe_lower_width, 35 + y_offset}, icon_pipe_lower_width, icon_pipe_lower_height);
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
            int y_offset = get_random_int(-2, 4);

            obstacles[i] = create_game_object((struct vector2D){128 + icon_cloud_width, 10 + y_offset}, icon_cloud_width, icon_cloud_height);
            set_game_object_graphic(obstacles[i], icon_cloud);
            set_game_object_type(obstacles[i], CLOUD);

            next_lightnings[i] = get_game_uptime() + 5;
            next_rain_drops[i] = get_game_uptime() + 1;
            return;
        }
    }
}

void generate_powerup()
{
    int i;
    for (i = 0; i < 20; i++)
    {
        if (powerups[i] == NULL || !powerups[i]->active)
        {
            powerups[i] = create_game_object((struct vector2D){128 + icon_star_width, get_random_int(icon_star_height,32)}, icon_star_width, icon_star_height);
            set_game_object_graphic(powerups[i], icon_star);
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

        // Get random number.
        int r = get_random_int(0, 20);

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
    struct collision_box player_box = get_entity_collision_box(player);

    // Loop through all level objects.
    int i;
    for (i = 0; i < 20; i++)
    {
        // Check if the obstacle is active.
        if (obstacles[i] != NULL && obstacles[i]->active)
        {
            // Move the obstacle to the left.
            set_game_object_position(obstacles[i], (struct vector2D){obstacles[i]->position.x - 0.2, obstacles[i]->position.y});

            // Check if the player can collide with the obstacle.
            if (obstacles[i]->type == PIPE || obstacles[i]->type == CLOUD_LIGHTNING)
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
                // Set the position of the lightning.
                set_game_object_position(lightnings[i], (struct vector2D){obstacles[i]->position.x + icon_lightning_width / 2, obstacles[i]->position.y + icon_lightning_height + 3});

                // Get the collision box of the player.
                struct collision_box lightning_box = get_game_object_collision_box(lightnings[i]);

                // Check if the player is colliding with the lightning.
                if (player_box.x_right > lightning_box.x_left && player_box.x_left < lightning_box.x_right && player_box.y_bottom > lightning_box.y_top && player_box.y_top < lightning_box.y_bottom)
                {
                    game_over();
                }

                // Check if the lightning is old.
                if (lightnings[i]->age + 0.3 < get_game_uptime())
                {
                    remove_game_object(lightnings[i]);
                    set_game_object_type(obstacles[i], CLOUD);
                    set_game_object_graphic(obstacles[i], icon_cloud);
                    lightnings[i] = NULL;
                }
            }

            // Check if the cloud should spawn a lightning.
            if (obstacles[i]->type == CLOUD && next_lightnings[i] < get_game_uptime())
            {
                next_lightnings[i] = get_game_uptime() + 2 + get_random_int(0, 2);

                lightnings[i] = create_game_object((struct vector2D){obstacles[i]->position.x + icon_lightning_width / 2, obstacles[i]->position.y + icon_lightning_height + 1}, icon_lightning_width, icon_lightning_height);
                set_game_object_graphic(lightnings[i], icon_lightning);
                set_game_object_type(lightnings[i], LIGHTNING);

                set_game_object_type(obstacles[i], CLOUD_LIGHTNING);
                set_game_object_graphic(obstacles[i], icon_lightningcloud);
            }

            // Check if the cloud should spawn a rain drop.
            if (obstacles[i]->type == CLOUD && next_rain_drops[i] < get_game_uptime())
            {
                next_rain_drops[i] = get_game_uptime() + 0.5 + get_random_int(1, 2);

                generate_rain_drop((struct vector2D){obstacles[i]->position.x + get_random_int(-icon_cloud_width / 2, icon_cloud_width / 2), obstacles[i]->position.y});
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
            set_game_object_position(powerups[i], (struct vector2D){powerups[i]->position.x - 0.2, powerups[i]->position.y});

            // Get the collision box of the powerup.
            struct collision_box powerup_box = get_game_object_collision_box(powerups[i]);

            // Check if the player is colliding with the powerup.
            if (player_box.x_right > powerup_box.x_left && player_box.x_left < powerup_box.x_right && player_box.y_bottom > powerup_box.y_top && player_box.y_top < powerup_box.y_bottom)
            {
                remove_game_object(powerups[i]);
                powerups[i] = NULL;
                invincible_until = get_game_uptime() + 10;
                
            }
        }
    
        // Check if the rain drop is active.
        if (rain_drops[i] != NULL && rain_drops[i]->active)
        {
            // Move the rain drop.
            set_entity_position(rain_drops[i], (struct vector2D){rain_drops[i]->position.x - 0.2, rain_drops[i]->position.y});

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
    set_game_object_position(ground, (struct vector2D){ground->position.x - 0.2, ground->position.y});

    // Update location of ground if it goes off screen.
    if (ground->position.x <= 0)
    {
        set_game_object_position(ground, (struct vector2D){ground->position.x + ground->width / 2, 32});
    }

    // Get the current score.
    get_score(score_text);

    // Update label if the score has changed.
    if (invincible_until > get_game_uptime()) {
        set_label_text(top_label, "INVINCIBLE", true);
    }
    else if (top_label->text != score_text && !get_game_paused())
    {
        set_label_text(top_label, score_text, true);
    }

    // Check if the player is invincible.
    if (invincible_until > get_game_uptime()) {
        set_entity_visibility(player, (int)(get_game_uptime() * 10) % 2);
    }
    else {
        set_entity_visibility(player, true);
    }

    // Check if the player is on the ground.
    if (player->on_ground)
    {
        game_over();
    }

    // Check if the player is colliding with the world edges.
    if (player->position.x < 0 || player->position.x > 128)
    {
        set_entity_velocity(player, (struct vector2D){0, player->velocity.y});
        set_entity_position(player, (struct vector2D){player->position.x < 0 ? 0 : 128, player->position.y});
    }
}

void init_gamescene()
{
    // Update random seed.
    set_random_seed((unsigned long)(get_game_uptime() * 100));

    // Set the player as alive.
    alive = true;

    // Set invincible_until.
    invincible_until = 0;

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
    top_label = create_label("BTN4 to Start", (struct vector2D){64, 0}, true, false);

    // Create the player.
    player = create_entity((struct vector2D){20, 20}, icon_bird_width, icon_bird_height);
    set_entity_graphic(player, icon_bird);

    // Create the ground.
    ground = create_game_object((struct vector2D){128, 32}, icon_ground_width, icon_ground_height);
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
}