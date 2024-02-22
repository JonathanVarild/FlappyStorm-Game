#include <stdint.h>

uint8_t spi_send_recv(uint8_t data);
void display_init(void);
void display_update(void);
void draw_rect(int x, int y, int width, int height);