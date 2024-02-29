#include "gameengine.h"
#include "scenes.h"

// Main function where the game is launched.
int main()
{
	// Initialize game engine.
	game_init();

	// Load the menu scene.
	load_menuscene();

	// Set the application into an infinite loop.
	for (;;);

	// Return 0 for success.
	return 0;
}
