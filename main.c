#include <pic32mx.h>
#include "gameengine.h"
#include "utilities.h"
#include <stdbool.h>
#include <stdlib.h>
#include "scenes.h"

int main()
{
	game_init();

	init_gamescene();

	for (;;)
		;
	return 0;
}
