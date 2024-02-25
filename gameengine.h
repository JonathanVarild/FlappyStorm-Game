#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_ENTITIES 64
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

extern struct entity entities[MAX_ENTITIES];
extern struct label labels[MAX_LABELS];

struct entity *create_entity(struct vector2D pos, int width, int height);
struct label *create_label(char *text, struct vector2D pos, bool centered, bool selected);

void set_entity_position(struct entity *ent, struct vector2D pos);
void set_label_position(struct label *lbl, struct vector2D pos);
void set_entity_velocity(struct entity *ent, struct vector2D vel);
void set_entity_graphic(struct entity *ent, uint8_t *graphic);

struct collision_box get_collision_box(struct entity *ent);

void game_init(void);
void game_tick(void);
void game_draw(void);
