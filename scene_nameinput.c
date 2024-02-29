#include "gameengine.h"
#include <stdbool.h>
#include "scenes.h"
#include "player_manager.h"

// Create pointers for the title and menu items.
static Label *title;
static Label *menu_items[6];

// Create variable for the selected item and the player ID.
static int selected_item;
static int player_ID;

// Function to select an item in the menu.
static void select_item() {

    // Check if we have selected "Confirm".
    if (selected_item == 5) {

        // Check if the name has a leading space.
        if (player_names[player_ID][0][0] == ' ') {
            // Set the title text to "No lead space!".
            set_label_text(title, "No lead space!", true);
        }
        else {
            // Format the name and load the game scene.
            format_name(player_ID);
            unload_inputscene();
            load_gamescene(player_ID);
        }

    } else {
        // Check what char to change to.
        if (player_names[player_ID][selected_item][0] == ' ') {
            // Change to an A if the char is a space.
            player_names[player_ID][selected_item][0] = 'A';
        }
        else {
            // Increase the char by 1.
            player_names[player_ID][selected_item][0] += 1;
        }

        // Check if the char is larger than 'Z'.
        if (player_names[player_ID][selected_item][0] > 'Z') {
            // Change the char to a space.
            player_names[player_ID][selected_item][0] = ' ';
        }

        // Set the label text to the new char. (Display spaces as "-")
        set_label_text(menu_items[selected_item], player_names[player_ID][selected_item][0] == ' ' ? "-" : player_names[player_ID][selected_item], false);
    }
}

// Function to move to the left in the menu.
static void go_left() {
    // Unselect the current item.
    set_label_selected(menu_items[selected_item], false);

    // Add 5 to make it cycle from 0 to 5.
    selected_item = (selected_item + 5) % 6;

    // Select the new item.
    set_label_selected(menu_items[selected_item], true);

    // Set the title text to "Enter your name:" if it may have been changed.
    set_label_text(title, "Enter your name:", true);
}

// Function to move to the right in the menu.
static void go_right() {
    // Unselect the current item.
    set_label_selected(menu_items[selected_item], false);

    // Add 1 to make it cycle from 5 to 0.
    selected_item = (selected_item + 1) % 6;

    // Select the new item.
    set_label_selected(menu_items[selected_item ], true);

    // Set the title text to "Enter your name:" if it may have been changed.
    set_label_text(title, "Enter your name:", true);
}

// Function to load the input scene.
void load_inputscene(int selected_player_ID)
{
    // Set the player_ID.
    player_ID = selected_player_ID;

    // Set engine functions for buttons.
    button_4_click = select_item;
    button_3_click = go_left;
    button_2_click = go_right;

    // Set the selected item to 0.
    selected_item = 0;

    // Create the title label.
    title = create_label("Enter your name:", (Vector2D){64, 0}, true, false);

    // Create the menu items.
    int i;
    for (i = 0; i < 5; i++) {
        menu_items[i] = create_label("-", (Vector2D){44 + 8 * i, 16}, false, i == 0);
    }

    // Create the confirm button.
    menu_items[5] = create_label("Confirm", (Vector2D){64, 24}, true, false);
}

// Function to unload the game scene.
void unload_inputscene()
{
    // Reset engine functions.
    button_4_click = NULL;
    button_3_click = NULL;
    button_2_click = NULL;

    // Remove the title label.
    remove_label(title);

    // Remove the menu items.
    int i;
    for (i = 0; i < 6; i++) {
        remove_label(menu_items[i]);
    }

    // Reset the selected item.
    selected_item = 0;
}