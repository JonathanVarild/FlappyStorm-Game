#ifndef GAME_H
#define GAME_H

void load_menuscene(void);
void unload_menuscene(void);

void load_gamescene(int selected_player_ID);
void unload_gamescene(void);

void load_inputscene(int selected_player_ID);
void unload_inputscene(void);

void load_playerscene(void);
void unload_playerscene(void);

void load_leaderboardscene(void);
void unload_leaderboardscene(void);

#endif