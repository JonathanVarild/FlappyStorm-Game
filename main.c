#include <pic32mx.h>
#include "./gameengine.h"
#include "./utilities.h"
#include <stdbool.h>
#include <stdlib.h>

struct entity* player;

void jump()
{
	set_entity_velocity(player, (struct vector2D){0, -3});
}

void go_left()
{
	set_entity_velocity(player, (struct vector2D){-1, 0});
}

void go_right()
{
	set_entity_velocity(player, (struct vector2D){1, 0});
}

int main()
{
	button_4_down = jump;
	button_3_down = go_left;
	button_2_down = go_right;

	static uint8_t smiley[] = { 0x00, 0x00, 0x48, 0x00, 0x48, 0x00, 0x48, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x01, 0x02, 0x01, 0x01, 0xFE, 0x00, 0x00, 0x00, };

	game_init();

	struct label* test_lbl = create_label("FlappyStorm", (struct vector2D){64, 0}, true, true);

	struct entity* reference_entity = create_entity((struct vector2D){45, 0}, 1, 1);

	player = create_entity((struct vector2D){0, 20}, 10, 10);
	set_entity_graphic(player, smiley);
	set_entity_velocity(player, (struct vector2D){10, 0});

	for (;;)
		;
	return 0;
}
