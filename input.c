#include "gameengine.h"
#include <stdbool.h>
#include "scenes.h"
#include "player_manager.h"

// Give these functions a bigger scope.
void(init_inputscene)(int selected_player_ID);
void(unload_inputscene)();
void(update_inputscene)();

static struct label *title;
static struct label *menu_items[6];
static int selected_item;
static int player_ID;

static void select_item() {

    if (selected_item == 5) {

        if (player_names[player_ID][0][0] == ' ') {
            set_label_text(title, "No lead space!", true);
        }
        else {
            format_name(player_ID);
            unload_inputscene();
            init_gamescene(player_ID);
        }

    } else {
        if (player_names[player_ID][selected_item][0] == ' ') {
            player_names[player_ID][selected_item][0] = 'A';
        }
        else {
            player_names[player_ID][selected_item][0] += 1;
        }
        if (player_names[player_ID][selected_item][0] > 'Z') {
            player_names[player_ID][selected_item][0] = ' ';
        }
        set_label_text(menu_items[selected_item], player_names[player_ID][selected_item][0] == ' ' ? "-" : player_names[player_ID][selected_item], false);
    }
}

static void go_left() {
    set_label_selected(menu_items[selected_item], false);
    selected_item = (selected_item + 5) % 6;
    set_label_selected(menu_items[selected_item], true);
    set_label_text(title, "Enter your name:", true);
}

static void go_right() {
    set_label_selected(menu_items[selected_item], false);
    selected_item = (selected_item + 1) % 6;
    set_label_selected(menu_items[selected_item ], true);
    set_label_text(title, "Enter your name:", true);
}

void init_inputscene(int selected_player_ID)
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
    title = create_label("Enter your name:", (struct vector2D){64, 0}, true, false);

    // Create the menu items.
    int i;
    for (i = 0; i < 5; i++) {
        menu_items[i] = create_label("-", (struct vector2D){44 + 8 * i, 16}, false, i == 0);
    }

    // Create the confirm button.
    menu_items[5] = create_label("Confirm", (struct vector2D){64, 24}, true, false);

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