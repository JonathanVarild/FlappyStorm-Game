#include <stdbool.h>

char player_names[3][6][2] = {
    {" ", " ", " ", " ", " ", " "},
    {" ", " ", " ", " ", " ", " "},
    {" ", " ", " ", " ", " ", " "}
};
char formatted_names[3][6] = {"     ", "     ", "     "};

int highscores[3] = {0, 0, 0};

void format_name(int player_ID) {
    bool ending_space = true;

    int i;
    for (i = 4; i >= 0; i--) {
        if (player_names[player_ID][i][0] == ' ' && ending_space) {
            formatted_names[player_ID][i] = '\0';
        }
        else {
            formatted_names[player_ID][i] = player_names[player_ID][i][0];
            ending_space = false;
        }
    }
    formatted_names[player_ID][5] = '\0';
}

void set_highscore(int player_ID, int score) {
    highscores[player_ID] = score;
}