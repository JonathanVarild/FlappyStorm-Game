#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_GAME_OBJECTS 32
#define MAX_ENTITIES 32
#define MAX_LABELS 8

struct vector2D {
    double x;
    double y;
};

struct collision_box {
    int x_left;
    int x_right;
    int y_top;
    int y_bottom;
};

struct game_object
{
    bool active;
    struct vector2D position;
    int width;
    int height;
    uint8_t *graphic;
};

struct entity {
    bool active;
    struct vector2D position;
    struct vector2D velocity;
    bool on_ground;
    int width;
    int height;
    uint8_t *graphic;
};

struct label {
    bool active;
    char *text;
    struct vector2D position;
    int x_offset;
    bool selected;
};

extern struct game_object game_objects[MAX_GAME_OBJECTS];
extern struct entity entities[MAX_ENTITIES];
extern struct label labels[MAX_LABELS];

struct game_object *create_game_object(struct vector2D pos, int width, int height);
struct entity *create_entity(struct vector2D pos, int width, int height);
struct label *create_label(char *text, struct vector2D pos, bool centered, bool selected);

void remove_game_object(struct game_object *obj);
void remove_entity(struct entity *ent);
void remove_label(struct label *lbl);

void set_game_object_position(struct game_object *obj, struct vector2D pos);
void set_entity_position(struct entity *ent, struct vector2D pos);
void set_label_position(struct label *lbl, struct vector2D pos);
void set_entity_velocity(struct entity *ent, struct vector2D vel);
void set_game_object_graphic(struct game_object *obj, uint8_t *graphic);
void set_entity_graphic(struct entity *ent, uint8_t *graphic);
void set_label_selected(struct label *lbl, bool selected);

struct collision_box get_game_object_collision_box(struct game_object *obj);
struct collision_box get_entity_collision_box(struct entity *ent);

extern void (*on_game_tick)(void);

void game_init(void);
void game_tick(void);
void game_draw(void);

extern void (*button_4_click)(void);
extern void (*button_3_click)(void);
extern void (*button_2_click)(void);

#endif