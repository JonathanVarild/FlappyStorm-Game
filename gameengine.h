#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

// Define limits for game objects, entities and labels.
#define MAX_GAME_OBJECTS 32
#define MAX_ENTITIES 32
#define MAX_LABELS 16
#define DRAW_LOOP 32

// Create struct for 2D vector.
typedef struct {
    double x;
    double y;
} Vector2D;

// Create struct for collision box.
typedef struct {
    int x_left;
    int x_right;
    int y_top;
    int y_bottom;
} Collision_box;

// Create struct for game object.
typedef struct {
    bool active;
    bool is_visible;
    Vector2D position;
    int width;
    int height;
    double age;
    int type;
    uint8_t *graphic;
} Game_object;


// Create struct for entity.
typedef struct {
    bool active;
    bool is_visible;
    Vector2D position;
    Vector2D velocity;
    bool on_ground;
    int width;
    int height;
    double age;
    int type;
    uint8_t *graphic;
} Entity;

// Create struct for label.
typedef struct {
    bool active;
    char *text;
    Vector2D position;
    int x_offset;
    bool selected;
} Label;

// Creation functions.
Game_object *create_game_object(Vector2D pos, int width, int height);
Entity *create_entity(Vector2D pos, int width, int height);
Label *create_label(char *text, Vector2D pos, bool centered, bool selected);

// Removal functions.
void remove_game_object(Game_object *obj);
void remove_entity(Entity *ent);
void remove_label(Label *lbl);

// Setters.
void set_game_object_position(Game_object *obj, Vector2D pos);
void set_entity_position(Entity *ent, Vector2D pos);
void set_label_position(Label *lbl, Vector2D pos);
void set_entity_velocity(Entity *ent, Vector2D vel);
void set_game_object_graphic(Game_object *obj, uint8_t *graphic);
void set_entity_graphic(Entity *ent, uint8_t *graphic);
void set_label_selected(Label *lbl, bool selected);
void set_label_text(Label *lbl, char *text, bool centered);
void set_entity_type(Entity *ent, int type);
void set_game_object_type(Game_object *obj, int type);
void set_entity_visibility(Entity *ent, bool visible);
void set_game_object_visibility(Game_object *obj, bool visible);

// Getters.
void get_game_object_collision_box(Collision_box *box, Game_object *obj);
void get_entity_collision_box(Collision_box *box, Entity *ent);

// Create pointers for engine functions.
void (*on_game_tick)(void);
void (*button_4_click)(void);
void (*button_3_click)(void);
void (*button_2_click)(void);

// Engine functions.
double get_game_uptime(void);
void set_game_paused(bool paused);
bool get_game_paused();
void game_set_ground_level(int level);
int game_get_ground_level();
void game_init(void);
void game_tick(void);
void game_draw(void);

#endif