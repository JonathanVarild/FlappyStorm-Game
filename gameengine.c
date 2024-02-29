#include "gameengine.h"
#include "screenengine.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/*
Object arrays
*/

// Create arrays for different objects.
static Game_object game_objects[MAX_GAME_OBJECTS] = {0};
static Entity entities[MAX_ENTITIES] = {0};
static Label labels[MAX_LABELS] = {0};

/*
Creation functions
*/

// Function used to create new game objects.
Game_object *create_game_object(Vector2D pos, int width, int height)
{
    // Loop through all game objects.
    int i;
    for (i = 0; i < MAX_GAME_OBJECTS; i++)
    {
        // Check if the game object is active.
        if (!game_objects[i].active)
        {
            // Create a new game object instance.
            game_objects[i].active = true;
            game_objects[i].is_visible = true;
            game_objects[i].position = pos;
            game_objects[i].width = width;
            game_objects[i].height = height;
            game_objects[i].age = get_game_uptime();

            // Return a pointer to the game object.
            return &game_objects[i];
        }
    }
}

// Function used to create new entities.
Entity *create_entity(Vector2D pos, int width, int height)
{
    // Loop through all entities.
    int i;
    for (i = 0; i < MAX_ENTITIES; i++)
    {
        // Check if the entity is active.
        if (!entities[i].active)
        {
            // Create a new entity instance.
            entities[i].active = true;
            entities[i].is_visible = true;
            entities[i].position = pos;
            entities[i].velocity.x = 0;
            entities[i].velocity.y = 0;
            entities[i].on_ground = false;
            entities[i].width = width;
            entities[i].height = height;
            entities[i].age = get_game_uptime();

            // Return a pointer to the entity.
            return &entities[i];
        }
    }
}

// Function used to create new labels.
Label *create_label(char *text, Vector2D pos, bool centered, bool selected)
{
    // Loop through all labels.
    int i;
    for (i = 0; i < MAX_LABELS; i++)
    {
        // Check if the label is active.
        if (!labels[i].active)
        {
            // Create a variable to calculate the pixel width of the text.
            int len = 0;

            // Check if the text should be centered.
            if (centered)
            {
                // Loop through all characters in the text.
                int j = 0;
                while (text[j] != '\0')
                {
                    // Add 8 to the length for each character.
                    len += 8;
                    j++;
                }
            }

            // Create a new label instance.
            labels[i].active = true;
            labels[i].text = text;
            labels[i].position = pos;
            labels[i].x_offset = len / 2;
            labels[i].selected = selected;

            // Return a pointer to the label.
            return &labels[i];
        }
    }
}

/*
Removal functions
*/

// Function used to remove game objects.
void remove_game_object(Game_object *obj)
{
    // Set the game object to inactive and remove its graphic.
    obj->active = false;
    obj->graphic = NULL;
}

// Function used to remove entities.
void remove_entity(Entity *ent)
{
    // Set the entity to inactive and remove its graphic.
    ent->active = false;
    ent->graphic = NULL;
}

// Function used to remove labels.
void remove_label(Label *lbl)
{
    // Set the label to inactive and remove its text.
    lbl->active = false;
    lbl->text = NULL;
}

/*
Setters
*/

// Function used to set the position of a game object.
void set_game_object_position(Game_object *obj, Vector2D pos)
{
    // Set the position of the game object.
    obj->position = pos;
}

// Function used to set the position of an entity.
void set_entity_position(Entity *ent, Vector2D pos)
{
    // Set the position of the entity.
    ent->position = pos;
}

// Function used to set the position of a label.
void set_label_position(Label *lbl, Vector2D pos)
{
    // Set the position of the label.
    lbl->position = pos;
}

// Function used to set the velocity of an entity.
void set_entity_velocity(Entity *ent, Vector2D vel)
{
    // Set the velocity of the entity.
    ent->velocity = vel;
}

// Function used to set the graphic of a game object.
void set_game_object_graphic(Game_object *obj, uint8_t *graphic)
{
    // Set the graphic of the game object.
    obj->graphic = graphic;
}

// Function used to set the graphic of an entity.
void set_entity_graphic(Entity *ent, uint8_t *graphic)
{
    // Set the graphic of the entity.
    ent->graphic = graphic;
}

// Function used to set the selected state of a label.
void set_label_selected(Label *lbl, bool selected)
{
    // Set the selected state of the label.
    lbl->selected = selected;
}

