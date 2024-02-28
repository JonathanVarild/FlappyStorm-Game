#ifndef GAME_H
#define GAME_H

#include "gameengine.h"
#include <stdbool.h>
#include "icons.h"

void init_menuscene(void);
void unload_menuscene(void);


void init_gamescene(int player_ID);
void unload_gamescene(void);

void init_inputscene(int selected_player_ID);
void unload_inputscene(void);

void init_playerscene(void);
void unload_playerscene(void);

#endif