#include <pic32mx.h>
#include "./gameengine.h"
#include "./utilities.h"
#include <stdbool.h>
#include <stdlib.h>

struct entity *player;

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
	button_4_click = jump;
	button_3_click = go_left;
	button_2_click = go_right;

	static uint8_t smiley[] = {
		0x00,
		0x00,
		0x48,
		0x00,
		0x48,
		0x00,
		0x48,
		0x00,
		0x00,
		0x00,
		0x00,
		0x00,
		0x01,
		0x02,
		0x01,
		0x01,
		0xFE,
		0x00,
		0x00,
		0x00,
	};

	static uint8_t bird[] = {
		0x00,
		0x00,
		0x00,
		0x00,
		0xF8,
		0x01,
		0x04,
		0x02,
		0x9E,
		0x04,
		0xA1,
		0x04,
		0x21,
		0x08,
		0x11,
		0x0F,
		0x8A,
		0x08,
		0x04,
		0x07,
		0xF8,
		0x00,
		0x00,
		0x00,
	};

	static uint8_t smaller_bird[] = {
		0xFC,
		0x00,
		0x02,
		0x01,
		0x4D,
		0x02,
		0x51,
		0x02,
		0x11,
		0x04,
		0x89,
		0x07,
		0x46,
		0x04,
		0x82,
		0x03,
		0x7C,
		0x00,
	};

	static uint8_t smallest_bird[] = {
		0x7C,
		0x00,
		0x86,
		0x00,
		0x29,
		0x01,
		0x09,
		0x01,
		0xC9,
		0x03,
		0x27,
		0x02,
		0xC2,
		0x01,
		0x3C,
		0x00
	};

	game_init();

	struct label *test_lbl = create_label("FlappyStorm", (struct vector2D){64, 0}, true, true);

	struct entity *reference_entity = create_entity((struct vector2D){45, 0}, 1, 1);

	player = create_entity((struct vector2D){0, 20}, 10, 8);
	set_entity_graphic(player, smallest_bird);
	set_entity_velocity(player, (struct vector2D){10, 0});

	for (;;)
		;
	return 0;
}
