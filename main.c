/* 
    The example code for using the PCD8544 LCD driver with RP2040 
    Using MIT License
*/

#include <stdio.h>
#include "pico/stdlib.h"
#include "pcd8544.h"

// Define your SPI and GPIO pins here
#define SPI_PORT spi0
#define PIN_SCK  2
#define PIN_MOSI 3
#define PIN_CS   5
#define PIN_DC   4
#define PIN_RST  6

int main() {
    stdio_init_all();

    pcd8544_t lcd;
    pcd8544_pins_t pins = {
        .spi_port = SPI_PORT,
        .sck_pin = PIN_SCK,
        .mosi_pin = PIN_MOSI,
        .cs_pin = PIN_CS,
        .dc_pin = PIN_DC,
        .rst_pin = PIN_RST
    };

    pcd8544_init(&lcd, pins);

    // Test functions
    pcd8544_clear(&lcd);
    pcd8544_draw_string(&lcd, 0, 0, "Hello, RP2040!", 1);
    pcd8544_draw_string(&lcd, 0, 8, "PCD8544 Driver", 1);
    pcd8544_display_update(&lcd);
    sleep_ms(2000);

    pcd8544_clear(&lcd);
    pcd8544_draw_pixel(&lcd, 10, 10, 1);
    pcd8544_draw_pixel(&lcd, 11, 10, 1);
    pcd8544_draw_pixel(&lcd, 10, 11, 1);
    pcd8544_draw_pixel(&lcd, 11, 11, 1);
    pcd8544_display_update(&lcd);
    sleep_ms(1000);

    pcd8544_draw_line(&lcd, 0, 20, 83, 40, 1);
    pcd8544_display_update(&lcd);
    sleep_ms(2000);

    pcd8544_draw_rectangle(&lcd, 5, 5, 20, 15, 1, 0); // Hollow rectangle
    pcd8544_draw_rectangle(&lcd, 30, 25, 30, 20, 1, 1); // Filled rectangle
    pcd8544_display_update(&lcd);
    sleep_ms(2000);

    pcd8544_clear_area(&lcd, 35, 30, 20, 10); // Clear a part of the filled rectangle
    pcd8544_display_update(&lcd);
    sleep_ms(2000);

    pcd8544_clear(&lcd);
    pcd8544_draw_string(&lcd, 0, 0, "ASCII Test:", 1);
    pcd8544_draw_string(&lcd, 0, 8, "!\"#$%&'()*+,-./", 1);
    pcd8544_draw_string(&lcd, 0, 16, "0123456789:;<=>?", 1);
    pcd8544_draw_string(&lcd, 0, 24, "@ABCDEFGHIJKLMNO", 1);
    pcd8544_draw_string(&lcd, 0, 32, "PQRSTUVWXYZ[\\]^_", 1);
    pcd8544_draw_string(&lcd, 0, 40, "`abcdefghijklmno", 1);
    pcd8544_display_update(&lcd);
    sleep_ms(5000);

    while (1) {
        // Keep display on
        sleep_ms(1000);
    }

    return 0;
}


