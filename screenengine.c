#include <pic32mx.h>
#include <stdint.h>
#include "./utilities.h"

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
        }
    }
}