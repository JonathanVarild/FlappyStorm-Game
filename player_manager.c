#include <stdbool.h>

// Create a 3D array for player names.
// The first dimension is the player ID.
// The second dimension is the name length.
// The third dimension is the character. (Only 1 character is used)
char player_names[3][5][2] = {
    {" ", " ", " ", " ", " "},
    {" ", " ", " ", " ", " "},
    {" ", " ", " ", " ", " "}
};

// Create a 2D array for formatted player names.
char formatted_names[3][6] = {"     ", "     ", "     "};

// Create an array to store all highscores.
int highscores[3] = {0, 0, 0};

// Function to format the name of a player.
void format_name(int player_ID) {

    // Variable to store if the name has an ending space.
    bool ending_space = true;

    // Loop through the unformatted name.
    int i;
    for (i = 4; i >= 0; i--) {
        // Check if the current char is a space and if the name has an ending space.
        if (player_names[player_ID][i][0] == ' ' && ending_space) {
            // Add null terminator.
            formatted_names[player_ID][i] = '\0';
        }
        else {
            // Add the character to the formatted name.
            formatted_names[player_ID][i] = player_names[player_ID][i][0];

            // Set ending space to false.
            ending_space = false;
        }
    }

    // Add null terminator to the end of the formatted name.
    formatted_names[player_ID][5] = '\0';
}

// Function to set the highscore of a player.
void set_highscore(int player_ID, int score) {
    // Set the highscore of the player.
    highscores[player_ID] = score;
}