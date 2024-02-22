#include <pic32mx.h>
#include "./screenengine.h"
#include "./utilities.h"

int main()
{

	display_init();

	while (1)
	{

		int i;
		for (i = 0; i < 118; i++)
		{

			matrix_reset();

			draw_rect(10, 10, i, 20);
			display_update();
			tick_delay(100000);
		}

		for (i = 118; i >= 0; i--)
		{

			matrix_reset();

			draw_rect(10, 10, i, 20);
			display_update();
			tick_delay(100000);
		}
	}

	for (;;)
		;
	return 0;
}
