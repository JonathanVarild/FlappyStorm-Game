#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include <stdbool.h>

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

extern struct entity entities[MAX_ENTITIES];

struct game_object
{
    bool active;
    struct vector2D position;
};

// Function prototypes
struct entity *create_entity(struct vector2D pos, int width, int height);
void set_position(struct entity *ent, struct vector2D pos);
void set_velocity(struct entity *ent, struct vector2D vel);
struct collision_box get_collision_box(struct entity *ent);
void game_tick(void);
void game_init(void);
void game_draw(void);

#endif // GAME_ENGINE_H