// Function used to set the text of a label.
void set_label_text(Label *lbl, char *text, bool centered)
{
    // Set the text of the label.
    lbl->text = text;

    // Check if the text should be centered.
    if (centered)
    {
        // Create a variable to calculate the pixel width of the text.
        int len = 0;

        // Loop through all characters in the text.
        int j = 0;
        while (text[j] != '\0')
        {
            // Add 8 to the length for each character.
            len += text[j] == ' ' ? 4 : 8;
            j++;
        }

        // Set the x offset of the label.
        lbl->x_offset = len / 2;
    }
    else
    {
        // Set the x offset of the label to 0.
        lbl->x_offset = 0;
    }
}

// Function used to set the type of an entity.
void set_entity_type(Entity *ent, int type)
{
    // Set the type of the entity.
    ent->type = type;
}

// Function used to set the type of a game object.
void set_game_object_type(Game_object *obj, int type)
{
    // Set the type of the game object.
    obj->type = type;
}

// Function used to set the visibility of an entity.
void set_entity_visibility(Entity *ent, bool visible)
{
    // Set the visibility of the entity.
    ent->is_visible = visible;
}

// Function used to set the visibility of a game object.
void set_game_object_visibility(Game_object *obj, bool visible)
{
    // Set the visibility of the game object.
    obj->is_visible = visible;
}

/*
Getters
*/

// Function used to get the collision box of a game object.
void get_game_object_collision_box(Collision_box *box, Game_object *obj)
{
    // Set the collision box values.
    box->x_left = obj->position.x;
    box->x_right = obj->position.x + obj->width;
    box->y_top = obj->position.y - obj->height;
    box->y_bottom = obj->position.y;
}

// Function used to get the collision box of an entity.
void get_entity_collision_box(Collision_box *box, Entity *ent)
{
    // Set the collision box values.
    box->x_left = ent->position.x;
    box->x_right = ent->position.x + ent->width;
    box->y_top = ent->position.y - ent->height;
    box->y_bottom = ent->position.y;
}

/*
Engine functions
*/

// Variables for engine functions.
static double game_uptime = 0;
static bool game_paused = false;
static int ground_level = 32;

// Function used to get the uptime of the game.
double get_game_uptime()
{
    // Return the uptime of the game.
    return game_uptime;
}

// Function used to pause the game tick function.
void set_game_paused(bool paused)
{
    // Set the game paused variable.
    game_paused = paused;
}

// Function used to get the state of the game.
bool get_game_paused()
{
    // Return the state of the game.
    return game_paused;
}

// Function used to set the ground level of the game.
void game_set_ground_level(int level)
{
    // Set the ground level of the game.
    ground_level = level;
}

// Function used to get the ground level of the game.
int game_get_ground_level()
{
    // Return the ground level of the game.
    return ground_level;
}

// Function used to initialize the game.
void game_init()
{
    // Initialize the display.
    display_init();

    // Set up clicky buttons as inputs.
    TRISDSET = 0x7F0;

    // Set up Timer 2 interrupt with a period of 0.005 seconds. (200 fps / ticks per second)
    T2CON = 0x0;       // Stop existing timer2 and clear control register
    T2CONSET = 0x70;   // Set prescaler to 1:256
    PR2 = 3125;        // Set period register to ((0.02 * 40000000) / 256) = 3125
    TMR2 = 0x0;        // Clear the timer register
    IFSCLR(0) = 0x100; // Clear the Timer 2 interrupt flag
    IECSET(0) = 0x100; // Enable Timer 2 interrupts
    T2CONSET = 0x8000; // Start Timer 2
    IPCSET(2) = 0x1A;  // Set priority level to 7 (0001 1100)

    // Enable interrupts.
    enable_interrupt();
}

// Function used to update the game.
void game_tick()
{
    // Call on_game_tick function if there is any.
    if (on_game_tick != NULL)
    {
        on_game_tick();
    }

    // Loop through all entities.
    int i;
    for (i = 0; i < MAX_ENTITIES; i++)
    {

        // Get the entity.
        Entity *ent = &entities[i];

        // Check if the entity is active.
        if (!ent->active)
        {
            continue;
        }

        // Update the position of the entity.
        ent->position.x += ent->velocity.x * 0.15;
        ent->position.y += ent->velocity.y * 0.15;

        // Check if x velocity isn't 0.
        if (ent->velocity.x != 0)
        {
            ent->velocity.x = ent->on_ground ? ent->velocity.x * 0.92 : ent->velocity.x * 0.96;
        }

        // Check if the entity isn't on the ground.
        if (!ent->on_ground)
        {
            // Apply gravity.
            ent->velocity.y = ent->velocity.y < 3 ? ent->velocity.y + 0.2 : 3;

            Collision_box ent_box;
            get_entity_collision_box(&ent_box, ent);

            // Check if the entity is touching the ground.
            if (ent_box.y_bottom >= game_get_ground_level())
            {
                // Set the entity on the ground.
                ent->on_ground = true;

                // Set the entity position to the ground level and remove the y velocity.
                ent->position.y = game_get_ground_level();
                ent->velocity.y = 0;
            }
            else if (ent_box.y_top < 0)
            {
                // Set the entity position to the top level and remove the y velocity.
                ent->position.y = ent->height;
                ent->velocity.y = 0;
            }
        }

        // CHeck if the entity is moving up or down.
        if (ent->position.y < game_get_ground_level())
        {
            // Set the entity as not on the ground.
            ent->on_ground = false;
        }
    }
}

