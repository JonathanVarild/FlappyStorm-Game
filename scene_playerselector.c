#include "gameengine.h"
#include <stdbool.h>
#include "scenes.h"
#include "player_manager.h"

static Label *title;
static Label *menu_items[3];

static int selected_item;

static void select_item()
{
    unload_playerscene();

    if (player_names[selected_item][0][0] == ' ')
    {
        init_inputscene(selected_item);
    }
    else
    {
        init_gamescene(selected_item);
    }
}

static void go_up()
{
    set_label_selected(menu_items[selected_item], false);
    selected_item = (selected_item + 2) % 3;
    set_label_selected(menu_items[selected_item], true);
}

static void go_down()
{
    set_label_selected(menu_items[selected_item], false);
    selected_item = (selected_item + 1) % 3;
    set_label_selected(menu_items[selected_item], true);
}

void init_playerscene()
{
    // Set engine functions for buttons.
    button_4_click = select_item;
    button_3_click = go_up;
    button_2_click = go_down;

    selected_item = 0;

    title = create_label("Choose player:", (Vector2D){64, 0}, true, false);

    int i;
    for (i = 0; i < 3; i++)
    {
        menu_items[i] = create_label(player_names[i][0][0] == ' ' ? "Empty slot" : formatted_names[i], (Vector2D){64, 8 + i * 8}, true, i == 0);
    }
}

// Function to unload the game scene.
void unload_playerscene()
{
    button_4_click = NULL;
    button_3_click = NULL;
    button_2_click = NULL;

    on_game_tick = NULL;

    // Remove the title label.
    remove_label(title);

    int i;
    // Remove the menu items.
    for (i = 0; i < 3; i++)
    {
        if (menu_items[i] != NULL && menu_items[i]->active)
        {
            remove_label(menu_items[i]);
        }
    }
}