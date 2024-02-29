#include "gameengine.h"
#include <stdbool.h>
#include "scenes.h"
#include "player_manager.h"

static Label *title;
static Label *rows[3];

static int highscore_order[3] = {0, 1, 2};
static char formatted_highscores[3][4] = {"000", "000", "000"};
static char highscore_rows[3][15] = {"1: ", "2: ", "3: "};

static int selected_item;

// Give these functions a bigger scope.
void(init_leaderboardscene)();
void(unload_leaderboardscene)();
void(update_leaderboardscene)();

// Function to get the current score.
void format_highscores()
{
    int i;
    for (i = 0; i < 3; i++)
    {
        // Calculate the score.
        int score = highscores[i];

        // Convert the score to a string.
        formatted_highscores[i][0] = '0' + (score / 100) % 10;
        formatted_highscores[i][1] = '0' + (score / 10) % 10;
        formatted_highscores[i][2] = '0' + (score / 1) % 10;
        formatted_highscores[i][3] = '\0';
    }
}

void calculate_highscore_order()
{
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = i + 1; j < 3; j++)
        {
            if (highscores[highscore_order[i]] < highscores[highscore_order[j]])
            {
                int temp = highscore_order[i];
                highscore_order[i] = highscore_order[j];
                highscore_order[j] = temp;
            }
        }
    }
}

void generate_highscore_rows()
{
    int i;
    for (i = 0; i < 3; i++)
    {
        int player_ID = highscore_order[i];

        // Make sure the player has a name.
        if (player_names[player_ID][0][0] == ' ')
        {
            continue;
        }

        int j = 0;
        while (j < 6 && formatted_names[player_ID][j] != '\0')
        {
            highscore_rows[i][3 + j] = formatted_names[player_ID][j];
            j++;
        }

        j += 3;
        highscore_rows[i][j] = ' ';

        j++;
        highscore_rows[i][j] = '(';

        int k;
        for (k = 0; k < 3; k++)
        {
            j++;
            highscore_rows[i][j] = formatted_highscores[player_ID][k];
        }

        j++;
        highscore_rows[i][j] = ')';

        j++;
        highscore_rows[i][j] = '\0';
    }
}

void go_back()
{
    unload_leaderboardscene();
    init_menuscene();
}

void init_leaderboardscene()
{
    // Set engine functions for buttons.
    button_4_click = go_back;
    button_3_click = go_back;
    button_2_click = go_back;

    // Calculate the highscore order.
    calculate_highscore_order();

    // Format the highscores.
    format_highscores();

    // Generate the highscore rows.
    generate_highscore_rows();

    // Create the title.
    title = create_label("Leaderboard", (Vector2D){64, 0}, true, false);

    int i;
    for (i = 0; i < 3; i++)
    {
        int player_ID = highscore_order[i];

        if (player_names[player_ID][0][0] == ' ')
        {
            continue;
        }

        // Create the highscore row.
        rows[i] = create_label(highscore_rows[i], (Vector2D){64, 8 + i * 8}, true, false);
    }
}

// Function to unload the game scene.
void unload_leaderboardscene()
{
    button_4_click = NULL;
    button_3_click = NULL;
    button_2_click = NULL;

    on_game_tick = NULL;

    // Remove the title label.
    remove_label(title);

    int i;
    for (i = 0; i < 3; i++)
    {
        if (rows[i] != NULL && rows[i]->active)
        {
            // Remove the highscore row.
            remove_label(rows[i]);
        }
    }
}