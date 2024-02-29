#ifndef SCREENENGINE_H
#define SCREENENGINE_H

#include <pic32mx.h>
#include <stdint.h>
#include "utilities.h"
#include <stdbool.h>

// Define the display dimensions.
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

void display_init(void);
void display_update(void);
void clear_display(void);
void draw_pixel(int x, int y);
void draw_rect(int x, int y, int width, int height);
void draw_text(int x, int y, char *text, bool selected);
void draw_graphic(int x, int y, int width, int height, uint8_t *graphic);

#endif