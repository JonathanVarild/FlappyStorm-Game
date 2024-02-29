#include "gameengine.h"
#include <stdbool.h>
#include "scenes.h"


// Create pointers for the title and menu items.
static Label *title;
static Label *menu_items[2];

// Create variable for the selected item and the title position.
static int selected_item;
static double title_y;

// Function to select an item in the menu.
static void select_item()
{
    // Check if we have selected "Start" or "Leaderboard".
    if (selected_item == 0)
    {
        // Unload the menu scene and load the player scene.
        unload_menuscene();
        load_playerscene();
    }
    else if (selected_item == 1)
    {
        // Unload the menu scene and load the leaderboard scene.
        unload_menuscene();
        load_leaderboardscene();
    }
}

// Function to change the selected item in the menu.
static void change_item()
{
    // Set the selected item as not selected.
    set_label_selected(menu_items[selected_item], false);

    // Append 1 and make it cycle from 1 to 0.
    selected_item = (selected_item + 1) % 2;

    // Set the newly selected item as selected.
    set_label_selected(menu_items[selected_item], true);
}

// Function to update the game scene. This function is called every game tick.
void update_menuscene()
{
    // Check if the title should move up.
    if (title_y > 0 && get_game_uptime() > 2)
    {
        // Decrease the title position and set the new position.
        title_y -= 0.5;
        set_label_position(title, (Vector2D){64, title_y});
    }
    else if ((int)(title_y) == 0 && selected_item == -1)
    {
        // Set the selected item as the first item.
        selected_item = 0;

        // Create the menu items.
        menu_items[0] = create_label("Start", (Vector2D){64, 16}, true, true);
        menu_items[1] = create_label("Leaderboard", (Vector2D){64, 24}, true, false);

        // Set the engine functions for the buttons.
        button_4_click = select_item;
        button_3_click = change_item;
        button_2_click = change_item;
    }
}

// Function to load the menu scene.
void load_menuscene()
{
    // Set engine functions for buttons.
    on_game_tick = update_menuscene;

    // Set the selected item to -1.
    selected_item = -1;

    // Set the title position.
    title_y = 12;

    // Create the title label.
    title = create_label("FlappyStorm", (Vector2D){64, title_y}, true, false);
}

// Function to unload the game scene.
void unload_menuscene()
{
    // Reset all engine functions.
    button_4_click = NULL;
    button_3_click = NULL;
    button_2_click = NULL;
    on_game_tick = NULL;

    // Remove the title label.
    remove_label(title);

    // Loop through the menu items.
    int i;
    for (i = 0; i < 2; i++)
    {
        // Check if the menu item is active.
        if (menu_items[i] != NULL && menu_items[i]->active)
        {
            // Remove the menu item.
            remove_label(menu_items[i]);
        }
    }
}