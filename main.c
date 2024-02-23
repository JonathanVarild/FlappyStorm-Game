#include <pic32mx.h>
#include "./gameengine.h"
#include "./utilities.h"
#include "./screenengine.h"

int main()
{

	game_init();

	struct entity* test_ent = create_entity((struct vector2D){45, 10}, 5, 5);
	struct entity* test_ent2 = create_entity((struct vector2D){10, 10}, 10, 10);
	struct entity* test_ent3 = create_entity((struct vector2D){80, 10}, 1, 1);

	while (1)
	{
		game_tick();
		game_draw();
		tick_delay(50000);
	}

	for (;;)
		;
	return 0;
}
