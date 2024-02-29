#include "gameengine.h"
#include <stdbool.h>
#include "scenes.h"
#include "player_manager.h"

// Create pointers for the title and rows.
static Label *title;
static Label *rows[3];

// Create an array to store the highscore data.
static int highscore_order[3] = {0, 1, 2};
static char formatted_highscores[3][4] = {"000", "000", "000"};
static char highscore_rows[3][15] = {"1: ", "2: ", "3: "};

// Function to format highscores into strings.
static void format_highscores()
{
    // Loop through the highscores.
    int i;
    for (i = 0; i < 3; i++)
    {
        // Get the highscore.
        int score = highscores[i];

        // Convert the score to a string.
        formatted_highscores[i][0] = '0' + (score / 100) % 10;
        formatted_highscores[i][1] = '0' + (score / 10) % 10;
        formatted_highscores[i][2] = '0' + (score / 1) % 10;
        formatted_highscores[i][3] = '\0';
    }
}

// Function to calculate the highscore order.
static void calculate_highscore_order()
{
    // Loop through the highscores.
    int i, j;
    for (i = 0; i < 3; i++)
    {
        for (j = i + 1; j < 3; j++)
        {
            // Check if the current highscore is less than the compared highscore.
            if (highscores[highscore_order[i]] < highscores[highscore_order[j]])
            {
                // Swap the highscore order.
                int temp = highscore_order[i];
                highscore_order[i] = highscore_order[j];
                highscore_order[j] = temp;
            }
        }
    }
}

// Function to generate the highscore rows.
static void generate_highscore_rows()
{
    // Loop through the highscore order.
    int i;
    for (i = 0; i < 3; i++)
    {
        // Get the player ID.
        int player_ID = highscore_order[i];

        // Make sure the player has a name.
        if (player_names[player_ID][0][0] == ' ')
        {
            continue;
        }

        // Loop through the formatted name.
        int j = 0;
        while (j < 6 && formatted_names[player_ID][j] != '\0')
        {
            // Add the character to the highscore row.
            highscore_rows[i][3 + j] = formatted_names[player_ID][j];
            j++;
        }

        // Append a space to the end of the name.
        j += 3;
        highscore_rows[i][j] = ' ';

        // Append a parenthesis.
        j++;
        highscore_rows[i][j] = '(';

        // Loop through the formatted highscore.
        int k;
        for (k = 0; k < 3; k++)
        {
            // Add the highscore to the highscore row.
            j++;
            highscore_rows[i][j] = formatted_highscores[player_ID][k];
        }

        // Append a parenthesis.
        j++;
        highscore_rows[i][j] = ')';

        // Add null terminator.
        j++;
        highscore_rows[i][j] = '\0';
    }
}

// Function to go back to the menu scene.
static void go_back()
{
    // Unload the leaderboard scene and load the menu scene.
    unload_leaderboardscene();
    load_menuscene();
}

// Function to load the leaderboard scene.
void load_leaderboardscene()
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

    // Loop through the highscore order.
    int i;
    for (i = 0; i < 3; i++)
    {
        // Get the player ID.
        int player_ID = highscore_order[i];

        // Make sure the player has a name.
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
    // Reset the engine functions.
    button_4_click = NULL;
    button_3_click = NULL;
    button_2_click = NULL;
    on_game_tick = NULL;

    // Remove the title label.
    remove_label(title);

    // Loop through the rows.
    int i;
    for (i = 0; i < 3; i++)
    {
        // Check if the row is active.
        if (rows[i] != NULL && rows[i]->active)
        {
            // Remove the highscore row.
            remove_label(rows[i]);
        }
    }
}