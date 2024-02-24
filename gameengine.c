#include <stdbool.h>
#include <stdlib.h>
#include "./screenengine.h"

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

#define MAX_ENTITIES 64

struct collision_box
{
    int x_left;
    int x_right;
    int y_top;
    int y_bottom;
};

struct vector2D
{
    double x;
    double y;
};

struct entity
{
    bool active;
    struct vector2D position;
    struct vector2D velocity;
    bool on_ground;
    int width;
    int height;
};

struct game_object
{
    bool active;
    struct vector2D position;
};

struct entity entities[MAX_ENTITIES] = {0};

struct entity *create_entity(struct vector2D pos, int width, int height)
{

    // Loop through all entities.
    int i;
    for (i = 0; i < MAX_ENTITIES; i++)
    {
        if (!entities[i].active)
        {
            entities[i].active = true;
            entities[i].position = pos;
            entities[i].velocity.x = 0;
            entities[i].velocity.y = 0;
            entities[i].on_ground = false;
            entities[i].width = width;
            entities[i].height = height;
            return &entities[i];
        }
    }
}

void set_position(struct entity *ent, struct vector2D pos)
{
    ent->position = pos;
}

void set_velocity(struct entity *ent, struct vector2D vel)
{
    ent->velocity = vel;
}

struct collision_box get_collision_box(struct entity *ent)
{
    struct collision_box box;
    box.x_left = ent->position.x;
    box.x_right = ent->position.x + ent->width;
    box.y_top = ent->position.y - ent->height;
    box.y_bottom = ent->position.y;
    return box;
}

void game_tick()
{
    // Loop through all entities.
    int i;
    for (i = 0; i < MAX_ENTITIES; i++)
    {

        // Get the entity.
        struct entity *ent = &entities[i];

        // Check if the entity is active.
        if (!ent->active)
        {
            continue;
        }

        // Update the position of the entity.
        ent->position.x += ent->velocity.x * 0.1;
        ent->position.y += ent->velocity.y * 0.1;

        // Check if the entity isn't on the ground.
        if (!ent->on_ground)
        {
            // Apply gravity.
            ent->velocity.y = ent->velocity.y < 3 ? ent->velocity.y + 0.1 : 3;

            // Check if the entity is touching the ground.
            if (get_collision_box(ent).y_bottom >= DISPLAY_HEIGHT)
            {
                ent->on_ground = true;
                ent->position.y = DISPLAY_HEIGHT;
                ent->velocity.y = 0;
            }
        }

        // CHeck if the entity is moving up or down.
        if (ent->position.y < DISPLAY_HEIGHT)
        {
            ent->on_ground = false;
        }
    }
}

void game_init()
{
    display_init();
}

void game_draw()
{
    matrix_reset();

    // Loop through all entities.
    int i;
    for (i = 0; i < MAX_ENTITIES; i++)
    {

        // Get the entity.
        struct entity *ent = &entities[i];

        // Check if the entity is active.
        if (!ent->active)
        {
            continue;
        }

        // Draw the entity.
        draw_rect((int)(ent->position.x) - ent->width / 2, (int)(ent->position.y) - ent->height, ent->width, ent->height);
    }

    display_update();
}