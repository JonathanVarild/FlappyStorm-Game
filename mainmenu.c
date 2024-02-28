#include "gameengine.h"
#include <stdbool.h>
#include "icons.h"
#include "utilities.h"

struct label *title;
struct label *menu_items[2];

int selected_item;
double title_y;

// Give these functions a bigger scope.
void(init_menuscene)();
void(unload_menuscene)();
void(update_menuscene)();

void select_item() {
    if (selected_item == 0) {
        // Start game
        unload_menuscene();
        init_gamescene();
    } else if (selected_item == 1) {
        // Leaderboard
    }

}

void change_item() {
    selected_item = (selected_item + 1) % 2;

    int i;
    for (i = 0; i < 2; i++) {
        set_label_selected(menu_items[i], i == selected_item);
    }
}

// Function to update the game scene. This function is called every game tick.
void update_menuscene()
{

    if (title_y > 0 && get_game_uptime() > 2) {
        title_y -= 0.5;
        set_label_position(title, (struct vector2D){64, title_y});
    }
    else if ((int)(title_y) == 0 && selected_item == -1) {
        selected_item = 0;

        menu_items[0] = create_label("Start", (struct vector2D){64, 16}, true, true);
        menu_items[1] = create_label("Leaderboard", (struct vector2D){64, 24}, true, false);
    }
    
}

void init_menuscene()
{
    // Set engine functions for buttons.
    button_4_click = select_item;
    button_3_click = change_item;

    on_game_tick = update_menuscene;

    selected_item = -1;
    
    title_y = 12;

    title = create_label("FlappyStorm", (struct vector2D){64, title_y}, true, false);
}

// Function to unload the game scene.
void unload_menuscene()
{
    // Remove the title label.
    remove_label(title);
    int i;

    // Remove the menu items.
    for (i = 0; i < 2; i++) {
        if (menu_items[i] != NULL && menu_items[i]->active) {
            remove_label(menu_items[i]);
        }
    }
}