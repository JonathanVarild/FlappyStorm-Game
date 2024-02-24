#include <stdbool.h>
#include <stdlib.h>
#include "./screenengine.h"

#define MAX_ENTITIES 64
#define MAX_LABELS 8
#define DRAW_LOOP 64

/*
Structs
*/

struct vector2D
{
    double x;
    double y;
};

struct collision_box
{
    int x_left;
    int x_right;
    int y_top;
    int y_bottom;
};

struct game_object
{
    bool active;
    struct vector2D position;
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

struct label
{
    bool active;
    char *text;
    struct vector2D position;
    int x_offset;
    bool selected;
};

/*
Object arrays
*/

struct entity entities[MAX_ENTITIES] = {0};
struct label labels[MAX_LABELS] = {0};

/*
Creation functions
*/

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

struct label *create_label(char *text, struct vector2D pos, bool centered, bool selected)
{
    // Loop through all labels.
    int i;
    for (i = 0; i < MAX_LABELS; i++)
    {
        if (!labels[i].active)
        {
            int len = 0;

            if (centered)
            {
                int j = 0;
                while (text[j] != '\0')
                {
                    len += text[len] == ' ' ? 4 : 8;
                    j++;
                }
            }

            labels[i].active = true;
            labels[i].text = text;
            labels[i].position = pos;
            labels[i].x_offset = len / 2;
            labels[i].selected = selected;
            return &labels[i];
        }
    }
}

/*
Setters
*/

void set_entity_position(struct entity *ent, struct vector2D pos)
{
    ent->position = pos;
}

void set_label_position(struct label *lbl, struct vector2D pos)
{
    lbl->position = pos;
}

void set_entity_velocity(struct entity *ent, struct vector2D vel)
{
    ent->velocity = vel;
}

/*
Getters
*/

struct collision_box get_collision_box(struct entity *ent)
{
    struct collision_box box;
    box.x_left = ent->position.x;
    box.x_right = ent->position.x + ent->width;
    box.y_top = ent->position.y - ent->height;
    box.y_bottom = ent->position.y;
    return box;
}

/*
Engine functions

*/

void game_init()
{
    display_init();
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

void game_draw()
{
    matrix_reset();

    // Loop through all entities.
    int i;
    for (i = 0; i < DRAW_LOOP; i++)
    {
        if (i < MAX_ENTITIES)
        {
            // Get the entity.
            struct entity *ent = &entities[i];

            // Check if the entity is active.
            if (ent->active)
            {
                draw_rect((int)(ent->position.x) - ent->width / 2, (int)(ent->position.y) - ent->height, ent->width, ent->height);
            }
        }

        if (i < MAX_LABELS)
        {
            // Get the label.
            struct label *lbl = &labels[i];

            // Check if the label is active.
            if (lbl->active)
            {
                draw_text((int)(lbl->position.x) - lbl->x_offset, (int)(lbl->position.y), lbl->text,  lbl->selected);
            }
        }
    }

    display_update();
}