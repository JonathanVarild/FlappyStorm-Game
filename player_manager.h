#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

#include <stdbool.h>

// Arrays for storing player names and highscores.
char player_names[3][5][2];
char formatted_names[3][6];
int highscores[3];

void format_name(int player_ID);
void set_highscore(int player_ID, int score);

#endif