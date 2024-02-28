#include "gameengine.h"
#include <stdbool.h>
#include "icons.h"
#include "utilities.h"

// Give these functions a bigger scope.
void(init_inputscene)();
void(unload_inputscene)();
void(update_inputscene)();

static struct label *title;
static struct label *menu_items[6];
static int selected_item;

static char name[5][2] = {" ", " ", " ", " ", " "};
static char formatted_name[6];

static void select_item() {

    if (selected_item == 5) {
        unload_inputscene();
        init_menuscene();
    } else {
        if (name[selected_item][0] == ' ') {
            name[selected_item][0] = 'A';
        }
        else {
            name[selected_item][0] += 1;
        }
        if (name[selected_item][0] > 'Z') {
            name[selected_item][0] = ' ';
        }
        set_label_text(menu_items[selected_item], name[selected_item][0] == ' ' ? "-" : name[selected_item], false);
    }

}

static void go_left() {
    set_label_selected(menu_items[selected_item], false);
    selected_item = (selected_item + 5) % 6;
    set_label_selected(menu_items[selected_item], true);
}

static void go_right() {
    set_label_selected(menu_items[selected_item], false);
    selected_item = (selected_item + 1) % 6;
    set_label_selected(menu_items[selected_item ], true);
}

char* get_formatted_name() {
    
    int i;
    for (i = 0; i < 5; i++) {
        formatted_name[i] = name[i][0];
    }
    formatted_name[5] = '\0';
    return formatted_name;

}

void init_inputscene()
{
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