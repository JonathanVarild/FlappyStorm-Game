#include "gameengine.h"
#include <stdbool.h>
#include "scenes.h"
#include "player_manager.h"

// Create pointers for the title and menu items.
static Label *title;
static Label *menu_items[3];

// Create variable for the selected item.
static int selected_item;

// Function to select an item in the menu.
static void select_item()
{
    // Unload the player scene.
    unload_playerscene();

    // Check if the selected item is an empty slot or not.
    if (player_names[selected_item][0][0] == ' ')
    {
        // Load the input scene.
        load_inputscene(selected_item);
    }
    else
    {
        // Load the game scene.
        load_gamescene(selected_item);
    }
}

// Function to change the selected item in the menu.
static void go_up()
{
    // Set the selected item as not selected.
    set_label_selected(menu_items[selected_item], false);

    // Append 2 and make it cycle from 2 to 0.
    selected_item = (selected_item + 2) % 3;

    // Set the newly selected item as selected.
    set_label_selected(menu_items[selected_item], true);
}

// Function to change the selected item in the menu.
static void go_down()
{
    // Set the selected item as not selected.
    set_label_selected(menu_items[selected_item], false);

    // Append 1 and make it cycle from 2 to 0.
    selected_item = (selected_item + 1) % 3;

    // Set the newly selected item as selected.
    set_label_selected(menu_items[selected_item], true);
}

// Function to load the player scene.
void load_playerscene()
{
    // Set engine functions for buttons.
    button_4_click = select_item;
    button_3_click = go_up;
    button_2_click = go_down;

    // Set the selected item as 0.
    selected_item = 0;

    // Create the title label.
    title = create_label("Choose player:", (Vector2D){64, 0}, true, false);

    // Loop through the menu items.
    int i;
    for (i = 0; i < 3; i++)
    {
        // Create the menu items.
        menu_items[i] = create_label(player_names[i][0][0] == ' ' ? "Empty slot" : formatted_names[i], (Vector2D){64, 8 + i * 8}, true, i == 0);
    }
}

// Function to unload the game scene.
void unload_playerscene()
{
    // Reset the engine functions.
    button_4_click = NULL;
    button_3_click = NULL;
    button_2_click = NULL;
    on_game_tick = NULL;

    // Remove the title label.
    remove_label(title);

    // Loop through the menu items.
    int i;
    for (i = 0; i < 3; i++)
    {
        // Check if the menu item is active.
        if (menu_items[i] != NULL && menu_items[i]->active)
        {
            // Remove the menu item.
            remove_label(menu_items[i]);
        }
    }
}