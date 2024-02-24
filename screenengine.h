#include <pic32mx.h>
#include <stdint.h>
#include "./utilities.h"
#include <stdbool.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

// Function prototypes
void display_init(void);
void display_update(void);
void matrix_reset(void);
void draw_pixel(int x, int y);
void draw_rect(int x, int y, int width, int height);
void draw_text(int x, int y, char *text, bool selected);