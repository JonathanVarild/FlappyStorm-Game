#include <pic32mx.h>
#include <stdint.h>
#include "./utilities.h"
#include <stdbool.h>

#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 32

// #define DISPLAY_VDD PORTFbits.RF6
// #define DISPLAY_VBATT PORTFbits.RF5
// #define DISPLAY_COMMAND_DATA PORTFbits.RF4
// #define DISPLAY_RESET PORTGbits.RG9

// Screen definitions.
#define DISPLAY_VDD_PORT PORTF
#define DISPLAY_VDD_MASK 0x40
#define DISPLAY_VBATT_PORT PORTF
#define DISPLAY_VBATT_MASK 0x20
#define DISPLAY_COMMAND_DATA_PORT PORTF
#define DISPLAY_COMMAND_DATA_MASK 0x10
#define DISPLAY_RESET_PORT PORTG
#define DISPLAY_RESET_MASK 0x200

uint8_t spi_send_recv(uint8_t data)
{
    while (!(SPI2STAT & 0x08))
        ;
    SPI2BUF = data;
    while (!(SPI2STAT & 0x01))
        ;
    return SPI2BUF;
}

void display_init()
{
    /* Set up peripheral bus clock */
    OSCCON &= ~0x180000;
    OSCCON |= 0x080000;

    /* Set up output pins */
    AD1PCFG = 0xFFFF;
    ODCE = 0x0;
    TRISECLR = 0xFF;
    PORTE = 0x0;

    /* Output pins for display signals */
    PORTF = 0xFFFF;
    PORTG = (1 << 9);
    ODCF = 0x0;
    ODCG = 0x0;
    TRISFCLR = 0x70;
    TRISGCLR = 0x200;

    /* Set up input pins */
    TRISDSET = (1 << 8);
    TRISFSET = (1 << 1);

    /* Set up SPI as master */
    SPI2CON = 0;
    SPI2BRG = 4;

    /* Clear SPIROV*/
    SPI2STATCLR &= ~0x40;
    /* Set CKP = 1, MSTEN = 1; */
    SPI2CON |= 0x60;

    /* Turn on SPI */
    SPI2CONSET = 0x8000;

    DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK;
    tick_delay(10);
    DISPLAY_VDD_PORT &= ~DISPLAY_VDD_MASK;
    tick_delay(1000000);

    spi_send_recv(0xAE);
    DISPLAY_RESET_PORT &= ~DISPLAY_RESET_MASK;
    tick_delay(10);
    DISPLAY_RESET_PORT |= DISPLAY_RESET_MASK;
    tick_delay(10);

    spi_send_recv(0x8D);
    spi_send_recv(0x14);

    spi_send_recv(0xD9);
    spi_send_recv(0xF1);

    DISPLAY_VBATT_PORT &= ~DISPLAY_VBATT_MASK;
    tick_delay(10000000);

    spi_send_recv(0xA1);
    spi_send_recv(0xC8);

    spi_send_recv(0xDA);
    spi_send_recv(0x20);

    spi_send_recv(0xAF);
}

uint8_t display_matrix[4][128] = {0};

void display_update()
{
    int i, j;
    for (i = 0; i < 4; i++)
    {
        DISPLAY_COMMAND_DATA_PORT &= ~DISPLAY_COMMAND_DATA_MASK; // Förbereder nytt kommando
        spi_send_recv(0x22);                                     // Väljer den aktuella raden som ska uppdateras. 0x22 är kommandot för att sätta radadressen.
        spi_send_recv(i);                                        // Vilken rad som ska uppdateras.

        spi_send_recv(0x0);  // ????
        spi_send_recv(0x10); // ????

        DISPLAY_COMMAND_DATA_PORT |= DISPLAY_COMMAND_DATA_MASK; // Förbereder nytt data

        for (j = 0; j < 128; j++)
        {
            spi_send_recv(display_matrix[i][j]);
        }
    }
}

matrix_reset()
{
    // Reset the displaymatrix to all zeros.
    int j, k;
    for (j = 0; j < 4; j++)
    {
        for (k = 0; k < 128; k++)
        {
            display_matrix[j][k] = 0;
        }
    }
}

void draw_pixel(int x, int y)
{
    
    int segment = y / 8;
    int columnData = 1 << (y % 8);
    display_matrix[segment][x] |= columnData;
}

void draw_rect(int x, int y, int width, int height)
{

    // Set i and j.
    int i, j;

    // Loop through the rows pixel by pixel.
    for (i = y; i < y + height; i++)
    {
        for (j = x; j < x + width; j++)
        {
            draw_pixel(j, i);
        }
    }
}