// Function used to draw the game.
void game_draw()
{
    // Clear the display.
    clear_display();

    // Loop through all entities.
    int i;
    for (i = 0; i < DRAW_LOOP; i++)
    {
        // Check if we should draw a game object.
        if (i < MAX_GAME_OBJECTS)
        {
            // Get the game object.
            Game_object *obj = &game_objects[i];

            // Check if the game object is active and visible.
            if (obj->active && obj->is_visible)
            {
                // Check if the game object has a graphic.
                if (obj->graphic != NULL)
                {
                    // Draw the graphic of the game object.
                    draw_graphic((int)(obj->position.x) - obj->width / 2, (int)(obj->position.y) - obj->height, obj->width, obj->height, obj->graphic);
                }
                else
                {
                    // Draw a rectangle for the game object.
                    draw_rect((int)(obj->position.x) - obj->width / 2, (int)(obj->position.y) - obj->height, obj->width, obj->height);
                }
            }
        }

        // Check if we should draw an entity.
        if (i < MAX_ENTITIES)
        {
            // Get the entity.
            Entity *ent = &entities[i];

            // Check if the entity is active.
            if (ent->active && ent->is_visible)
            {
                // Check if the entity has a graphic.
                if (ent->graphic != NULL)
                {
                    // Draw the graphic of the entity.
                    draw_graphic((int)(ent->position.x) - ent->width / 2, (int)(ent->position.y) - ent->height, ent->width, ent->height, ent->graphic);
                }
                else
                {
                    // Draw a rectangle for the entity.
                    draw_rect((int)(ent->position.x) - ent->width / 2, (int)(ent->position.y) - ent->height, ent->width, ent->height);
                }
            }
        }

        // Check if we should draw a label.
        if (i < MAX_LABELS)
        {
            // Get the label.
            Label *lbl = &labels[i];

            // Check if the label is active.
            if (lbl->active)
            {
                // Draw the text of the label.
                draw_text((int)(lbl->position.x) - lbl->x_offset, (int)(lbl->position.y), lbl->text, lbl->selected);
            }
        }
    }

    // Update the display.
    display_update();
}

/*
Interrupts
*/

// Variables to remember if the buttons have been activated.
static bool button_4_activated = false;
static bool button_3_activated = false;
static bool button_2_activated = false;

// Function used to handle interrupts.
void user_isr(void)
{
    // Check if the Timer 2 interrupt flag is set.
    if (IFS(0) & 0x100)
    {
        // Check if the game is paused.
        if (!game_paused)
        {
            // Increment the game uptime.
            game_uptime += 0.02;

            // Update the game.
            game_tick();
            game_draw();
        }

        // Get the button data.
        int buttonData = ((PORTD & 0xE0) >> 5);
        bool button_4_down = buttonData & 0b100;
        bool button_3_down = buttonData & 0b010;
        bool button_2_down = buttonData & 0b001;

        // Check if button 4 is pressed and has a assigned function.
        if (button_4_down && button_4_click != NULL && !button_4_activated)
        {
            // Call the assigned function. 
            button_4_click();

            // Set the button as activated.
            button_4_activated = true;
        }
        else if (!button_4_down && button_4_activated)
        {
            // Set the button as not activated if its released.
            button_4_activated = false;
        }

        // Check if button 3 is pressed and has a assigned function.
        if (button_3_down && button_3_click != NULL && !button_3_activated)
        {
            // Call the assigned function.
            button_3_click();

            // Set the button as activated.
            button_3_activated = true;
        }
        else if (!button_3_down && button_3_activated)
        {
            // Set the button as not activated if its released.
            button_3_activated = false;
        }

        // Check if clicky button 2 is pressed.
        if (button_2_down && button_2_click != NULL && !button_2_activated)
        {
            // Call the assigned function.
            button_2_click();

            // Set the button as activated.
            button_2_activated = true;
        }
        else if (!button_2_down && button_2_activated)
        {
            // Set the button as not activated if its released.
            button_2_activated = false;
        }

        // Clear the Timer 2 interrupt flag.
        IFSCLR(0) = 0x100;
    }
}