#include <pic32mx.h>
#include "./gameengine.h"
#include "./utilities.h"
#include <stdbool.h>
#include <stdlib.h>


int main()
{

	static uint8_t smiley[] = { 0x00, 0x00, 0x48, 0x00, 0x48, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x01, 0x02, 0x01, 0x01, 0xFE, 0x00, 0x00, 0x00, };

	game_init();

	struct entity* test_ent4 = create_entity((struct vector2D){45, 25}, 10, 10);
	struct entity* test_ent = create_entity((struct vector2D){45, 10}, 5, 5);
	struct entity* test_ent2 = create_entity((struct vector2D){10, 10}, 10, 10);
	struct entity* test_ent3 = create_entity((struct vector2D){80, 10}, 1, 1);
	struct entity* test_ent5 = create_entity((struct vector2D){100, 10}, 2, 2);
	struct entity* test_ent6 = create_entity((struct vector2D){100, 10}, 3, 3);

	set_entity_graphic(test_ent2, smiley);

	struct label* test_lbl = create_label("Cent+Sel", (struct vector2D){64, 0}, true, true);
	struct label* test_lbl2 = create_label("Normal", (struct vector2D){64, 10}, false, false);

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