uint8_t text_font[] = {
	0, 0, 0, 94, 0, 0, 0, 0,
	0, 0, 4, 3, 4, 3, 0, 0,
	0, 36, 126, 36, 36, 126, 36, 0,
	0, 36, 74, 255, 82, 36, 0, 0,
	0, 70, 38, 16, 8, 100, 98, 0,
	0, 52, 74, 74, 52, 32, 80, 0,
	0, 0, 0, 4, 3, 0, 0, 0,
	0, 0, 0, 126, 129, 0, 0, 0,
	0, 0, 0, 129, 126, 0, 0, 0,
	0, 42, 28, 62, 28, 42, 0, 0,
	0, 8, 8, 62, 8, 8, 0, 0,
	0, 0, 0, 128, 96, 0, 0, 0,
	0, 8, 8, 8, 8, 8, 0, 0,
	0, 0, 0, 0, 96, 0, 0, 0,
	0, 64, 32, 16, 8, 4, 2, 0,
	0, 62, 65, 73, 65, 62, 0, 0,
	0, 0, 66, 127, 64, 0, 0, 0,
	0, 0, 98, 81, 73, 70, 0, 0,
	0, 0, 34, 73, 73, 54, 0, 0,
	0, 0, 14, 8, 127, 8, 0, 0,
	0, 0, 35, 69, 69, 57, 0, 0,
	0, 0, 62, 73, 73, 50, 0, 0,
	0, 0, 1, 97, 25, 7, 0, 0,
	0, 0, 54, 73, 73, 54, 0, 0,
	0, 0, 6, 9, 9, 126, 0, 0,
	0, 0, 0, 102, 0, 0, 0, 0,
	0, 0, 128, 102, 0, 0, 0, 0,
	0, 0, 8, 20, 34, 65, 0, 0,
	0, 0, 20, 20, 20, 20, 0, 0,
	0, 0, 65, 34, 20, 8, 0, 0,
	0, 2, 1, 81, 9, 6, 0, 0,
	0, 28, 34, 89, 89, 82, 12, 0,
	0, 0, 126, 9, 9, 126, 0, 0,
	0, 0, 127, 73, 73, 54, 0, 0,
	0, 0, 62, 65, 65, 34, 0, 0,
	0, 0, 127, 65, 65, 62, 0, 0,
	0, 0, 127, 73, 73, 65, 0, 0,
	0, 0, 127, 9, 9, 1, 0, 0,
	0, 0, 62, 65, 81, 50, 0, 0,
	0, 0, 127, 8, 8, 127, 0, 0,
	0, 0, 65, 127, 65, 0, 0, 0,
	0, 0, 32, 64, 64, 63, 0, 0,
	0, 0, 127, 8, 20, 99, 0, 0,
	0, 0, 127, 64, 64, 64, 0, 0,
	0, 127, 2, 4, 2, 127, 0, 0,
	0, 127, 6, 8, 48, 127, 0, 0,
	0, 0, 62, 65, 65, 62, 0, 0,
	0, 0, 127, 9, 9, 6, 0, 0,
	0, 0, 62, 65, 97, 126, 64, 0,
	0, 0, 127, 9, 9, 118, 0, 0,
	0, 0, 38, 73, 73, 50, 0, 0,
	0, 1, 1, 127, 1, 1, 0, 0,
	0, 0, 63, 64, 64, 63, 0, 0,
	0, 31, 32, 64, 32, 31, 0, 0,
	0, 63, 64, 48, 64, 63, 0, 0,
	0, 0, 119, 8, 8, 119, 0, 0,
	0, 3, 4, 120, 4, 3, 0, 0,
	0, 0, 113, 73, 73, 71, 0, 0,
	0, 0, 127, 65, 65, 0, 0, 0,
	0, 2, 4, 8, 16, 32, 64, 0,
	0, 0, 0, 65, 65, 127, 0, 0,
	0, 4, 2, 1, 2, 4, 0, 0,
	0, 64, 64, 64, 64, 64, 64, 0,
	0, 0, 1, 2, 4, 0, 0, 0,
	0, 0, 48, 72, 40, 120, 0, 0,
	0, 0, 127, 72, 72, 48, 0, 0,
	0, 0, 48, 72, 72, 0, 0, 0,
	0, 0, 48, 72, 72, 127, 0, 0,
	0, 0, 48, 88, 88, 16, 0, 0,
	0, 0, 126, 9, 1, 2, 0, 0,
	0, 0, 80, 152, 152, 112, 0, 0,
	0, 0, 127, 8, 8, 112, 0, 0,
	0, 0, 0, 122, 0, 0, 0, 0,
	0, 0, 64, 128, 128, 122, 0, 0,
	0, 0, 127, 16, 40, 72, 0, 0,
	0, 0, 0, 127, 0, 0, 0, 0,
	0, 120, 8, 16, 8, 112, 0, 0,
	0, 0, 120, 8, 8, 112, 0, 0,
	0, 0, 48, 72, 72, 48, 0, 0,
	0, 0, 248, 40, 40, 16, 0, 0,
	0, 0, 16, 40, 40, 248, 0, 0,
	0, 0, 112, 8, 8, 16, 0, 0,
	0, 0, 72, 84, 84, 36, 0, 0,
	0, 0, 8, 60, 72, 32, 0, 0,
	0, 0, 56, 64, 32, 120, 0, 0,
	0, 0, 56, 64, 56, 0, 0, 0,
	0, 56, 64, 32, 64, 56, 0, 0,
	0, 0, 72, 48, 48, 72, 0, 0,
	0, 0, 24, 160, 160, 120, 0, 0,
	0, 0, 100, 84, 84, 76, 0, 0,
	0, 0, 8, 28, 34, 65, 0, 0,
	0, 0, 0, 126, 0, 0, 0, 0,
	0, 0, 65, 34, 28, 8, 0, 0,
	0, 0, 4, 2, 4, 2, 0, 0,
	0, 120, 68, 66, 68, 120, 0, 0,
};

void draw_text(int x, int y, char *text, bool selected) {

    // Declare variables.
    int i, j, k;

    // Loop through the text.
    for (i = 0; text[i] != '\0'; i++) {

        // Check if we have a space.
        if (text[i] == ' ') {
            x += 4;
            continue;
        }

        // Loop through the data-line for each char.
        for (j = 0; j < 8; j++) {

            // Get the data for the char.
            uint8_t char_data = text_font[(text[i] - 33) * 8 + j];

            // Loop through the column for each char.
            for (k = 0; k < 8; k++) {

                // Check if the column/pixel should be drawn.
                if (!selected && (char_data & (1 << k))) {
                    draw_pixel(x + i * 8 + j, y + k);
                }

                if (selected && !(char_data & (1 << k))) {
                    draw_pixel(x + i * 8 + j, y + k);
                }
            }
        }
    }
}