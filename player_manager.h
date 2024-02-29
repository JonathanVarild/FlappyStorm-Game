#ifndef PLAYER_MANAGER_H
#define PLAYER_MANAGER_H

char player_names[3][6][2];
char formatted_names[3][6];

int highscores[3];

void format_name(int player_ID);
void set_highscore(int player_ID, int score);

#endif