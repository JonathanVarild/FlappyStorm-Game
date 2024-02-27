#include <stdbool.h>
#include <stdlib.h>
#include "screenengine.h"
#include <stdint.h>

#define MAX_GAME_OBJECTS 32
#define MAX_ENTITIES 32
#define MAX_LABELS 8
#define DRAW_LOOP 64

/*
Utility functions
*/

double game_uptime = 0;

double get_game_uptime()
{
    return game_uptime;
}

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
    int width;
    int height;
    double age;
    uint8_t *graphic;
};

struct entity
{
    bool active;
    struct vector2D position;
    struct vector2D velocity;
    bool on_ground;
    int width;
    int height;
    double age;
    uint8_t *graphic;
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

struct game_object game_objects[MAX_GAME_OBJECTS] = {0};
struct entity entities[MAX_ENTITIES] = {0};
struct label labels[MAX_LABELS] = {0};

/*
Creation functions
*/

struct game_object *create_game_object(struct vector2D pos, int width, int height)
{
    // Loop through all game objects.
    int i;
    for (i = 0; i < MAX_GAME_OBJECTS; i++)
    {
        if (!game_objects[i].active)
        {
            game_objects[i].active = true;
            game_objects[i].position = pos;
            game_objects[i].width = width;
            game_objects[i].height = height;
            game_objects[i].age = get_game_uptime();
            return &game_objects[i];
        }
    }
}

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
            entities[i].age = get_game_uptime();
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
                    len += text[j] == ' ' ? 4 : 8;
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
Removal functions
*/

void remove_game_object(struct game_object *obj)
{
    obj->active = false;
    obj->graphic = NULL;
}

void remove_entity(struct entity *ent)
{
    ent->active = false;
    ent->graphic = NULL;
}

void remove_label(struct label *lbl)
{
    lbl->active = false;
    lbl->text = NULL;
}

/*
Setters
*/

void set_game_object_position(struct game_object *obj, struct vector2D pos)
{
    obj->position = pos;
}

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

void set_game_object_graphic(struct game_object *obj, uint8_t *graphic)
{
    obj->graphic = graphic;
}

void set_entity_graphic(struct entity *ent, uint8_t *graphic)
{
    ent->graphic = graphic;
}

void set_label_selected(struct label *lbl, bool selected)
{
    lbl->selected = selected;
}

void set_label_text(struct label *lbl, char *text, bool centered)
{
    lbl->text = text;

    if (centered)
    {
        int len = 0;
        int j = 0;
        while (text[j] != '\0')
        {
            len += text[j] == ' ' ? 4 : 8;
            j++;
        }
        lbl->x_offset = len / 2;
    }
    else {
        lbl->x_offset = 0;
    }
}

/*
Getters
*/

struct collision_box get_game_object_collision_box(struct game_object *obj)
{
    struct collision_box box;
    box.x_left = obj->position.x;
    box.x_right = obj->position.x + obj->width;
    box.y_top = obj->position.y - obj->height;
    box.y_bottom = obj->position.y;
    return box;
}

struct collision_box get_entity_collision_box(struct entity *ent)
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

void (*on_game_tick)(void);
bool game_paused = false;
int ground_level = 32;

void set_game_state(bool paused)
{
    game_paused = paused;
}

bool get_game_state()
{
    return game_paused;
}

void game_set_ground_level(int level)
{
    ground_level = level;
}

int game_get_ground_level()
{
    return ground_level;
}

void game_init()
{
    display_init();

    // Set up clicky buttons as inputs.
    TRISDSET = 0x7F0;

    // Enable interrupts for buttons.
    IECSET(0) = 0x180000; // Enable interrupts for buttons.
    IPCSET(4) = 0x1F000000; // Set priority level to 7 (1111 0000 0000 0000 0000 0000 0000 0000)

    // Set up Timer 2 interrupt with a period of 0.005 seconds. (200 fps / ticks per second)
	T2CON = 0x0;			// Stop any existing timer2 and clear control register
	T2CONSET = 0x70;		// Set prescaler to 1:256
	PR2 = 1562;			    // Set period register to ((0.01 * 80000000) / 256) = 1562
	TMR2 = 0x0;				// Clear the timer register
	IFSCLR(0) = 0x100;		// Clear the Timer 2 interrupt flag
	IECSET(0) = 0x100;		// Enable Timer 2 interrupts
	T2CONSET = 0x8000;		// Start Timer 2
	IPCSET(2) = 0x1A;		// Set priority level to 7 (0001 1100)

    // Enable interrupts.
	enable_interrupt();    
}

void game_tick()
{
    // Call on_game_tick function.
    if (on_game_tick != NULL)
    {
        on_game_tick();
    }

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

        // Check if x velocity isn't 0.
        if (ent->velocity.x != 0)
        {
            ent->velocity.x = ent->on_ground ? ent->velocity.x * 0.94 : ent->velocity.x * 0.98;
        }

        // Check if the entity isn't on the ground.
        if (!ent->on_ground)
        {
            // Apply gravity.
            ent->velocity.y = ent->velocity.y < 3 ? ent->velocity.y + 0.1 : 3;

            // Check if the entity is touching the ground.
            if (get_entity_collision_box(ent).y_bottom >= game_get_ground_level())
            {
                ent->on_ground = true;
                ent->position.y = game_get_ground_level();
                ent->velocity.y = 0;
            }
            else if (get_entity_collision_box(ent).y_bottom < 0)
            {
                ent->position.y = 0;
                ent->velocity.y = 0;
            }
        }

        // CHeck if the entity is moving up or down.
        if (ent->position.y < game_get_ground_level())
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
        if (i < MAX_GAME_OBJECTS) {
            // Get the game object.
            struct game_object *obj = &game_objects[i];

            // Check if the game object is active.
            if (obj->active)
            {
                if (obj->graphic != NULL)
                {
                    draw_graphic((int)(obj->position.x) - obj->width / 2, (int)(obj->position.y) - obj->height, obj->width, obj->height, obj->graphic);
                }
                else
                {
                    draw_rect((int)(obj->position.x) - obj->width / 2, (int)(obj->position.y) - obj->height, obj->width, obj->height);
                }
            }
        }

        if (i < MAX_ENTITIES)
        {
            // Get the entity.
            struct entity *ent = &entities[i];

            // Check if the entity is active.
            if (ent->active)
            {
                if (ent->graphic != NULL)
                {
                    draw_graphic((int)(ent->position.x) - ent->width / 2, (int)(ent->position.y) - ent->height, ent->width, ent->height, ent->graphic);
                }
                else
                {
                    draw_rect((int)(ent->position.x) - ent->width / 2, (int)(ent->position.y) - ent->height, ent->width, ent->height);
                }
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

/*
Interrupts
*/

void (*button_4_click)(void);
void (*button_3_click)(void);
void (*button_2_click)(void);

bool button_4_activated = false;
bool button_3_activated = false;
bool button_2_activated = false;

void user_isr(void)
{
    // Check if the Timer 2 interrupt flag is set.
	if (IFS(0) & 0x100)
	{   
        if (!game_paused) {
            // Increment the game uptime.
            game_uptime += 0.01;

            // Update the game.
            game_tick();
            game_draw();
        }
        IFSCLR(0) = 0x100;
	}

    // Get the button data.
    int buttonData = ((PORTD & 0xE0) >> 5);
    bool button_4_down = buttonData & 0b100;
    bool button_3_down = buttonData & 0b010;
    bool button_2_down = buttonData & 0b001;

    // Check if clicky button 4 is pressed.
    if (button_4_down && button_4_click != NULL && !button_4_activated)
    {
        button_4_click();
        button_4_activated = true;
    }
    else if(!button_4_down && button_4_activated) {
        button_4_activated = false;
    }

    // Check if clicky button 3 is pressed.
    if (button_3_down && button_3_click != NULL && !button_3_activated)
    {
        button_3_click();
        button_3_activated = true;
    }
    else if(!button_3_down && button_3_activated) {
        button_3_activated = false;
    }

    // Check if clicky button 2 is pressed.
    if (button_2_down && button_2_click != NULL && !button_2_activated)
    {
        button_2_click();
        button_2_activated = true;
    }
    else if(!button_2_down && button_2_activated) {
        button_2_activated = false;
    }
}