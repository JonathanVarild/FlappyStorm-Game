#ifndef GAME_H
#define GAME_H

#include "gameengine.h"
#include <stdbool.h>
#include "icons.h"

void init_gamescene(void);
void unload_gamescene(void);

void init_menuscene(void);
void unload_menuscene(void);

void init_inputscene(void);
void unload_inputscene(void);
char* get_formatted_name(void);

#endif